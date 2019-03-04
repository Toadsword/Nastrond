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
#include <iostream>

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

		m_RoadManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<RoadManager>(
			"RoadManager");

		m_Init = true;

		Log::GetInstance()->Msg("Building Manager initialized");

		//DEBUG
		int nmbBuilding = 20000;
		for (int i = 0; i < nmbBuilding; i++)
		{
			SpawnBuilding(static_cast<BuildingType>((std::rand() % static_cast<int>(7)) + 1) , Vec2f(0, 0));
		}
	}

	void BuildingManager::Update(float dt)
	{
		if (!m_Init)
		{
			Init();
		}
	}

	void BuildingManager::FixedUpdate()
	{
	}

	void BuildingManager::Draw() {}

	void BuildingManager::SpawnBuilding(const BuildingType buildingType, const Vec2f position)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->SpawnBuilding(position);
			break;
		case BuildingType::MINE:
			m_ProductionBuildingManager->SpawnBuilding(position, buildingType);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ProductionBuildingManager->SpawnBuilding(position, buildingType);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_ProductionBuildingManager->SpawnBuilding(position, buildingType);
			break;
		case BuildingType::WAREHOUSE:
			m_WarehouseManager->SpawnBuilding(position);
			break;
		case BuildingType::DWELLING:
			m_DwellingManager->SpawnBuilding(position);
			break;
		}
	}

	void BuildingManager::DestroyBuilding(const BuildingType buildingType, const Entity entity)
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

	Entity BuildingManager::AttributeDwarfToWorkingPlace(const BuildingType buildingType)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE: {
			Entity forgeEntity = INVALID_ENTITY;
			forgeEntity = m_ForgeManager->GetBuildingWithFreePlace();

			if (forgeEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ForgeManager->AttributeDwarfToBuilding(forgeEntity);
			return forgeEntity;
		}
		case BuildingType::MINE: {
			Entity mineEntity = INVALID_ENTITY;
			mineEntity = m_ProductionBuildingManager->GetBuildingWithFreePlace(buildingType);

			if (mineEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ProductionBuildingManager->AttributeDwarfToBuilding(mineEntity);
			return mineEntity;
		}
		case BuildingType::EXCAVATION_POST: {
			Entity excavationPostEntity = INVALID_ENTITY;
			excavationPostEntity = m_ProductionBuildingManager->GetBuildingWithFreePlace(buildingType);

			if (excavationPostEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ProductionBuildingManager->AttributeDwarfToBuilding(excavationPostEntity);
			return excavationPostEntity;
		}
		case BuildingType::MUSHROOM_FARM: {
			Entity mushroomFarmEntity = INVALID_ENTITY;
			mushroomFarmEntity = m_ProductionBuildingManager->GetBuildingWithFreePlace(buildingType);

			if (mushroomFarmEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ProductionBuildingManager->AttributeDwarfToBuilding(mushroomFarmEntity);
			return mushroomFarmEntity;
		}
		case BuildingType::WAREHOUSE:
			Entity warehouseEntity = INVALID_ENTITY;
			warehouseEntity = m_WarehouseManager->GetBuildingWithFreePlace();

			if (warehouseEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_WarehouseManager->AttributeDwarfToBuilding(warehouseEntity);
			return warehouseEntity;
		}
		return INVALID_ENTITY;
	}

	void BuildingManager::DeallocateDwarfToWorkingPlace(const BuildingType buildingType, const Entity entity)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->DeallocateDwarfToBuilding(entity);
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

	void BuildingManager::DwarfEnterBuilding(const BuildingType buildingType, const Entity entity)
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

	void BuildingManager::DwarfExitBuilding(const BuildingType buildingType, const Entity entity)
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

	void BuildingManager::DwarfTakesResources(const BuildingType buildingType, const Entity entity, const ResourceType resourceType)
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

	void BuildingManager::DwarfPutsResources(const BuildingType buildingType, const Entity entity, const ResourceType resourceType, const unsigned int resourceQuantity)
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

	void BuildingManager::RegistrationBuildingToBeEmptied(const Entity entity, const BuildingType buildingType, const ResourceType resourceType)
	{
		InventoryTask inventoryTask = INVALID_INVENTORY_TASK;

		inventoryTask.giver = entity;
		inventoryTask.giverType = buildingType;
		inventoryTask.resourceType = resourceType;

		m_BuildingsNeedToBeEmptied.push_back(inventoryTask);


		for (unsigned int i = 0; i < m_BuildingsNeedToBeEmptied.size(); i++)
		{
			const Entity warehouseEntity = m_WarehouseManager->GetWarehouseWithFreeSpaceAvailable(m_BuildingsNeedToBeEmptied[i].resourceType);

			if(warehouseEntity == INVALID_ENTITY)
				break;

			m_WarehouseManager->ReserveForFill(warehouseEntity, m_BuildingsNeedToBeEmptied[i].resourceType);

			m_BuildingsNeedToBeEmptied[i].receiver = warehouseEntity;
			m_BuildingsNeedToBeEmptied[i].receiverType = BuildingType::WAREHOUSE;

			m_InventoryTasks.push_back(m_BuildingsNeedToBeEmptied[i]);
			m_BuildingsNeedToBeEmptied[i] = INVALID_INVENTORY_TASK;

			if(i == m_BuildingsNeedToBeEmptied.size() - 1)
			{
				m_BuildingsNeedToBeEmptied = std::vector<InventoryTask>();
			}
		}
	}

	void BuildingManager::RegistrationBuildingToBeFill(const Entity entity, const BuildingType buildingType, const ResourceType resourceType)
	{
		InventoryTask inventoryTask = INVALID_INVENTORY_TASK;

		inventoryTask.receiver = entity;
		inventoryTask.receiverType = buildingType;
		inventoryTask.resourceType = resourceType;

		m_BuildingsNeedToBeFill.push_back(inventoryTask);


		for (unsigned int i = 0; i < m_BuildingsNeedToBeFill.size(); i++)
		{
			const Entity warehouseEntity = m_WarehouseManager->GetWarehouseWithResources(m_BuildingsNeedToBeFill[i].resourceType);

			if (warehouseEntity == INVALID_ENTITY)
				break;

			m_BuildingsNeedToBeFill[i].giver = warehouseEntity;
			m_BuildingsNeedToBeFill[i].giverType = BuildingType::WAREHOUSE;

			m_InventoryTasks.push_back(m_BuildingsNeedToBeFill[i]);
			m_BuildingsNeedToBeFill[i] = INVALID_INVENTORY_TASK;

			if (i == m_BuildingsNeedToBeFill.size() - 1)
			{
				m_BuildingsNeedToBeFill = std::vector<InventoryTask>();
			}
		}
	}

	Entity BuildingManager::AttributeDwarfToDwelling()
	{
		Entity dwelling = m_DwellingManager->GetBuildingWithFreePlace();

		if (dwelling == INVALID_ENTITY)
			return INVALID_ENTITY;

		m_DwellingManager->AttributeDwarfToBuilding(dwelling);

		return dwelling;
	}

	void BuildingManager::DeallocateDwarfToDwelling(const Entity entity)
	{
		m_DwellingManager->DeallocateDwarfToBuilding(entity);
	}
}
