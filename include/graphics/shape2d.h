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

#include <list>

#include <engine/system.h>
#include <engine/component.h>
#include <engine/transform2d.h>
#include <engine/editor.h>
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

class Shape : public Offsetable, public TransformRequiredComponent
{
public:
	Shape(Transform2d* transform, sf::Vector2f offset);
	virtual ~Shape();
	void Draw(sf::RenderWindow& window) const;
	void SetFillColor(sf::Color color) const;
	void Update(sf::Time dt) const;
protected:
	std::unique_ptr<sf::Shape> m_Shape = nullptr;
};

class Circle : public Shape
{
public:
	Circle(Transform2d* transform, sf::Vector2f offset, float radius);

protected:
	float m_Radius;
};

class Rectangle : public Shape
{
public:
	Rectangle(Transform2d* transform, sf::Vector2f offset, sf::Vector2f size);
protected:
	sf::Vector2f m_Size;

};


class Polygon : public Shape
{
public:
	Polygon(Transform2d* transform, sf::Vector2f offset, std::list<sf::Vector2f>& points);
protected:
	
};

namespace editor
{

struct ShapeInfo : ComponentInfo
{
};
struct CircleShapeInfo : ShapeInfo
{
	void DrawOnInspector() override;
	std::weak_ptr<Circle> circlePtr;
}; 
struct RectShapeInfo : ShapeInfo
{
	void DrawOnInspector() override;
	std::weak_ptr<Rectangle> rectanglePtr;
}; 
}

class ShapeManager : 
	public ComponentManager<std::shared_ptr<Shape>, std::shared_ptr<editor::ShapeInfo>>, public System
{

public:
	ShapeManager(Engine& engine);
	void Init() override;
	void Draw(sf::RenderWindow& window);
	void Update(sf::Time dt) override;
	void Clear() override;

	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;
protected:
	Transform2dManager& m_Transform2dManager;
	EntityManager& m_EntityManager;
};



}
#endif /* SRC_GRAPHICS_SHAPE_H_ */
