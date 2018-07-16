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


#ifndef SFGE_COMPONENT_H
#define SFGE_COMPONENT_H

#include <queue>
#include <vector>

#include <engine/globals.h>
#include <engine/log.h>
#include <engine/entity.h>

namespace sfge
{

enum ComponentType
{
	TRANSFORM = 1 << 0,
	SPRITE = 1 << 1,
	SHAPE = 1 << 2,
	BODY2D = 1 << 3,
	COLLIDER = 1 << 4,
	SOUND = 1 << 5,
	PYCOMPONENT = 1 << 6
};

template<class T>
class ComponentManager
{
public:
	ComponentManager() = default;

	virtual ~ComponentManager()
	{
		m_Components.clear();
	};

	T & GetComponent(Entity entity)
	{
		if(entity == INVALID_ENTITY)
		{
			Log::GetInstance()->Error("Trying to get component from INVALID_ENTITY");
		}
		return m_Components[entity-1];
	}

	std::vector<T>& GetComponents()
	{
		return m_Components;
	}

	virtual void CreateComponent(json& componentJson, Entity entity) = 0;
	virtual void DestroyComponent(Entity entity) = 0;

protected:
	std::vector<T> m_Components{INIT_ENTITY_NMB};
};


class LayerComponent
{
public:
	void SetLayer(int layer);
	int GetLayer() const;
	static bool LayerCompare(LayerComponent* s1, LayerComponent* s2);
protected:
	int m_Layer = 0;
};

template<class T>
class LayerComponentManager
{

};

template<>
class LayerComponentManager<LayerComponent>
{
public:

	virtual ~LayerComponentManager()
	{
		while (!m_LayerComponents.empty())
		{
			m_LayerComponents.pop();
		}
	};
	virtual void FillQueue() = 0;
protected:
	std::priority_queue<
		LayerComponent*, 
	std::deque<LayerComponent*>, 
	std::function<bool(LayerComponent*, LayerComponent*)>> m_LayerComponents{LayerComponent::LayerCompare};
};

class Offsetable
{
public:
	virtual ~Offsetable() = default;
	Offsetable(sf::Vector2f offset);
	sf::Vector2f GetOffset() const;
	virtual void SetOffset(sf::Vector2f offset);
protected:
	sf::Vector2f m_Offset;
};

}
#endif