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

#ifndef MINE_MANAGER_H
#define MINE_MANAGER_H

#include <iostream>

#include <engine/system.h>
#include <graphics/graphics2d.h>

#include "extensions/building_utilities.h"

namespace sfge::ext
{
//#define DEBUG_CHECK_PRODUCTION
#define TEST_SYSTEM_DEBUG
	/**
	 * \author Robin Alves
	 */
	class MineManager : public System
	{
	public:
		MineManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		/**
		 * \brief Spawn forge entity at the given position.
		 */
		void AddNewBuilding(Vec2f pos);

		/**
		 * \brief Destroy the mine at the given index. Return if the entity is not available.
		 */
		bool DestroyBuilding(Entity mineEntity);

		/**
		 * \brief Add dwarf to the dwarf slot struct of the given entity. Return false if there is no slot available or if the entity do not exist.
		 */
		bool AddDwarfToBuilding(Entity mineEntity);

		/**
		 * \brief Remove dwarf to the dwarf slot struct of the given building. Return false if there is no slot available or if the entity do not exist.
		 */
		bool RemoveDwarfToBuilding(Entity mineEntity);

		/**
		 * \brief Return a mine with a slot available for a dwarf. If not
		 */
		Entity GetFreeSlotInBuilding();

		/**
		 * \brief Return a mine with a stack of Iron available.
		 */
		Entity GetBuildingWithResources();

		/**
		 * \brief Return the type of resources that the building produce.
		 */
		ResourceType GetResourceType();

		/**
		 * \brief Return an amount of the produced resources of the given building entity.
		 */
		int GetResourcesBack(Entity entity);

	private:
		/**
		 * \brief Produce resources.
		 */
		void RessourcesProduction();

		/**
		 * \brief Resize all vector to keep the same index for mine.
		 */
		void ResizeContainer(size_t newSize);

		/**
		 * \brief return true if a slot in the index is empty then take his place.
		 */
		bool CheckEmptySlot(Entity newEntity, Transform2d* transformPtr);

		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;

#ifdef TEST_SYSTEM_DEBUG
		const size_t m_EntitiesNmb = 1000;
		size_t m_EntitiesCount = 0;

		const unsigned int m_FramesBeforeAdd = 0u;
		unsigned int m_frameInProgress = 0u;
#endif

		std::vector<Entity> m_EntityIndex;

		std::vector<DwarfSlots> m_DwarfSlots;
		std::vector<GiverInventory> m_IronInventory;

		float m_ProductionRate = 0.01f;
		unsigned int m_StackSize = 20u;
		ResourceType m_ResourceType = ResourceType::IRON;

		//Building texture
		std::string m_TexturePath;
		TextureId m_TextureId;
		sf::Texture* m_Texture;

		//Vertex array
		sf::VertexArray m_VertexArray;
	};
}
#endif
