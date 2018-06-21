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

#include <graphics/graphics.h>
#include <graphics/shape.h>
#include <utility/json_utility.h>
#include <engine/log.h>
#include <engine/transform.h>

namespace sfge
{

Shape::Shape(GameObject* gameObject):
		Component(gameObject)
{
}

void Shape::Init()
{
}

void Shape::Update(float time)
{
	if (m_Shape != nullptr)
	{
		m_Shape->setPosition(m_GameObject->GetTransform()->GetPosition() + m_Offset);
	}
}

void Shape::Draw(sf::RenderWindow& window) const
{
	if(m_Shape != nullptr)
	{
		window.draw(*m_Shape);
	}
}

void Shape::SetFillColor(sf::Color color) const
{
	if (m_Shape != nullptr)
	{
		m_Shape->setFillColor(color);
	}
}


Shape* Shape::LoadShape(Engine& engine, json& componentJson, GameObject* gameObject)
{
	Shape* shape = nullptr;


	if(CheckJsonNumber(componentJson, "shape_type"))
	{
		const auto shapeType = static_cast<ShapeType>(componentJson["shape_type"]);
		switch(shapeType)
		{
		case ShapeType::CIRCLE:
			shape = Circle::LoadCircle(componentJson, gameObject);
			break;
		case ShapeType::RECTANGLE:
			shape = Rectangle::LoadRectangle(componentJson, gameObject);
			break;
		case ShapeType::POLYGON:
			shape = Polygon::LoadPolygon(componentJson, gameObject);
			break;
		default: 
			break;
		}
	}
	const sf::Vector2f offset = GetVectorFromJson(componentJson, "offset");
		
	
	if(shape != nullptr)
	{
		{
			Log::GetInstance()->Msg("Load shape");
		}
		shape->SetOffset(offset);
		auto graphicsManager = engine.GetGraphicsManager();
		graphicsManager->GetShapeManager()->AddShape(shape);
	}
	return shape;
}

Circle::Circle(GameObject* gameObject,   float radius):
		Shape(gameObject)
{
	m_Radius = radius;
	m_Shape = std::make_shared<sf::CircleShape>(radius);
	m_Shape->setFillColor(sf::Color::Green);
	m_Shape->setOrigin(sf::Vector2f(radius, radius));
}

void Circle::Update(float dt)
{
	if (m_Shape != nullptr)
	{
		m_Shape->setPosition(m_GameObject->GetTransform()->GetPosition() + m_Offset);
	}
}



Circle* Circle::LoadCircle(json& componentJson, GameObject* gameObject)
{
	float radius = 1.0f;

	if(CheckJsonNumber(componentJson, "radius"))
	{
		radius = componentJson["radius"];
	}

	return new Circle(gameObject, radius);

}
Rectangle::Rectangle(GameObject* gameObject, sf::Vector2f size):
		Shape(gameObject)
{
	m_Size = size;
	m_Shape = std::make_shared<sf::RectangleShape>(size);
	m_Shape->setFillColor(sf::Color::Red);
	m_Shape->setOrigin(size / 2.0f);
}

void Rectangle::Update(float time)
{
	if (m_Shape != nullptr)
	{
		m_Shape->setPosition(m_GameObject->GetTransform()->GetPosition() + m_Offset);
	}
}

Rectangle* Rectangle::LoadRectangle(json& componentJson, GameObject* gameObject)
{
	const sf::Vector2f size = GetVectorFromJson(componentJson, "size");
	{
		std::ostringstream oss;
		oss << "Loading Rectangle with size: " << size.x << ", " << size.y;
		Log::GetInstance()->Msg(oss.str());
	}
	return new Rectangle(gameObject, size);
}


ShapeManager::ShapeManager(GraphicsManager& graphicsManager):
		m_GraphicsManager(graphicsManager)
{

}


void ShapeManager::Draw(sf::RenderWindow& window)
{
	for(auto shape : m_Shapes)
	{
		shape->Draw(window);
	}
}

void ShapeManager::Reset()
{
	for (auto shape : m_Shapes)
	{
		delete shape;
	}
	m_Shapes.clear();
}

void ShapeManager::Reload()
{
}

void ShapeManager::AddShape(Shape* shape)
{
	m_Shapes.push_back(shape);
}


Polygon::Polygon(GameObject * gameObject, std::list<sf::Vector2f>& points): Shape(gameObject)
{
}

	void Polygon::Update(float time)
{
}

Polygon * Polygon::LoadPolygon(json & componentJson, GameObject * gameObject)
{
	return nullptr;
}

}
