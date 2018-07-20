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
	auto entityManager = m_EntityManagerPtr.lock();
	if (entityManager and entityManager->HasComponent(entity, ComponentType::BODY2D))
	{
		auto & body = m_BodyManager.GetComponent(entity);

		b2FixtureDef fixtureDef;

		std::unique_ptr<b2Shape> shape = nullptr;

		

		if (shape)
		{
			fixtureDef.shape = shape.get();
		}
		body.GetBody()->CreateFixture(&fixtureDef);
	}
}
}
