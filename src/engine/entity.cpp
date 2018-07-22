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

#include <engine/entity.h>
#include <engine/globals.h>

namespace sfge
{
void editor::EntityInfo::DrawOnInspector()
{

}

EntityManager::EntityManager(Engine& engine) : Module(engine)
{
	MaskArray = std::vector<EntityMask>(INIT_ENTITY_NMB, 0U);
}

EntityMask EntityManager::GetMask(Entity entity)
{
	return MaskArray[entity-1];
}

Entity EntityManager::CreateEntity(Entity wantedEntity)
{
    if(wantedEntity == INVALID_ENTITY)
    {
        for (Entity entity = 1U; entity <= MaskArray.size(); entity++)
        {
            if(MaskArray[entity-1] == 0U)
            {
                return entity;
            }
        }
    }
    else
    {
        if(MaskArray[wantedEntity-1] == 0U)
        {
            return wantedEntity;
        }
    }
	return INVALID_ENTITY;
}

bool EntityManager::HasComponent(Entity entity, ComponentType componentType)
{
	return (MaskArray[entity - 1] & (int)componentType) == (int)componentType;
}

void EntityManager::AddComponentType(Entity entity, ComponentType componentType)
{

	MaskArray[entity - 1] = MaskArray[entity - 1] | (int)componentType;
}

void EntityManager::RemoveComponentType(Entity entity, ComponentType componentType)
{
}

editor::EntityInfo& EntityManager::GetEntityInfo(Entity entity)
{
	return m_EntityInfos[entity - 1];
}
}
