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
#include <physics/physics2d.h>
#include <engine/transform2d.h>
#include <engine/entity.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <engine/engine.h>
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

void editor::Body2dInfo::DrawOnInspector()
{
	const auto b2Body = body->GetBody();
	ImGui::Separator();
	ImGui::Text("Body2d");
	switch (b2Body->GetType())
	{
	case b2_staticBody:

		ImGui::LabelText("Type", "Static");
		break;
	case b2_dynamicBody:
		ImGui::LabelText("Type", "Dynamic");
		break;
	case b2_kinematicBody:
		ImGui::LabelText("Type", "Kinematic");
		break;
	}
	float velocity[2] =
	{
		meter2pixel(b2Body->GetLinearVelocity().x),
		meter2pixel(b2Body->GetLinearVelocity().y)
	};
	ImGui::InputFloat2("Velocity", velocity);
	if (ImGui::IsItemHovered())
	{
		auto& velocities = m_Velocities;
		std::vector<float> xValues(velocities.size());
		std::vector<float> yValues(velocities.size());
		for (auto vIndex = 0; vIndex < velocities.size(); vIndex++)
		{
			xValues[vIndex] = velocities[vIndex].x;
			yValues[vIndex] = velocities[vIndex].y;
		}
		//Plot last second velocities
		ImGui::BeginTooltip();
		ImGui::PlotLines("X", &xValues[0], xValues.size(), 0, "", -10.0f, 10.0f, ImVec2(0, 120));
		ImGui::PlotLines("Y", &yValues[0], yValues.size(), 0, "", -10.0f, 10.0f, ImVec2(0, 120));
		ImGui::EndTooltip();
	}
}

void editor::Body2dInfo::AddVelocity(b2Vec2 velocity)
{
	m_Velocities.push_back(velocity);
	if(m_Velocities.size() > m_VelocitiesMaxSize)
	{
		m_Velocities.pop_front();
	}
}

std::deque<b2Vec2>& editor::Body2dInfo::GetVelocities()
{
	return m_Velocities;
}


Body2dManager::Body2dManager(Engine& engine):
	ComponentManager<sfge::Body2d, sfge::editor::Body2dInfo>(),
	System(engine), 
	m_EntityManager(m_Engine.GetEntityManager()),
	m_Transform2dManager(m_Engine.GetTransform2dManager())
{
	m_EntityManager.AddObserver(this);
}

void Body2dManager::Init()
{
	m_Components = std::vector<Body2d>(INIT_ENTITY_NMB, { nullptr, sf::Vector2f() });
	m_ComponentsInfo = std::vector<editor::Body2dInfo>{ INIT_ENTITY_NMB };
	m_WorldPtr = m_Engine.GetPhysicsManager().GetWorld();
}

void Body2dManager::FixedUpdate()
{
	
		for (int i = 0; i < m_Components.size(); i++)
		{
			Entity entity = i + 1;
			if (m_EntityManager.HasComponent(entity, ComponentType::BODY2D) &&
				m_EntityManager.HasComponent(entity, ComponentType::TRANSFORM2D))
			{
				auto & transform = m_Transform2dManager.GetComponent(entity);
				auto & body2d = GetComponent(entity);
				m_ComponentsInfo[i].AddVelocity(body2d.GetLinearVelocity());
				transform.Position = meter2pixel(body2d.GetBody()->GetPosition()) - body2d.GetOffset();
			}
		}
	
}

void Body2dManager::CreateComponent(json& componentJson, Entity entity)
{
	Log::GetInstance()->Msg("Create component Transform");
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


		auto offset = GetVectorFromJson(componentJson, "offset");
		
		
		auto& transform = m_Transform2dManager.GetComponentRef(entity);
		bodyDef.position = pixel2meter(transform.Position + offset);
		
		auto* body = world->CreateBody(&bodyDef);
		m_Components[entity - 1] = Body2d(&transform, offset);
		m_Components[entity - 1].SetBody(body);
		m_ComponentsInfo[entity - 1].body = &m_Components[entity - 1];
	}
}

void Body2dManager::DestroyComponent(Entity entity)
{
}

}

