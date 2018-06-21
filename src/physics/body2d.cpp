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
#include <engine/game_object.h>
#include <engine/transform.h>

namespace sfge
{

void Body2d::Init()
{
}

void Body2d::Update(float dt)
{
	m_GameObject->GetTransform()->SetPosition(meter2pixel(m_Body->GetPosition()));
}

b2Body * Body2d::GetBody()
{
	return m_Body;
}

void Body2d::SetVelocity(b2Vec2 v)
{
	if (m_Body != nullptr)
	{
		m_Body->SetLinearVelocity(v);
	}
}

b2Vec2 Body2d::GetVelocity()
{
	if (m_Body != nullptr)
	{
		return m_Body->GetLinearVelocity();
	}
	return b2Vec2();
}

b2BodyType Body2d::GetBodyType()
{
	if (m_Body != nullptr)
		return m_Body->GetType();
	return b2_staticBody;
}


float Body2d::GetMass()
{
	if (m_Body != nullptr)
	{
		return m_Body->GetMass();
	}
	return 0.0f;
}

void Body2d::AddForce(b2Vec2 f)
{
	if (m_Body != nullptr)
	{
		m_Body->ApplyForce(f, m_Body->GetPosition(), true);
	}
}



Body2d * Body2d::LoadBody2d(Engine & engine, GameObject * gameObject, json& componentJson)
{
	auto physicsManager = engine.GetPhysicsManager();
	if (physicsManager->GetWorld() == nullptr)
	{
		return nullptr;
	}
	b2World* world = physicsManager->GetWorld();

	Body2d* bodyComponent = new Body2d(gameObject);

	b2BodyDef bodyDef;
	bodyDef.userData = bodyComponent;
	bodyDef.type = b2BodyType::b2_dynamicBody;
	if (CheckJsonNumber(componentJson, "body_type"))
	{
		bodyDef.type = componentJson["body_type"];
	}
	bodyDef.position = pixel2meter(gameObject->GetTransform()->GetPosition());
	if (CheckJsonParameter(componentJson, "offset", json::value_t::array))
	{
		if (componentJson["offset"].size() == 2)
		{
			bodyDef.position += pixel2meter(sf::Vector2f(componentJson["offset"]["x"], componentJson["offset"]["y"]));
		}
	}
	if (CheckJsonNumber(componentJson, "gravity_scale"))
	{
		bodyDef.gravityScale = componentJson["gravity_scale"];
	}

	b2Body* body = world->CreateBody(&bodyDef);
	bodyComponent->m_Body = body;
	physicsManager->m_Bodies.push_back(bodyComponent);
	return bodyComponent;
}

}

