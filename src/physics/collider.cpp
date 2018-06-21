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
#include <physics/body2d.h>
#include <physics/physics.h>
namespace sfge
{
void Collider::Init()
{
}
void Collider::Update(float dt)
{
}
void Collider::OnColliderEnter(Collider * collider) const
{
	if (collider->m_Fixture->IsSensor() or m_Fixture->IsSensor())
	{
		m_GameObject->OnTriggerEnter(collider);
	}
	else
	{
		m_GameObject->OnCollisionEnter(collider);
	}
}

void Collider::OnColliderExit(Collider * collider) const
{
	if (collider->m_Fixture->IsSensor() or m_Fixture->IsSensor())
	{
		m_GameObject->OnTriggerExit(collider);
	}
	else
	{
		m_GameObject->OnCollisionExit(collider);
	}
}

bool Collider::IsTrigger() const
{
	if (m_Fixture != nullptr)
		return m_Fixture->IsSensor();
	return false;
}

Collider* Collider::LoadCollider(Engine & engine, GameObject * gameObject, json & componentJson)
{
	auto body2d = gameObject->GetComponent<Body2d>();
	if (body2d != nullptr)
	{
		auto* collider = new Collider(gameObject);
		b2FixtureDef fixtureDef;
		fixtureDef.userData = collider;
		
		ColliderType colliderType = ColliderType::NONE;
		if (CheckJsonNumber(componentJson, "collider_type"))
		{
			colliderType = static_cast<ColliderType>(componentJson["collider_type"]);
		}
		b2Shape* shape = nullptr;
		switch (colliderType)
		{
		case ColliderType::CIRCLE:
			{
				const auto circleShape = new b2CircleShape();
				if (CheckJsonNumber(componentJson, "radius"))
				{
					circleShape->m_radius = pixel2meter(static_cast<float>(componentJson["radius"]));
				}
				//For offset circleShape->m_p 
				shape = circleShape;
			}
		break;
		case ColliderType::RECTANGLE:
			{
				b2PolygonShape* rectShape = new b2PolygonShape();
				const b2Vec2 boxSize = pixel2meter(GetVectorFromJson(componentJson, "size"));
				const auto offset = GetVectorFromJson(componentJson, "offset");

				rectShape->SetAsBox(boxSize.x / 2.0f, boxSize.y / 2.0f, b2Vec2(offset.x, offset.y), 0.0f);
				shape = rectShape;
				break; 
			}
		default: 
			break;
		}
		if (CheckJsonNumber(componentJson, "bouncing"))
		{
			fixtureDef.restitution = componentJson["bouncing"];
		}
		if (shape != nullptr)
		{
			fixtureDef.shape = shape;
		}
		if (CheckJsonParameter(componentJson, "sensor", json::value_t::boolean))
		{
			fixtureDef.isSensor = componentJson["sensor"];
		}
		collider->m_Fixture = body2d->GetBody()->CreateFixture(&fixtureDef);
		if (shape != nullptr)
		{
			delete(shape);
		}
		return collider;
	}
	else
	{
		Log::GetInstance()->Error("No body attached on the GameObject");
	}
	return nullptr;
}

}
