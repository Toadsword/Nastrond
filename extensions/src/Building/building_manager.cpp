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


#include <extensions/Building/building_manager.h>
#include "SFML/Window/WindowImpl.hpp"
#include "extensions/AI/behavior_tree_nodes_extension.h"


namespace sfge::ext
{
	BuildingManager::BuildingManager(Engine & engine) : System(engine) {}

	void BuildingManager::Init()
	{
		m_DwellingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<DwellingManager>(
			"DwellingManager");

		m_ForgeManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<ForgeManager>(
			"ForgeManager");

		m_ProductionBuildingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<ProductionBuildingManager>(
			"ProductionBuildingManager");

		m_WarehouseManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<WarehouseManager>(
			"WarehouseManager");
	}

	void BuildingManager::Update(float dt) {}

	void BuildingManager::FixedUpdate()
	{
	}

	void BuildingManager::Draw() {}

	void BuildingManager::SpawnBuilding(BuildingType buildingType, Vec2f position)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->AddNewBuilding(position);
			break;
		case BuildingType::MINE:
			m_ProductionBuildingManager->AddNewBuilding(position, buildingType);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ProductionBuildingManager->AddNewBuilding(position, buildingType);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_ProductionBuildingManager->AddNewBuilding(position, buildingType);
			break;
		case BuildingType::WAREHOUSE:
			m_WarehouseManager->AddNewBuilding(position);
			break;
		case BuildingType::DWELLING:
			m_DwellingManager->AddNewBuilding(position);
		break;
		}
	}

	void BuildingManager::DestroyBuilding(BuildingType buildingType, Entity entity)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->DestroyBuilding(entity);
			break;
		case BuildingType::MINE:
			m_ProductionBuildingManager->DestroyBuilding(entity, buildingType);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ProductionBuildingManager->DestroyBuilding(entity, buildingType);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_ProductionBuildingManager->DestroyBuilding(entity, buildingType);
			break;
		case BuildingType::WAREHOUSE:
			m_WarehouseManager->DestroyBuilding(entity);
			break;
		}
	}

	Entity BuildingManager::AttributeDwarfToWorkingPlace(BuildingType buildingType)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE: {
			Entity forgeEntity = INVALID_ENTITY;
			forgeEntity = m_ForgeManager->GetFreeSlotInBuilding();

			if (forgeEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ForgeManager->AddDwarfToBuilding(forgeEntity);
			return forgeEntity;
		}
		case BuildingType::MINE: {
			Entity mineEntity = INVALID_ENTITY;
			mineEntity = m_ProductionBuildingManager->GetFreeSlotInBuilding(buildingType);

			if (mineEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ProductionBuildingManager->AddDwarfToBuilding(mineEntity, buildingType);
			return mineEntity;
		}
		case BuildingType::EXCAVATION_POST: {
			Entity excavationPostEntity = INVALID_ENTITY;
			excavationPostEntity = m_ProductionBuildingManager->GetFreeSlotInBuilding(buildingType);

			if (excavationPostEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ProductionBuildingManager->AddDwarfToBuilding(excavationPostEntity, buildingType);
			return excavationPostEntity;
		}
		case BuildingType::MUSHROOM_FARM: {
			Entity mushroomFarmEntity = INVALID_ENTITY;
			mushroomFarmEntity = m_ProductionBuildingManager->GetFreeSlotInBuilding(buildingType);

			if (mushroomFarmEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ProductionBuildingManager->AddDwarfToBuilding(mushroomFarmEntity, buildingType);
			return mushroomFarmEntity;
		}
		case BuildingType::WAREHOUSE:
			Entity warehouseEntity = INVALID_ENTITY;
			warehouseEntity = m_WarehouseManager->GetFreeSlotInBuilding();

			if (warehouseEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_WarehouseManager->AddDwarfToBuilding(warehouseEntity);
			return warehouseEntity;
		}
		return INVALID_ENTITY;
	}

	void BuildingManager::DeallocateDwarfToWorkingPlace(BuildingType buildingType, Entity entity)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->RemoveDwarfToBuilding(entity);
			break;
		case BuildingType::MINE:
			m_ProductionBuildingManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ProductionBuildingManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_ProductionBuildingManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::WAREHOUSE:
			m_WarehouseManager->DwarfEnterBuilding(entity);
			break;
		}

	}

	void BuildingManager::DwarfEnterBuilding(BuildingType buildingType, Entity entity)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::MINE:
			m_ProductionBuildingManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ProductionBuildingManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_ProductionBuildingManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::WAREHOUSE:
			m_WarehouseManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::DWELLING:
			m_DwellingManager->DwarfEnterBuilding(entity);
			break;
		}
	}

	void BuildingManager::DwarfExitBuilding(BuildingType buildingType, Entity entity)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->DwarfExitBuilding(entity);
			break;
		case BuildingType::MINE:
			m_ProductionBuildingManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ProductionBuildingManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_ProductionBuildingManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::WAREHOUSE:
			m_WarehouseManager->DwarfExitBuilding(entity);
			break;
		case BuildingType::DWELLING:
			m_DwellingManager->DwarfExitBuilding(entity);
			break;
		}
	}

	void BuildingManager::DwarfTakesResources(BuildingType buildingType, Entity entity, ResourceType resourceType)
	{
		switch(buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->DwarfTakesResources(entity);
			break;
		case BuildingType::MINE:
			m_ProductionBuildingManager->DwarfTakesResources(entity, buildingType);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ProductionBuildingManager->DwarfTakesResources(entity, buildingType);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_ProductionBuildingManager->DwarfTakesResources(entity, buildingType);
			break;
		case BuildingType::WAREHOUSE:
			m_WarehouseManager->DwarfTakesResources(entity, resourceType);
			break;
		}
	}

	void BuildingManager::DwarfPutsResources(BuildingType buildingType, Entity entity, ResourceType resourceType, unsigned int resourceQuantity)
	{
		switch(buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->DwarfPutsResources(entity);
			break;

		case BuildingType::DWELLING:
			m_DwellingManager->DwarfPutsResources(entity);
			break;

		case BuildingType::WAREHOUSE:
			m_WarehouseManager->DwarfPutsResources(entity, resourceType);
			break;
		}
	}

	BuildingManager::InventoryTask BuildingManager::ConveyorLookForTask()
	{
		for(int i = 0; i < m_InventoryTasks.size(); i++)
		{
			if(!(m_InventoryTasks[i] == INVALID_INVENTORY_TASK))
			{
				const InventoryTask givenInventoryTask = m_InventoryTasks[i];
				m_InventoryTasks[i] = INVALID_INVENTORY_TASK;
				return givenInventoryTask;
			}
			else if (i == m_InventoryTasks.size() - 1)
			{
				m_InventoryTasks = std::vector<InventoryTask>();
			}
		}
		return INVALID_INVENTORY_TASK;
	}

	void BuildingManager::RegistrationBuildingToBeEmptied(Entity entity, BuildingType buildingType, ResourceType resourceType, int resourceQuantity)
	{
		InventoryTask inventoryTask = INVALID_INVENTORY_TASK;

		inventoryTask.giver = entity;
		inventoryTask.giverType = buildingType;
		inventoryTask.resourceType = resourceType;
		inventoryTask.resourceQuantity = resourceQuantity;

		m_BuildingsNeedToBeEmptied.push_back(inventoryTask);


		for (unsigned int i = 0; i < m_BuildingsNeedToBeEmptied.size(); i++)
		{
			const Entity warehouseEntity = m_WarehouseManager->GetWarehouseWithFreeSpaceAvailable(m_BuildingsNeedToBeEmptied[i].resourceType);

			if(warehouseEntity == INVALID_ENTITY)
				break;

			m_WarehouseManager->ReserveFill(warehouseEntity, m_BuildingsNeedToBeEmptied[i].resourceType);

			m_BuildingsNeedToBeEmptied[i].receiver = warehouseEntity;
			m_BuildingsNeedToBeEmptied[i].receiverType = BuildingType::WAREHOUSE;

			m_InventoryTasks.push_back(m_BuildingsNeedToBeEmptied[0]);
			m_BuildingsNeedToBeEmptied[i] = INVALID_INVENTORY_TASK;

			if(i == m_BuildingsNeedToBeEmptied.size() - 1)
			{
				m_BuildingsNeedToBeEmptied = std::vector<InventoryTask>();
			}
		}
	}

	void BuildingManager::RegistrationBuildingToBeFill(Entity entity, BuildingType buildingType, ResourceType resourceType, int resourceQuantity)
	{
		InventoryTask inventoryTask = INVALID_INVENTORY_TASK;

		inventoryTask.receiver = entity;
		inventoryTask.receiverType = buildingType;
		inventoryTask.resourceType = resourceType;
		inventoryTask.resourceQuantity = GetStackSizeByResourceType(resourceType);

		m_BuildingsNeedToBeFill.push_back(inventoryTask);


		for (unsigned int i = 0; i < m_BuildingsNeedToBeFill.size(); i++)
		{
			const Entity warehouseEntity = m_WarehouseManager->GetWarehouseWithResources(m_BuildingsNeedToBeFill[i].resourceType);

			if (warehouseEntity == INVALID_ENTITY)
				break;

			m_BuildingsNeedToBeFill[i].giver = warehouseEntity;
			m_BuildingsNeedToBeFill[i].giverType = BuildingType::WAREHOUSE;

			m_InventoryTasks.push_back(m_BuildingsNeedToBeFill[0]);
			m_BuildingsNeedToBeFill[i] = INVALID_INVENTORY_TASK;

			if (i == m_BuildingsNeedToBeFill.size() - 1)
			{
				m_BuildingsNeedToBeFill = std::vector<InventoryTask>();
			}
		}
	}

	Entity BuildingManager::AttributeDwarfToDwelling()
	{
		Entity dwelling = m_DwellingManager->GetFreeSlotInBuilding();

		if (dwelling == INVALID_ENTITY)
			return INVALID_ENTITY;

		m_DwellingManager->AddDwarfToBuilding(dwelling);

		return dwelling;
	}

	void BuildingManager::DeallocateDwarfToDwelling(Entity entity)
	{
		m_DwellingManager->RemoveDwarfToBuilding(entity);
	}
}
