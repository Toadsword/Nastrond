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

#include <extensions/dwelling_manager.h>
#include <extensions/forge_manager.h>
#include <extensions/production_building_manager.h>
#include <extensions/warehouse_manager.h>


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

		void SpawnBuilding(BuildingType buildingType, Vec2f position);

		void DestroyBuilding(BuildingType buildingType, Entity entity);

		Entity AttributeDwarfToWorkingPlace(BuildingType buildingType);

		void DeallocateDwarfToWorkingPlace(BuildingType buildingType, Entity entity);

		void DwarfEnterBuilding(BuildingType buildingType, Entity entity);

		void DwarfExitBuilding(BuildingType buildingType, Entity entity);

		void DwarfTakesResources(BuildingType buildingType, Entity entity, ResourceType resourceType);

		void DwarfPutsResources(BuildingType buildingType, Entity entity, ResourceType resourceType, unsigned int resourceQuantity);

		InventoryTask ConveyorLookForTask();


		//for dwellings
		Entity AttributeDwarfToDwelling() { return INVALID_ENTITY; };

		void DeallocateDwarfToDwelling(Entity entity) {};


	private:
		DwellingManager* m_DwellingManager;
		ForgeManager* m_ForgeManager;
		ProductionBuildingManager* m_ProductionBuildingManager;
		WarehouseManager* m_WarehouseManager;
	};
}
#endif