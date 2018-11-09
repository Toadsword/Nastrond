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
#include <engine/system.h>
#include <engine/engine.h>

#include <utility/json_utility.h>
#include <engine/vector.h>

namespace sfge
{

enum class ComponentType : int
{
  	NONE = 0,
	TRANSFORM2D = 1 << 0,
	SPRITE2D = 1 << 1,
	SHAPE2D = 1 << 2,
	BODY2D = 1 << 3,
	COLLIDER2D = 1 << 4,
	SOUND = 1 << 5,
	PYCOMPONENT = 1 << 6
};

template<class T, class TInfo>
class BasicComponentManager
{
public:
    BasicComponentManager() = default;
  	BasicComponentManager(const BasicComponentManager&) = delete;
	virtual ~BasicComponentManager()
	{
		m_Components.clear();
		m_ComponentsInfo.clear();
	}

	std::vector<T>& GetComponents()
	{
		return m_Components;
	}

	virtual T* AddComponent(Entity entity) = 0;
	virtual void CreateComponent(json& componentJson, Entity entity) = 0;
	virtual void CreateEmptyComponent(Entity entity)
	{
		json emptyComponent;
		CreateComponent(emptyComponent, entity);
	};
	virtual void DestroyComponent(Entity entity) = 0;
protected:
	std::vector<T> m_Components;
	std::vector<TInfo> m_ComponentsInfo;
};

template<class T, class TInfo>
class SingleComponentManager :
		public BasicComponentManager<T, TInfo>,
		public ResizeObserver,
		public System
{
public:
	SingleComponentManager(Engine& engine): System(engine), BasicComponentManager<T,TInfo>()
	{
		BasicComponentManager<T,TInfo>::m_Components = std::vector<T>{ INIT_ENTITY_NMB };
        BasicComponentManager<T,TInfo>::m_ComponentsInfo = std::vector<TInfo>{ INIT_ENTITY_NMB };
	}

  	SingleComponentManager(SingleComponentManager&& componentManager) = default;
  	SingleComponentManager(const SingleComponentManager& componentManager) = delete;

	void Init() override
	{
		System::Init();
		m_EntityManager = m_Engine.GetEntityManager();
		m_EntityManager->AddObserver(this);
	}

	virtual ~SingleComponentManager()
	{
	}

	TInfo& GetComponentInfo(Entity entity)
	{
		if (entity == INVALID_ENTITY)
		{
			Log::GetInstance()->Error("Trying to get component from INVALID_ENTITY");
		}
		return BasicComponentManager<T,TInfo>::m_ComponentsInfo[entity - 1];
	}

	virtual T* GetComponentPtr(Entity entity)
	{
		if (entity == INVALID_ENTITY)
		{
			Log::GetInstance()->Error("Trying to get component from INVALID_ENTITY");
		}
		return &BasicComponentManager<T,TInfo>::m_Components[entity - 1];
	}

	T& GetComponentRef(Entity entity)
	{
		if (entity == INVALID_ENTITY)
		{
			Log::GetInstance()->Error("Trying to get component from INVALID_ENTITY");
		}
		return BasicComponentManager<T,TInfo>::m_Components[entity - 1];
	}

	void OnResize(size_t newSize) override
	{
		BasicComponentManager<T,TInfo>::m_Components.resize(newSize);
		BasicComponentManager<T,TInfo>::m_ComponentsInfo.resize(newSize);
	}
protected:
	EntityManager* m_EntityManager = nullptr;
};

template<class T, class TInfo>
class MultipleComponentManager : 
	public BasicComponentManager<T,TInfo>, 
	public System, 
	public ResizeObserver
{
	MultipleComponentManager(Engine& engine): System(engine), BasicComponentManager<T,TInfo>()
	{
		
	}
	void Init() override
	{
		System::Init();
		BasicComponentManager<T,TInfo>::m_EntityManager = m_Engine.GetEntityManager();
		BasicComponentManager<T,TInfo>::m_EntityManager->AddObserver(this);
	}
	void OnResize(size_t newSize) override
	{
		BasicComponentManager<T,TInfo>::m_Components.resize(MULTIPLE_COMPONENTS_MULTIPLIER*newSize);
		BasicComponentManager<T,TInfo>::m_ComponentsInfo.resize(MULTIPLE_COMPONENTS_MULTIPLIER*newSize);
	}
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
	Vec2f GetOffset() const;
	virtual void SetOffset(sf::Vector2f offset);
protected:
	Vec2f m_Offset;
};


}
#endif
