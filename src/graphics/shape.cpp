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


void Shape::Draw(sf::RenderWindow& window) const
{
}

void Shape::SetFillColor(sf::Color color) const
{
	
}


Circle::Circle(float radius)
		
{
	m_Radius = radius;
}



Rectangle::Rectangle(sf::Vector2f size)
{
	m_Size = size;
}


ShapeManager::ShapeManager(GraphicsManager& graphicsManager):
		m_GraphicsManager(graphicsManager)
{

}


void ShapeManager::Draw(sf::RenderWindow& window)
{
	for(auto shape : m_Shapes)
	{
	}
}

void ShapeManager::Reset()
{
	for (auto shape : m_Shapes)
	{
	}
	m_Shapes.clear();
}

void ShapeManager::Reload()
{
}


Polygon::Polygon(std::list<sf::Vector2f>& points)
{
}


}
