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

#include <physics/body2d.h>
#include <physics/physics.h>
#include <engine/transform.h>
#include <engine/entity.h>


namespace sfge
{
Body2d::Body2d() : TransformRequiredComponent(nullptr), Offsetable(sf::Vector2f())
{
}

Body2d::Body2d(Transform2d * transform, sf::Vector2f offset) : 
	TransformRequiredComponent(transform), Offsetable(offset)
{
}

b2Vec2 Body2d::GetLinearVelocity()
{
	if (m_Body)
		return m_Body->GetLinearVelocity();
		
	return b2Vec2();
}

void Body2d::SetLinearVelocity(b2Vec2 velocity)
{
	if (m_Body)
		m_Body->SetLinearVelocity(velocity);

}

void Body2d::ApplyForce(b2Vec2 force)
{
	if (m_Body)
		m_Body->ApplyForceToCenter(force, true);
}

b2BodyType Body2d::GetType()
{
	if(m_Body)
		return m_Body->GetType();
	return b2_staticBody;
}

float Body2d::GetMass()
{
	if (m_Body)
		return m_Body->GetMass();
	return 0.0f;
}

void Body2d::SetBody(b2Body * body)
{
	m_Body = body;
}

b2Body * Body2d::GetBody()
{
	return m_Body;
}

Body2dManager::Body2dManager(Engine& engine) : 
	Module(engine)
{
	m_Components = std::vector<Body2d>(INIT_ENTITY_NMB, { nullptr, sf::Vector2f() });
	m_EntityManagerPtr = m_Engine.GetEntityManager();
	m_TransformManagerPtr = m_Engine.GetTransform2dManager();
}

void Body2dManager::Init()
{
	if (const auto physicsManager = m_Engine.GetPhysicsManager().lock())
		m_WorldPtr = physicsManager->GetWorld();
}

void Body2dManager::FixedUpdate()
{
	auto entityManager = m_EntityManagerPtr.lock();
	auto transformManager = m_TransformManagerPtr.lock();
	if (entityManager and transformManager)
	{
		for (int i = 0; i < m_Components.size(); i++)
		{
			Entity entity = i + 1;
			if (entityManager->HasComponent(entity, ComponentType::BODY2D) &&
				entityManager->HasComponent(entity, ComponentType::TRANSFORM))
			{
				auto & transform = transformManager->GetComponent(entity);
				auto & body2d = GetComponent(entity);
				transform.Position = meter2pixel(body2d.GetBody()->GetPosition()) - body2d.GetOffset();
			}
		}
	}
}

void Body2dManager::CreateComponent(json& componentJson, Entity entity)
{
	if (auto world = m_WorldPtr.lock())
	{
		b2BodyDef bodyDef;
		if (CheckJsonExists(componentJson, "body_type"))
		{
			bodyDef.type = componentJson["body_type"];
		}
		if (CheckJsonNumber(componentJson, "gravity_scale"))
		{
			bodyDef.gravityScale = componentJson["gravity_scale"];
		}

		auto* body = world->CreateBody(&bodyDef);
		Transform2d* transform = nullptr;
		if (auto transformManager = m_TransformManagerPtr.lock())
		{
			transform = transformManager->GetComponentPtr(entity);
		}
		auto offset = GetVectorFromJson(componentJson, "offset");

		m_Components[entity - 1] = Body2d(transform, offset);
		m_Components[entity - 1].SetBody(body);
	}
}

void Body2dManager::DestroyComponent(Entity entity)
{
}
}

