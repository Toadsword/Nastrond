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

#include <physics/physics.h>

#include <engine/config.h>
namespace sfge
{

const float PhysicsManager::pixelPerMeter = 100.0f;


void PhysicsManager::Init()
{
	b2Vec2 gravity = b2Vec2();
	if(const auto configPtr = m_Engine.GetConfig().lock())
		gravity = configPtr->gravity;
	m_World = new b2World(gravity);
	m_ContactListener = new ContactListener();
	m_World->SetContactListener(m_ContactListener);

}

void PhysicsManager::Update(sf::Time dt)
{
	if (m_World)
	{
		m_World->Step(dt.asSeconds(), m_VelocityIterations, m_PositionIterations);
	}
}

b2World * PhysicsManager::GetWorld() const
{
	return m_World;
}


void PhysicsManager::Destroy()
{
	if (m_World)
	{
		delete(m_World);
		m_World = nullptr;
	}
	if(m_ContactListener)
	{
		delete(m_ContactListener);
		m_ContactListener = nullptr;
	}

}

void PhysicsManager::Clear()
{
	Destroy();
	Init();
}

void PhysicsManager::Collect()
{
}

void ContactListener::BeginContact(b2Contact* contact) 
{
	//TODO pack b2Contact data and entity
	
}

void ContactListener::EndContact(b2Contact* contact)
{
	//TODO pack b2Contact data and entity
}


float pixel2meter(float pixel)
{
	return pixel/PhysicsManager::pixelPerMeter;
}

float pixel2meter(int pixel)
{
	return pixel / PhysicsManager::pixelPerMeter;
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
	return meter*PhysicsManager::pixelPerMeter;
}

sf::Vector2f meter2pixel(b2Vec2 meter)
{
	return sf::Vector2f(meter2pixel(meter.x), meter2pixel(meter.y));
}

}
