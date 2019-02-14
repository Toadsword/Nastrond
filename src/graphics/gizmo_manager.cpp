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

#include <graphics/graphics2d.h>
#include <graphics/gizmo_manager.h>
#include <utility/json_utility.h>
#include <utility/log.h>
#include <engine/transform2d.h>
#include <engine/engine.h>
#include <imgui.h>
#include <imgui-SFML.h>

namespace sfge
{

Gizmo::Gizmo (): Gizmo(nullptr, sf::Vector2f())
{

}
Gizmo::Gizmo(Transform2d * transform, const sf::Vector2f offset) : Offsetable(offset)
{
	
}

Gizmo::~Gizmo()
{
	//m_Gizmo = nullptr;
}

void Gizmo::Draw(sf::RenderWindow& window) const
{

		//window.draw(*m_Gizmo);
		sf::VertexArray quad(sf::Quads, 4);

		// on le définit comme un rectangle, placé en (10, 10) et de taille 100x100
		quad[0].position = sf::Vector2f(40, 40);
		quad[1].position = sf::Vector2f(140, 40);
		quad[2].position = sf::Vector2f(140, 140);
		quad[3].position = sf::Vector2f(40, 140);

		quad[0].color = sf::Color::Green;
		quad[0].color = sf::Color::Green;
		quad[0].color = sf::Color::Green;
		quad[0].color = sf::Color::Green;

		window.draw(quad);
}


/*
for circle, use : You approximate a circle with a polygon of many points. The points have the following positions:
center.x + radius * cos(angle)
center.y + radius * sin(angle)
angle is increased stepwise, so it's uniformly distributed in [0, 2*pi[.
*/
void Gizmo::Update(float dt, Transform2d* transform) const
{
	(void) dt;
	auto newPosition = m_Offset;

	if(transform != nullptr)
	{
		newPosition += transform->Position;
	}
}

void GizmoManager::Init()
{
	MultipleComponentManager::Init();
	m_Transform2dManager = m_Engine.GetTransform2dManager();
}


void GizmoManager::DrawGizmo(sf::RenderWindow &window)
{

	rmt_ScopedCPUSample(GizmoDraw,0)
	for(auto i = 0u; i < m_Components.size(); i++)
	{
		if(m_EntityManager->HasComponent(i + 1, ComponentType::GIZMO2D))
		{
			m_Components[i].Draw(window);
		}
	}
}



void GizmoManager::Update(const float dt)
{

	rmt_ScopedCPUSample(GizmoUpdate,0)
	for (auto i = 0u; i < m_Components.size(); i++)
	{
		if (m_EntityManager->HasComponent(i + 1, ComponentType::GIZMO2D) && m_EntityManager->HasComponent(i + 1, ComponentType::TRANSFORM2D))
		{
			m_Components[i].Update(dt, m_Transform2dManager->GetComponentPtr(i + 1));
		}
	}
	
}

void GizmoManager::Clear()
{
	m_Components.clear ();
	m_Components.resize (INIT_ENTITY_NMB);

}



Gizmo *GizmoManager::AddComponent (Entity entity)
{
	auto GizmoPtr = GetComponentPtr (entity);
	//tComponentInfo (entity).gizmoPtr = GizmoPtr;
	m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::GIZMO2D);
	m_ComponentsInfo[entity - 1].SetEntity(entity);
	return GizmoPtr;
}



void GizmoManager::CreateComponent(json& componentJson, Entity entity)
{
	//Log::GetInstance()->Msg("Create component Gizmo");
	sf::Vector2f offset;
	if (CheckJsonExists(componentJson, "offset"))
	{
		offset = GetVectorFromJson(componentJson, "offset");
	}

	auto& gizmo = m_Components[entity-1];
	gizmo.SetOffset(offset);

	
	
	
}

void GizmoManager::DestroyComponent(Entity entity)
{
	(void) entity;
}

void GizmoManager::OnResize(size_t new_size)
{
	m_Components.resize(new_size);
	m_ComponentsInfo.resize(new_size);

	for (size_t i = 0; i < new_size; ++i) {
		m_ComponentsInfo[i].SetEntity(i + 1);
		m_ComponentsInfo[i].gizmoPtr = &m_Components[i];
	}
}

}


