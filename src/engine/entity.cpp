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

#include <engine/engine.h>
#include <engine/config.h>
#include <engine/entity.h>
#include <engine/globals.h>

namespace sfge
{
void editor::EntityInfo::DrawOnInspector()
{

}

void EntityManager::Init()
{
	Clear();
}

void EntityManager::Clear()
{
	m_MaskArray = std::vector<EntityMask>(INIT_ENTITY_NMB, INVALID_ENTITY);
}

EntityMask EntityManager::GetMask(Entity entity)
{
	return m_MaskArray[entity-1];
}

Entity EntityManager::CreateEntity(Entity wantedEntity)
{
    if(wantedEntity == INVALID_ENTITY)
    {
        for (Entity entity = 1U; entity <= m_MaskArray.size(); entity++)
        {
            if(m_MaskArray[entity-1] == INVALID_ENTITY)
            {
                return entity;
            }
        }
    }
    else
    {
        if(m_MaskArray[wantedEntity-1] == INVALID_ENTITY)
        {
            return wantedEntity;
        }
    }
	return INVALID_ENTITY;
}

bool EntityManager::HasComponent(Entity entity, ComponentType componentType)
{
	return (m_MaskArray[entity - 1] & (int)componentType) == (int)componentType;
}

void EntityManager::AddComponentType(Entity entity, ComponentType componentType)
{

	m_MaskArray[entity - 1] = m_MaskArray[entity - 1] | (int)componentType;
}

void EntityManager::RemoveComponentType(Entity entity, ComponentType componentType)
{
}

editor::EntityInfo& EntityManager::GetEntityInfo(Entity entity)
{
	return m_EntityInfos[entity - 1];
}

void EntityManager::ResizeEntityNmb(size_t newSize)
{
	m_MaskArray.resize(newSize);
	m_EntityInfos.resize(newSize);
	for (auto* resizeObserver : m_ResizeObsververs)
	{
		resizeObserver->OnResize(newSize);
	}
	if(auto config = m_Engine.GetConfig().lock())
	{
		config->currentEntitiesNmb = newSize;
	}
}

void EntityManager::AddObserver(ResizeObserver* resizeObserver)
{
	m_ResizeObsververs.push_back(resizeObserver);
}
}
