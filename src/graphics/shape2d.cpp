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
#include <engine/log.h>
#include <engine/transform2d.h>
#include <engine/engine.h>
#include <imgui.h>
#include <imgui-SFML.h>

namespace sfge
{
Shape::Shape(Transform2d * transform, const sf::Vector2f offset) : 
	Offsetable(offset), TransformRequiredComponent(transform)
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

void Shape::Update(float dt) const
{
	if(m_Shape and m_Transform)
	{
		m_Shape->setPosition(m_Transform->Position + m_Offset);
	}
}

Circle::Circle(Transform2d* transform, sf::Vector2f offset, float radius) : Shape(transform, offset)
{
	m_Radius = radius;
	m_Shape = std::make_unique<sf::CircleShape>(m_Radius);
	m_Shape->setOrigin(radius, radius);
	if(transform)
	m_Shape->setPosition(transform->Position + offset);
}

Rectangle::Rectangle(Transform2d* transform, sf::Vector2f offset, sf::Vector2f size) : Shape(transform, offset)
{

	m_Size = size;
	m_Shape = std::make_unique<sf::RectangleShape>(m_Size);
	m_Shape->setOrigin(size / 2.0f);
	if(transform)
		m_Shape->setPosition(transform->Position + offset);
}


Polygon::Polygon(Transform2d* transform, sf::Vector2f offset, std::list<sf::Vector2f>& points) : Shape(transform, offset)
{
	//TODO Add the possibility for polygon
}

ShapeManager::ShapeManager(Engine& engine):
	System(engine),
	m_Transform2dManager(m_Engine.GetTransform2dManager()),
	m_EntityManager(m_Engine.GetEntityManager())
{
}

void editor::CircleShapeInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Circle Shape");
	const auto circle = circlePtr;
	if (circle)
	{
		float offset[2] =
		{
			circle->GetOffset().x,
			circle->GetOffset().y
		};
		ImGui::InputFloat2("Offset", offset);
	}
}

void editor::RectShapeInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Rectangle Shape");
	const auto rect = rectanglePtr;
	if (rect)
	{
		float offset[2] =
		{
			rect->GetOffset().x,
			rect->GetOffset().y
		};
		ImGui::InputFloat2("Offset", offset);
	}
}


void ShapeManager::Init()
{

	m_EntityManager.AddObserver(this);
}


void ShapeManager::Draw(sf::RenderWindow& window)
{

	
		for(int i = 0; i < m_Components.size(); i++)
		{
			if(m_Components[i] and m_EntityManager.HasComponent(i + 1, ComponentType::SHAPE2D))
			{
				m_Components[i]->Draw(window);
			}
		}
	
}

void ShapeManager::Update(float dt)
{
	
	for (int i = 0; i < m_Components.size(); i++)
	{
		if (m_Components[i] and m_EntityManager.HasComponent(i+1, ComponentType::SHAPE2D))
		{
			m_Components[i]->Update(dt);
		}
	}
	
}

void ShapeManager::Clear()
{
	m_Components = std::vector<std::unique_ptr<Shape>>{ INIT_ENTITY_NMB };
}

Shape* ShapeManager::GetShapePtr(Entity entity)
{
	if (entity == INVALID_ENTITY)
	{
		Log::GetInstance()->Error("Trying to get component from INVALID_ENTITY");
	}
	return m_Components[entity - 1].get();
}

editor::ShapeInfo* ShapeManager::GetShapeInfoPtr(Entity entity)
{
	if (entity == INVALID_ENTITY)
	{
		Log::GetInstance()->Error("Trying to get component from INVALID_ENTITY");
	}
	return m_ComponentsInfo[entity - 1].get();
}


void ShapeManager::CreateComponent(json& componentJson, Entity entity)
{
	Log::GetInstance()->Msg("Create component Shape");
	
	if (CheckJsonNumber(componentJson, "shape_type"))
	{
		const ShapeType shapeType = componentJson["shape_type"];
		switch (shapeType)
		{
		case ShapeType::CIRCLE:
		{
			sf::Vector2f offset;
			if (CheckJsonExists(componentJson, "offset"))
			{
				offset = GetVectorFromJson(componentJson, "offset");
			}
			float radius = 10.0f;
			if (CheckJsonNumber(componentJson, "radius"))
			{
				radius = componentJson["radius"];
			}
			auto circle = std::make_unique<Circle>(
				m_Transform2dManager.GetComponentPtr(entity),
				offset,
				radius);

			auto circleShapeInfo = std::unique_ptr<editor::CircleShapeInfo>();
			circleShapeInfo->circlePtr = circle.get();
			m_Components[entity - 1] = std::move(circle);
			m_ComponentsInfo[entity - 1] = std::move(circleShapeInfo);
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
			auto rect = std::make_unique<Rectangle>(
				m_Transform2dManager.GetComponentPtr(entity),
				offset,
				size);

			auto rectShapeInfo = std::unique_ptr<editor::RectShapeInfo>();

			rectShapeInfo->rectanglePtr = rect.get();
			m_Components[entity - 1] = std::move(rect);
			m_ComponentsInfo[entity - 1] = std::move(rectShapeInfo);
			
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
}

void ShapeManager::OnResize(size_t new_size)
{
	m_Components.resize(new_size);
	m_ComponentsInfo.resize(new_size);
}
}


