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
#include <graphics/gizmo2d.h>
#include <engine/transform2d.h>

namespace sfge
{

Gizmo::Gizmo (): Offsetable(sf::Vector2f())
{

}

Gizmo::Gizmo(Transform2d * transform, const sf::Vector2f offset) : Offsetable(offset)
{
	
}

Gizmo::~Gizmo()
{
	m_Gizmo = nullptr;
}

void Gizmo::Draw(sf::RenderWindow& window) const
{

		//window.draw(*m_Gizmo);
}

void Gizmo::Update(float dt, Transform2d* transform) const
{
	(void) dt;
	auto newPosition = m_Offset;

	if(transform != nullptr)
	{
		newPosition += transform->Position;
	}
}

GizmoManager::~GizmoManager() {

}

GizmoManager::GizmoManager(Engine &engine) : MultipleComponentManager(engine)
{

}
void Gizmo::SetShape(std::unique_ptr<Gizmo> gizmo)
{
	m_Gizmo = std::move(gizmo);
}

/*Gizmo *Gizmo::GetShape()
{
	return m_Gizmo.get();
}*/

void editor::GizmoInfo::DrawOnInspector() {

}


void GizmoManager::Init()
{
	MultipleComponentManager::Init();
	m_Transform2dManager = m_Engine.GetTransform2dManager();
}




void GizmoManager::DrawGizmos(sf::RenderWindow &window)
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



void GizmoManager::Update(float dt)
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

void GizmoManager::Collect() {

}


Gizmo *GizmoManager::AddComponent (Entity entity)
{
	auto GizmoPtr = GetComponentPtr(entity);
	//auto& ComponentInfo = GetComponentInfo(entity);
	m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::GIZMO2D);
	m_ComponentsInfo[entity - 1].SetEntity(entity);
	return GizmoPtr;
}



void GizmoManager::CreateComponent(json& componentJson, Entity entity)
{
	//Log::GetInstance()->Msg("Create component Shape");
	sf::Vector2f offset;
	if (CheckJsonExists(componentJson, "offset"))
	{
		offset = GetVectorFromJson(componentJson, "offset");
	}

	auto& gizmo = m_Components[entity - 1];
	gizmo.SetOffset(offset);

	/*auto shapeInfo = editor::ShapeInfo();
	shapeInfo.shapePtr = &shape;*/

	//Todo Change For Gizmo
	if (CheckJsonNumber(componentJson, "gizmo_type"))
	{
		const GizmoType gizmoType = componentJson["gizmo_type"];
		switch (gizmoType)
		{
		case GizmoType::CIRCLE:
		{

/*
for circle, use : You approximate a circle with a polygon of many points. The points have the following positions:
center.x + radius * cos(angle)
center.y + radius * sin(angle)
angle is increased stepwise, so it's uniformly distributed in [0, 2*pi[.
*/

			/*float radius = 10.0f;
			if (CheckJsonNumber(componentJson, "radius"))
			{
				radius = componentJson["radius"];
			}

			auto circleShape = std::make_unique <sf::CircleShape>();
			circleShape->setRadius(radius);
			circleShape->setOrigin(radius, radius);
			gizmo.SetShape(std::move(circleShape));*/
			gizmo.Update(0.0f, m_Transform2dManager->GetComponentPtr(entity));
		}
		break;
		case GizmoType::QUAD:
		{
			sf::VertexArray quad(sf::Quads, 4);

			// define here the vertex Array for a quad
			
			quad[0].position = sf::Vector2f(40, 40);
			quad[1].position = sf::Vector2f(140, 40);
			quad[2].position = sf::Vector2f(140, 140);
			quad[3].position = sf::Vector2f(40, 140);

			quad[0].color = sf::Color::Green;
			quad[1].color = sf::Color::Green;
			quad[2].color = sf::Color::Green;
			quad[3].color = sf::Color::Green;
			
			//window.draw(quad);

			/*sf::Vector2f offset;
			if (CheckJsonExists(componentJson, "offset"))
			{
				offset = GetVectorFromJson(componentJson, "offset");
			}
			sf::Vector2f size = sf::Vector2f();
			if (CheckJsonExists(componentJson, "size"))
			{
				size = GetVectorFromJson(componentJson, "size");
			}
			auto rect = std::make_unique<sf::RectangleShape>();
			rect->setSize(size);
			rect->setOrigin(size.x / 2.0f, size.y / 2.0f);
			gizmo.SetShape(std::move(rect));*/
			gizmo.Update(0.0f, m_Transform2dManager->GetComponentPtr(entity));

		}
		break;
		case GizmoType::LINE: 
		{
			//lines for vertex Array gizmo
			gizmo.Update(0.0f, m_Transform2dManager->GetComponentPtr(entity));
		}
		break;
		default:
			Log::GetInstance()->Error("Invalid shape type in GizmoManager Component Creation");
			break;
		}
	}
	else
	{
		std::ostringstream oss;
		oss << "[Error] No shape_type defined in json:  " << componentJson;
		Log::GetInstance()->Error(oss.str());
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

int GizmoManager::GetFreeComponentIndex() {
	return 0;
}

Gizmo* GizmoManager::GetComponentPtr(Entity entity) {
	return nullptr;
}

}


