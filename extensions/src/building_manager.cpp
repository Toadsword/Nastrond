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
		m_MineManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<MineManager>(
			"MineManager");
	}

	void BuildingManager::Update(float dt){}

	void BuildingManager::FixedUpdate(){}

	void BuildingManager::Draw(){}

	void BuildingManager::SpawnBuilding(BuildingType buildingType, Vec2f position)
	{
		switch (buildingType)
		{
		case BuildingType::FORGE:
			m_ForgeManager->AddNewBuilding(position);
			break;
		case BuildingType::MINE:
			m_MineManager->AddNewBuilding(position);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ExcavationPostManager->AddNewBuilding(position);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_MushroomFarmManager->AddNewBuilding(position);
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
			m_MineManager->DestroyBuilding(entity);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ExcavationPostManager->DestroyBuilding(entity);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_MushroomFarmManager->DestroyBuilding(entity);
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
			mineEntity = m_MineManager->GetFreeSlotInBuilding();

			if (mineEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_MineManager->AddDwarfToBuilding(mineEntity);
			return mineEntity;
		}
		case BuildingType::EXCAVATION_POST: {
			Entity excavationPostEntity = INVALID_ENTITY;
			excavationPostEntity = m_ExcavationPostManager->GetFreeSlotInBuilding();

			if (excavationPostEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_ExcavationPostManager->AddDwarfToBuilding(excavationPostEntity);
			return excavationPostEntity;
			}
		case BuildingType::MUSHROOM_FARM: {
			Entity mushroomFarmEntity = INVALID_ENTITY;
			mushroomFarmEntity = m_MushroomFarmManager->GetFreeSlotInBuilding();

			if (mushroomFarmEntity == INVALID_ENTITY)
				return INVALID_ENTITY;

			m_MushroomFarmManager->AddDwarfToBuilding(mushroomFarmEntity);
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
			m_MineManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ExcavationPostManager->DwarfEnterBuilding(entity);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_MushroomFarmManager->DwarfEnterBuilding(entity);
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
			m_MineManager->DwarfExitBuilding(entity);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ExcavationPostManager->DwarfExitBuilding(entity);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_MushroomFarmManager->DwarfExitBuilding(entity);
			break;
		case BuildingType::WAREHOUSE:
			m_WarehouseManager->DwarfExitBuilding(entity);
			break;
		}
	}


}
