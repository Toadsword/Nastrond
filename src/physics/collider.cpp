/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <physics/collider.h>
#include <engine/globals.h>
#include <engine/component.h>
#include <physics/physics.h>

namespace sfge
{
ColliderManager::ColliderManager(Body2dManager& bodyManager) : 
	m_BodyManager(bodyManager)
{
	m_ColliderDatas.reserve(INIT_ENTITY_NMB * 4);
	m_EntityManagerPtr = bodyManager.GetEngine().GetEntityManager();
}

void ColliderManager::CreateComponent(json& componentJson, Entity entity)
{
	Log::GetInstance()->Msg("Create component Collider");
	auto entityManager = m_EntityManagerPtr.lock();
	if (entityManager and entityManager->HasComponent(entity, ComponentType::BODY2D))
	{
		auto & body = m_BodyManager.GetComponent(entity);

		b2FixtureDef fixtureDef;

		if (CheckJsonExists(componentJson, "sensor"))
		{
			fixtureDef.isSensor = componentJson["sensor"];
		}

		std::unique_ptr<b2Shape> shape = nullptr;

		if (CheckJsonExists(componentJson, "collider_type"))
		{
			ColliderType colliderType = static_cast<ColliderType>(componentJson["collider_type"]);
			switch (colliderType)
			{
			case ColliderType::CIRCLE:
				shape = std::make_unique<b2CircleShape>();
				if (CheckJsonNumber(componentJson, "radius"))
				{
					shape->m_radius = pixel2meter((float)componentJson["radius"]);
				}
				break;
			case ColliderType::BOX:
			{
				auto boxShape = std::make_unique<b2PolygonShape>();
				if (CheckJsonExists(componentJson, "size"))
				{
					auto size = pixel2meter(GetVectorFromJson(componentJson, "size"));
					{
						std::ostringstream oss;
						oss << "Box physics size: " << size.x << ", " << size.y;
						Log::GetInstance()->Msg(oss.str());
					}
					boxShape->SetAsBox(size.x / 2.0f, size.y / 2.0f);
				}
				shape = std::move(boxShape);
			}	
			break;
			default:
			{
				std::ostringstream oss;
				oss << "[Error] Collider of type: " << (int)colliderType << " could not be loaded from json: " << componentJson;
				Log::GetInstance()->Error(oss.str());
			}
				break;
			}
		}
		if (shape)
		{
			fixtureDef.shape = shape.get();

			auto fixture = body.GetBody()->CreateFixture(&fixtureDef);

			m_ColliderDatas.push_back(ColliderData());
			ColliderData& colliderData = m_ColliderDatas.back();
			colliderData.entity = entity;
			colliderData.fixture = fixture;
			colliderData.body = body.GetBody();
			fixture->SetUserData(&colliderData);
		}
	}
}
}
