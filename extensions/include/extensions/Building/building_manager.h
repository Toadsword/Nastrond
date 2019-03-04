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

#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H

#include <engine/system.h>
#include <python/python_engine.h>

#include <extensions/Building/dwelling_manager.h>
#include <extensions/Building/forge_manager.h>
#include <extensions/Building/production_building_manager.h>
#include <extensions/Building/warehouse_manager.h>
#include <extensions/Building/road_manager.h>


namespace sfge::ext
{
	/**
	 * \author Robin Alves
	 */
	class BuildingManager : public System
	{
	public:
		BuildingManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		struct InventoryTask
		{
			Entity giver = INVALID_ENTITY;
			Entity receiver = INVALID_ENTITY;
			BuildingType giverType = BuildingType::NO_BUILDING_TYPE;
			BuildingType receiverType = BuildingType::NO_BUILDING_TYPE;
			ResourceType resourceType = ResourceType::NO_RESOURCE_TYPE;
			char resourceQuantity = 0;

			bool operator==(const InventoryTask& rhs) const
			{
				return receiver == rhs.receiver && 
					   giver == rhs.receiver && 
					   giverType == rhs.giverType && 
					   receiverType == rhs.receiverType && 
					   resourceQuantity == rhs.resourceQuantity && 
					   resourceType == rhs.resourceType;
			}
		};

		InventoryTask INVALID_INVENTORY_TASK = InventoryTask();

		void SpawnBuilding(const BuildingType buildingType, const Vec2f position);

		void DestroyBuilding(const BuildingType buildingType, const Entity entity);

		Entity AttributeDwarfToWorkingPlace(const BuildingType buildingType);

		void DeallocateDwarfToWorkingPlace(const BuildingType buildingType, const Entity entity);

		void DwarfEnterBuilding(const BuildingType buildingType, const Entity entity);

		void DwarfExitBuilding(const BuildingType buildingType, const Entity entity);

		void DwarfTakesResources(const BuildingType buildingType, const Entity entity, const ResourceType resourceType);

		void DwarfPutsResources(const BuildingType buildingType, const Entity entity, const ResourceType resourceType, const unsigned int resourceQuantity);

		InventoryTask ConveyorLookForTask();

#pragma region Move resources section
		void RegistrationBuildingToBeEmptied(const Entity entity, const BuildingType buildingType, const ResourceType resourceType);

		void RegistrationBuildingToBeFill(const Entity entity, const BuildingType buildingType, const ResourceType resourceType);
#pragma endregion

		//for dwellings
		Entity AttributeDwarfToDwelling();

		void DeallocateDwarfToDwelling(const Entity entity);


	private:
		DwellingManager* m_DwellingManager;
		ForgeManager* m_ForgeManager;
		ProductionBuildingManager* m_ProductionBuildingManager;
		WarehouseManager* m_WarehouseManager;
		RoadManager* m_RoadManager;

		bool m_Init = false;

		std::vector<InventoryTask> m_BuildingsNeedToBeEmptied;
		std::vector<InventoryTask> m_BuildingsNeedToBeFill;

		std::vector<InventoryTask> m_InventoryTasks;

		int coolDown = 0;
	};
}
#endif