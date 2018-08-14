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

#include <physics/physics2d.h>
#include <python/python_engine.h>
#include <engine/config.h>
#include <engine/engine.h>
namespace sfge
{

const float Physics2dManager::pixelPerMeter = 100.0f;


void Physics2dManager::Init()
{
	b2Vec2 gravity = b2Vec2();
	if(const auto configPtr = m_Engine.GetConfig().lock())
		gravity = configPtr->gravity;
	m_World = std::make_shared<b2World>(gravity);
	m_ContactListener = std::make_unique<ContactListener>(m_Engine);
	m_World->SetContactListener(m_ContactListener.get());

	m_BodyManager.Init();
}

void Physics2dManager::Update(sf::Time dt)
{
	
}

void Physics2dManager::FixedUpdate()
{
	auto config = m_Engine.GetConfig().lock();
	if (config and m_World)
	{
		m_World->Step(config->fixedDeltaTime,
			config->velocityIterations,
			config->positionIterations);
		m_BodyManager.FixedUpdate();
	}
}

std::weak_ptr<b2World> Physics2dManager::GetWorld() const
{
	return m_World;
}


void Physics2dManager::Destroy()
{
	if (m_World)
	{
		m_World = nullptr;
	}
	if(m_ContactListener)
	{
		m_ContactListener = nullptr;
	}

}

void Physics2dManager::Clear()
{
	Destroy();
	Init();
}

void Physics2dManager::Collect()
{
}


Body2dManager& Physics2dManager::GetBodyManager()
{
	return m_BodyManager;
}

ColliderManager& Physics2dManager::GetColliderManager()
{
	return m_ColliderManager;
}


void ContactListener::BeginContact(b2Contact* contact)
{
	auto pythonEngine = m_Engine.GetPythonEngine();
	auto colliderA = static_cast<ColliderData*>(contact->GetFixtureA()->GetUserData());
	auto colliderB = static_cast<ColliderData*>(contact->GetFixtureB()->GetUserData());

	if (colliderA->fixture->IsSensor() or colliderB->fixture->IsSensor())
	{
		//Trigger
		
		pythonEngine.OnTriggerEnter(colliderA->entity, colliderB);
		pythonEngine.OnTriggerEnter(colliderB->entity, colliderA);
		
	}
	else
	{
		//Collision
			pythonEngine.OnCollisionEnter(colliderA->entity, colliderB);
			pythonEngine.OnCollisionEnter(colliderB->entity, colliderA);
		
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	auto pythonEngine = m_Engine.GetPythonEngine();
	auto colliderA = static_cast<ColliderData*>(contact->GetFixtureA()->GetUserData());
	auto colliderB = static_cast<ColliderData*>(contact->GetFixtureB()->GetUserData());

	if (colliderA->fixture->IsSensor() or colliderB->fixture->IsSensor())
	{
		//Trigger
			pythonEngine.OnTriggerExit(colliderA->entity, colliderB);
			pythonEngine.OnTriggerExit(colliderB->entity, colliderA);
		
	}
	else
	{
		//Collision
			pythonEngine.OnCollisionExit(colliderA->entity, colliderB);
			pythonEngine.OnCollisionExit(colliderB->entity, colliderA);
		
	}
}


float pixel2meter(float pixel)
{
	return pixel/Physics2dManager::pixelPerMeter;
}

float pixel2meter(int pixel)
{
	return pixel / Physics2dManager::pixelPerMeter;
}

b2Vec2 pixel2meter(sf::Vector2f pixel)
{
	return b2Vec2(pixel2meter(pixel.x), pixel2meter(pixel.y));
}

b2Vec2 pixel2meter(sf::Vector2i pixel)
{
	return b2Vec2(pixel2meter(pixel.x), pixel2meter(pixel.y));
}

float meter2pixel(float meter)
{
	return meter*Physics2dManager::pixelPerMeter;
}

sf::Vector2f meter2pixel(b2Vec2 meter)
{
	return sf::Vector2f(meter2pixel(meter.x), meter2pixel(meter.y));
}

ContactListener::ContactListener(Engine& engine):
	m_Engine(engine)
{
}
}
