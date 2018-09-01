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

#ifndef SFGE_ENTITY_H
#define SFGE_ENTITY_H

#include <vector>
#include <engine/system.h>
#include <engine/editor.h>
#include <engine/globals.h>
#include <engine/component.h>

namespace sfge
{
enum class ComponentType : int;
template<class T, class TInfo>
class ComponentManager;
class ResizeObserver
{
public:
	virtual void OnResize(size_t new_size) = 0;
};
/**
 * \brief Entity index number, starting from 1U
 */

using EntityMask = int;

namespace editor
{

struct EntityInfo : NamableEditorComponent, IDrawableInspector
{
void DrawOnInspector() override;	
};

}

class EntityManager : public System
{
public:
	using System::System;
	void Init() override;

	void Clear() override;

	EntityMask GetMask(Entity entity);
	Entity CreateEntity(Entity wantedEntity = INVALID_ENTITY);
	bool HasComponent(Entity entity, ComponentType componentType);
	void AddComponentType(Entity entity, ComponentType componentType);
	void RemoveComponentType(Entity entity, ComponentType componentType);
	editor::EntityInfo& GetEntityInfo(Entity entity);

	void ResizeEntityNmb(size_t newSize);
	void AddObserver(ResizeObserver* resizeObserver);

private:
	std::vector<EntityMask> m_MaskArray{ INIT_ENTITY_NMB };
	std::vector<editor::EntityInfo> m_EntityInfos{ INIT_ENTITY_NMB };
	std::vector<ResizeObserver*> m_ResizeObsververs{10, nullptr};
};
/*
template <>
void EntityManager::AddObserver(ComponentManager<std::any, std::any>* componentManager)
{
	m_ResizeObsververs.push_back(componentManager);
}
*/

}

#endif 
