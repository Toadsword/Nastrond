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

#ifndef SFGE_SHAPE_H_
#define SFGE_SHAPE_H_

#include <engine/module.h>
#include <engine/component.h>
#include <list>
//Externals
#include <SFML/Graphics.hpp>

namespace sfge
{

enum class ShapeType
{
	NONE,
	CIRCLE,
	RECTANGLE,
	POLYGON,
	CONVEX,
};

class Shape 
{
public:
	void Draw(sf::RenderWindow& window) const;

	void SetFillColor(sf::Color color) const;
protected:
};

class Circle : public Shape
{
public:
	Circle(float radius);
protected:
	float m_Radius;
};

class Rectangle : public Shape
{
public:
	Rectangle(sf::Vector2f size);
protected:
	sf::Vector2f m_Size;

};

class Polygon : public Shape
{
public:
	Polygon(std::list<sf::Vector2f>& points);
protected:
	
};

class ShapeManager : ComponentManager<Shape>
{

public:
	ShapeManager(GraphicsManager& graphicsManager);
	void Draw(sf::RenderWindow& window);

	void Clear();
	void Reload();

	bool CreateComponent() override;
	bool DestroyComponent() override;
protected:
	GraphicsManager& m_GraphicsManager;
};

}
#endif /* SRC_GRAPHICS_SHAPE_H_ */
