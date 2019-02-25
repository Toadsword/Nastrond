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

#ifndef WAREHOUSE_MANAGER_H
#define WAREHOUSE_MANAGER_H

#include <engine/system.h>
#include <graphics/graphics2d.h>

#include <extensions/Building/building_utilities.h>

namespace sfge::ext
{
	/**
	 * \author Robin Alves
	 */
	class WarehouseManager : public System
	{
	public:
		WarehouseManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		/**
		 * \brief Spawn a warehouse entity at the given position.
		 * \param position : the location of spawn wanted.
		 */
		void SpawnBuilding(Vec2f position);

		/**
		 * \brief Destroy the warehouse at the given index.
		 * \param warehouseEntity : an entity that is a warehouse.
		 * \return true if the Entity exist and is a warehouse.
		 */
		bool DestroyBuilding(Entity warehouseEntity);

		/**
		 * \brief attribute a dwarf to the given warehouse entity.
		 * \param warehouseEntity : an entity that is a warehouse.
		 * \return true if the entity exist and is a warehouse and if a dwarf has been added to the building.
		 */
		bool AddDwarfToBuilding(Entity warehouseEntity);

		/**
		 * \brief remove the attribution of a dwarf to the given warehouse entity.
		 * \param warehouseEntity : an entity that is a warehouse.
		 * \return true if the entity exist and is a warehouse and if a dwarf has been added to the building.
		 */
		bool RemoveDwarfToBuilding(Entity warehouseEntity);

		/**
		 * \brief Increment the dwarf slot to tell that a dwarf go in.
		 * \param warehouseEntity : The Entity of the warehouse that the dwarf want to go in.
		 */
		void DwarfEnterBuilding(Entity warehouseEntity);

		/**
		 * \brief decrement the dwarf slot to tell that a dwarf go out.
		 * \param warehouseEntity : The Entity of the warehouse that the dwarf want to go out.
		 */
		void DwarfExitBuilding(Entity warehouseEntity);

		/**
		 * \brief Get a warehouse with a slot free for a dwarf.
		 * \return An entity of a warehouse.
		 */
		Entity GetFreeSlotInBuilding();

		Entity GetWarehouseWithResources(ResourceType resourceType);

		Entity GetWarehouseWithFreeSpaceAvailable(ResourceType resourceType);

		void DwarfTakesResources(Entity entity, ResourceType resourceType);

		void DwarfPutsResources(Entity entity, ResourceType resourceType);

		void ReserveFill(Entity entity, ResourceType resourceType);

		void ReserveEmpty(Entity entity, ResourceType resourceType);

	private:
		/**
		 * \brief check if a slot already setup is empty and fill it.
		 * \param newEntity : the entity that is newly created.
		 * \param transformPtr : the transform of the newly created.
		 * \return true if a slot was empty and has been fill.
		 */
		bool CheckEmptySlot(Entity newEntity);

		void SetupTexture(unsigned int warehouseIndex);

		void ReserveContainer(const size_t newSize);

		void AttributeContainer();


		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;
		Configuration* m_Configuration;


		sf::RenderWindow* m_Window;

		const unsigned short m_MaxCapacity = 200;

		unsigned int m_BuildingIndexCount = 0;
		unsigned int m_NumberReservation = 0;

		std::vector<Entity> m_EntityIndex;

		std::vector<DwarfSlots> m_DwarfSlots;

#pragma region All inventories
		std::vector<unsigned short> m_IronInventories;
		std::vector<unsigned short> m_StoneInventories;
		std::vector<unsigned short> m_ToolInventories;
		std::vector<unsigned short> m_MushroomInventories;

		std::vector<unsigned char> m_ReservedExportStackNumberIron;
		std::vector<unsigned char> m_ReservedExportStackNumberStone;
		std::vector<unsigned char> m_ReservedExportStackNumberTool;
		std::vector<unsigned char> m_ReservedExportStackNumberMushroom;

		std::vector<unsigned char> m_ReservedImportStackNumberIron;
		std::vector<unsigned char> m_ReservedImportStackNumberStone;
		std::vector<unsigned char> m_ReservedImportStackNumberTool;
		std::vector<unsigned char> m_ReservedImportStackNumberMushroom;
#pragma endregion


		//Warehouse texture
		std::string m_TexturePath;
		TextureId m_TextureId;
		sf::Texture* m_Texture;
	};
}
#endif