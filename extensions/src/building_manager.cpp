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


#include <extensions/building_manager.h>


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
	}

	void BuildingManager::Update(float dt) {}

	void BuildingManager::FixedUpdate()
	{
		if (blep)
		{
			//SpawnBuilding(BuildingType::MINE, Vec2f(100, 200));
			//SpawnBuilding(BuildingType::MUSHROOM_FARM, Vec2f(300, 200));
			//SpawnBuilding(BuildingType::DWELLING, Vec2f(300, 400));
			blep = false;
		}
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
		}
	}

	void BuildingManager::DwarfTakesResources(BuildingType buildingType, Entity entity)
	{
	}

	void BuildingManager::DwarfPutsResources(BuildingType buildingType, Entity entity, unsigned int resourceQuantity)
	{

	}

	BuildingManager::InventoryTask BuildingManager::ConveyorLookForTask()
	{
		std::vector<BuildingType> buildings;

		buildings.push_back(BuildingType::FORGE);
		buildings.push_back(BuildingType::MINE);
		buildings.push_back(BuildingType::EXCAVATION_POST);
		buildings.push_back(BuildingType::MUSHROOM_FARM);

		InventoryTask inventoryTask;

		for (int i = 0; i < buildings.size(); i++)
		{
			switch (buildings[i])
			{
			case BuildingType::FORGE:
				inventoryTask.giver = m_ForgeManager->GetBuildingWithResources();
				if (inventoryTask.giver == INVALID_ENTITY)
				{
					break;
				}
				inventoryTask.resourceType = m_ForgeManager->GetProducedResourceType();
				break;
			}
		}

		return InventoryTask();
	}
}
