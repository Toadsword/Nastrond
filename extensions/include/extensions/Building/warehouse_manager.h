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


		void SpawnBuilding(Vec2f position);

		void DestroyBuilding(Entity warehouseEntity);

		void AttributeDwarfToBuilding(Entity warehouseEntity);

		void DeallocateDwarfToBuilding(Entity warehouseEntity);

		void DwarfEnterBuilding(Entity warehouseEntity);

		void DwarfExitBuilding(Entity warehouseEntity);

		Entity GetBuildingWithFreePlace();

		Entity GetWarehouseWithResources(ResourceType resourceType);

		Entity GetWarehouseWithFreeSpaceAvailable(ResourceType resourceType);

		void DwarfTakesResources(Entity entity, ResourceType resourceType);

		void DwarfPutsResources(Entity entity, ResourceType resourceType);

		void ReserveForFill(Entity entity, ResourceType resourceType);

		void ReserveForEmpty(Entity entity, ResourceType resourceType);

	private:

		bool CheckFreeSlot(Entity newEntity);

		void SetupTexture(const Entity entity);

		void ReserveContainer(const size_t newSize);

		void AttributeContainer();


		EntityManager * m_EntityManager;
		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;
		Configuration* m_Configuration;

		bool m_Init = false;

		sf::RenderWindow* m_Window;

		const unsigned short m_MaxCapacity = 200;

		unsigned int m_BuildingIndexCount = 0;
		unsigned int m_NmbReservation = 0;

		std::vector<Entity> m_EntityIndex;

		std::vector<DwarfSlots> m_DwarfSlots;

#pragma region Inventories section
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