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
#include <graphics/shape2d.h>
#include <utility/json_utility.h>
#include <utility/log.h>
#include <engine/transform2d.h>
#include <engine/engine.h>
#include <imgui.h>
#include <imgui-SFML.h>

#define COMPONENT_OPTIMIZATION

namespace sfge
{

Shape::Shape (): Shape (nullptr, sf::Vector2f())
{

}
Shape::Shape(Transform2d * transform, const sf::Vector2f offset) : Offsetable(offset)
{
	
}

Shape::~Shape()
{
	m_Shape = nullptr;
}

void Shape::Draw(sf::RenderWindow& window) const
{
	if (m_Shape != nullptr)
	{
		window.draw(*m_Shape);
	}
}

void Shape::SetFillColor(sf::Color color) const
{
	if (m_Shape)
		m_Shape->setFillColor(color);
}

void Shape::Update(float dt, Transform2d* transform) const
{
	(void) dt;
	auto newPosition = m_Offset;

	if(transform != nullptr)
	{
		newPosition += transform->Position;
	}

	if(m_Shape != nullptr)
	{
		m_Shape->setPosition(newPosition);
	}
}
void Shape::SetShape (std::unique_ptr<sf::Shape> shape)
{
	m_Shape = std::move(shape);
}
sf::Shape *Shape::GetShape ()
{
	return m_Shape.get ();
}

void editor::ShapeInfo::DrawOnInspector ()
{
	if(shapePtr != nullptr && shapePtr->GetShape() != nullptr)
	{
		ImGui::Separator();
		ImGui::Text("Shape");

		float offset[2] =
		{
			shapePtr->GetOffset().x,
			shapePtr->GetOffset().y
		};

		ImGui::InputFloat2("Offset", offset);
		auto circleShape = dynamic_cast<sf::CircleShape*>(shapePtr->GetShape());
		if(circleShape != nullptr)
		{
			float radius = circleShape->getRadius ();
			ImGui::InputFloat ("Radius", &radius);
		}

		auto rectShape = dynamic_cast<sf::RectangleShape*>(shapePtr->GetShape ());
		if(rectShape != nullptr)
		{
			float size[2] =
			{
				rectShape->getSize ().x,
				rectShape->getSize ().y
			};
			ImGui::InputFloat2("Size", size);
		}
	}
}

void ShapeManager::Init()
{
	SingleComponentManager::Init();
	m_Transform2dManager = m_Engine.GetTransform2dManager();
}


void ShapeManager::DrawShapes(sf::RenderWindow &window)
{
	rmt_ScopedCPUSample(ShapeDraw,0)
#ifdef COMPONENT_OPTIMIZATION
	for (auto i = 0u; i < m_ConcernedEntities.size(); i++)
	{
		m_Components[m_ConcernedEntities[i] - 1].Draw(window);
	}
#else
	for(auto i = 0u; i < m_Components.size(); i++)
	{
		if(m_EntityManager->HasComponent(i + 1, ComponentType::SHAPE2D))
		{
			m_Components[i].Draw(window);
		}
	}
#endif
}

void ShapeManager::Update(const float dt)
{

	rmt_ScopedCPUSample(ShapeUpdate,0)
#ifdef COMPONENT_OPTIMIZATION
	for (auto i = 0u; i < m_ConcernedEntities.size(); i++)
	{
		m_Components[m_ConcernedEntities[i] - 1].Update(dt, m_Transform2dManager->GetComponentPtr(m_ConcernedEntities[i]));
	}
#else
	for (auto i = 0u; i < m_Components.size(); i++)
	{
		if (m_EntityManager->HasComponent(i + 1, ComponentType::SHAPE2D) && m_EntityManager->HasComponent(i + 1, ComponentType::TRANSFORM2D))
		{
			m_Components[i].Update(dt, m_Transform2dManager->GetComponentPtr(i + 1));
		}
	}
#endif
}

void ShapeManager::Clear()
{
	m_Components.clear ();
	m_Components.resize (INIT_ENTITY_NMB);

}



Shape *ShapeManager::AddComponent (Entity entity)
{
	auto shapePtr = GetComponentPtr (entity);
	GetComponentInfo (entity).shapePtr = shapePtr;
	m_ConcernedEntities.push_back(entity);
	m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::SHAPE2D);
	m_ComponentsInfo[entity - 1].SetEntity(entity);
	return shapePtr;
}

void ShapeManager::CreateComponent(json& componentJson, Entity entity)
{
	//Log::GetInstance()->Msg("Create component Shape");
	sf::Vector2f offset;
	if (CheckJsonExists(componentJson, "offset"))
	{
		offset = GetVectorFromJson(componentJson, "offset");
	}

	auto* shape = AddComponent(entity);
	shape->SetOffset(offset);

	auto shapeInfo = editor::ShapeInfo();
	shapeInfo.shapePtr = shape;

	if (CheckJsonNumber(componentJson, "shape_type"))
	{
		const ShapeType shapeType = componentJson["shape_type"];
		switch (shapeType)
		{
		case ShapeType::CIRCLE:
		{

			float radius = 10.0f;
			if (CheckJsonNumber(componentJson, "radius"))
			{
				radius = componentJson["radius"];
			}

			auto circleShape = std::make_unique <sf::CircleShape>();
			circleShape->setRadius (radius);
			circleShape->setOrigin (radius, radius);
			shape->SetShape (std::move(circleShape));
			shape->Update(0.0f, m_Transform2dManager->GetComponentPtr(entity));
		}
			break;
		case ShapeType::RECTANGLE:
		{
			sf::Vector2f offset;
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
			rect->setSize (size);
			rect->setOrigin (size.x/2.0f, size.y/2.0f);
            shape->SetShape (std::move (rect));
            shape->Update (0.0f, m_Transform2dManager->GetComponentPtr(entity));
			
		}
			break;
		default:
			Log::GetInstance()->Error("Invalid shape type in ShapeManager Component Creation");
			break;
		}
	}
	else
	{
		std::ostringstream oss;
		oss << "[Error] No shape_type defined in json:  "<<componentJson;
		Log::GetInstance()->Error(oss.str());
	}
	
	
}

void ShapeManager::DestroyComponent(Entity entity)
{
	RemoveConcernedEntity(entity);
	(void) entity;
}

void ShapeManager::OnResize(size_t new_size)
{
	m_Components.resize(new_size);
	m_ComponentsInfo.resize(new_size);

	for (size_t i = 0; i < new_size; ++i) {
		m_ComponentsInfo[i].SetEntity(i + 1);
		m_ComponentsInfo[i].shapePtr = &m_Components[i];
	}
}

}


