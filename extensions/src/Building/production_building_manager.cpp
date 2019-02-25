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

#include <extensions/Building/production_building_manager.h>
#include <extensions/Building/building_manager.h>


namespace sfge::ext
{

	ProductionBuildingManager::ProductionBuildingManager(Engine& engine) : System(engine) {}

	void ProductionBuildingManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
		m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();
		m_Configuration = m_Engine.GetConfig();
		m_BuildingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<BuildingManager>(
			"BuildingManager");

		m_Window = m_Engine.GetGraphics2dManager()->GetWindow();

		//Load Texture for mine
		m_MineTexturePath = "data/sprites/mine.png";
		m_MineTextureId = m_TextureManager->LoadTexture(m_MineTexturePath);
		m_MineTexture = m_TextureManager->GetTexture(m_MineTextureId);

		//Load Texture for excavation cave
		m_ExcavationPostTexturePath = "data/sprites/excavationCave.png";
		m_ExcavationPostTextureId = m_TextureManager->LoadTexture(m_ExcavationPostTexturePath);
		m_ExcavationPostTexture = m_TextureManager->GetTexture(m_ExcavationPostTextureId);

		//Load Texture for mushroom farm
		m_MushroomFarmTexturePath = "data/sprites/mushroomFarm.png";
		m_MushroomFarmTextureId = m_TextureManager->LoadTexture(m_MushroomFarmTexturePath);
		m_MushroomFarmTexture = m_TextureManager->GetTexture(m_MushroomFarmTextureId);
		m_Init = true;
	}

	void ProductionBuildingManager::Update(float dt)
	{
		if(!m_Init)
		{
			Init();
		}
	}

	void ProductionBuildingManager::FixedUpdate()
	{
		Produce();
	}

	void ProductionBuildingManager::Draw()
	{
	}



	void ProductionBuildingManager::SpawnBuilding(Vec2f position, BuildingType buildingType)
	{
		auto* entityManager = m_Engine.GetEntityManager();

		auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);

		if (newEntity == INVALID_ENTITY)
		{
			entityManager->ResizeEntityNmb(m_Configuration->currentEntitiesNmb + 1);
			newEntity = entityManager->CreateEntity(INVALID_ENTITY);
		}
		
		//add transform
		auto transformPtr = m_Transform2DManager->AddComponent(newEntity);
		transformPtr->Position = Vec2f(position.x, position.y);

		if (CheckEmptySlot(newEntity, buildingType))
		{
			return;
		}

		m_BuildingIndexCount++;

		if (m_BuildingIndexCount >= m_EntityIndex.size())
		{
			ResizeContainer(m_BuildingIndexCount + CONTAINER_RESERVATION);
		}

		const size_t newBuilding = m_BuildingIndexCount - 1;

		m_BuildingTypes[newBuilding] = buildingType;

		m_EntityIndex[newBuilding] = newEntity;

		const DwarfSlots newDwarfSlot;
		m_DwarfSlots[newBuilding] = newDwarfSlot;
		m_ResourcesInventories[newBuilding] = 0;
		m_ProgressionCoolDowns[newBuilding] = 0;

		switch (buildingType)
		{
		case BuildingType::MINE:
			m_ResourceTypes[newBuilding] = m_MineResourceType;
			break;
		case BuildingType::EXCAVATION_POST:
			m_ResourceTypes[newBuilding] = m_ExcavationPostResourceType;
			break;
		case BuildingType::MUSHROOM_FARM:
			m_ResourceTypes[newBuilding] = m_MushroomFarmResourceType;
			break;
		}

		AttributionTexture(newEntity, buildingType);
	}

	bool ProductionBuildingManager::DestroyBuilding(Entity entity, BuildingType buildingType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_EntityIndex[i] = INVALID_ENTITY;
				EntityManager* entityManager = m_Engine.GetEntityManager();
				entityManager->DestroyEntity(entity);
				return true;
			}
		}
		return false;
	}

	bool ProductionBuildingManager::AddDwarfToBuilding(Entity entity, BuildingType buildingType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity && m_BuildingTypes[i] == buildingType)
			{
				if (m_DwarfSlots[i].dwarfAttributed < m_DwarfSlots[i].maxDwarfCapacity)
				{
					m_DwarfSlots[i].dwarfAttributed++;
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}

	bool ProductionBuildingManager::RemoveDwarfToBuilding(Entity entity)
	{
		for (int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_DwarfSlots[i].dwarfAttributed--;
				return true;
			}
		}
		return false;
	}

	void ProductionBuildingManager::DwarfEnterBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_DwarfSlots[i].dwarfIn++;
				if (m_DwarfSlots[i].dwarfIn > m_DwarfSlots[i].maxDwarfCapacity)
					m_DwarfSlots[i].dwarfIn = m_DwarfSlots[i].maxDwarfCapacity;
			}
		}
	}


	void ProductionBuildingManager::DwarfExitBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_DwarfSlots[i].dwarfIn--;
				if (m_DwarfSlots[i].dwarfIn < 0)
					m_DwarfSlots[i].dwarfIn = 0;
			}
		}
	}

	Entity ProductionBuildingManager::GetFreeSlotInBuilding(BuildingType buildingType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_DwarfSlots[i].dwarfAttributed < m_DwarfSlots[i].maxDwarfCapacity && m_BuildingTypes[i] == buildingType)
			{
				return m_EntityIndex[i];
			}
		}
		return INVALID_ENTITY;
	}

	int ProductionBuildingManager::DwarfTakesResources(Entity entity, BuildingType buildingType)
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity && m_BuildingTypes[i] == buildingType)
			{
				m_ResourcesInventories[i] -= GetStackSizeByResourceType(m_ResourceTypes[i]);
				m_ReservedExportStackNumber[i]--;
				return GetStackSizeByResourceType(m_ResourceTypes[i]);
			}
		}
		return EMPTY_INVENTORY;
	}

	void ProductionBuildingManager::Produce()
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				continue;
			}

			m_ProgressionCoolDowns[i] += m_DwarfSlots[i].dwarfIn;

			if (m_ProgressionCoolDowns[i] == m_CoolDown)
			{
				m_ProgressionCoolDowns[i] = 0;
				m_ResourcesInventories[i]++;

				if(m_ResourcesInventories[i] >= m_ReservedExportStackNumber[i] * GetStackSizeByResourceType(m_ResourceTypes[i]) + GetStackSizeByResourceType(m_ResourceTypes[i]))
				{
					m_ReservedExportStackNumber[i]++;
					m_BuildingManager->RegistrationBuildingToBeEmptied(m_EntityIndex[i], m_BuildingTypes[i], m_ResourceTypes[i], 0);
				}

				if (m_ResourcesInventories[i] > m_MaxCapacity)
				{
					m_ResourcesInventories[i] = m_MaxCapacity;
				}
			}
		}
	}

	void ProductionBuildingManager::ResizeContainer(size_t newSize)
	{
		m_EntityIndex.resize(newSize, INVALID_ENTITY);
		m_DwarfSlots.resize(newSize);
		m_BuildingTypes.resize(newSize);
		m_ResourcesInventories.resize(newSize);
		m_ReservedExportStackNumber.resize(newSize);
		m_ProgressionCoolDowns.resize(newSize);
		m_ResourceTypes.resize(newSize);
	}

	bool ProductionBuildingManager::CheckEmptySlot(Entity newEntity, BuildingType buildingType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] != INVALID_ENTITY)
			{
				continue;
			}
			m_EntityIndex[i] = newEntity;
			const DwarfSlots newDwarfSlot;
			m_DwarfSlots[i] = newDwarfSlot;
			m_ResourcesInventories[i] = 0;
			m_BuildingTypes[i] = buildingType;

			switch (buildingType)
			{
			case BuildingType::MINE:
				m_ResourceTypes[i] = m_MineResourceType;
				break;
			case BuildingType::EXCAVATION_POST:
				m_ResourceTypes[i] = m_ExcavationPostResourceType;
				break;
			case BuildingType::MUSHROOM_FARM:
				m_ResourceTypes[i] = m_MushroomFarmResourceType;
				break;
			}

			AttributionTexture(newEntity, buildingType);


			return true;
		}
		return false;
	}

	void ProductionBuildingManager::AttributionTexture(Entity newEntity, BuildingType buildingType)
	{
		switch (buildingType)
		{
		case BuildingType::MINE:
			SetupTexture(newEntity, m_MineTexture);
			break;

		case BuildingType::EXCAVATION_POST:
			SetupTexture(newEntity, m_ExcavationPostTexture);
			break;

		case BuildingType::MUSHROOM_FARM:
			SetupTexture(newEntity, m_MushroomFarmTexture);
			break;
		}
	}

	void ProductionBuildingManager::SetupTexture(Entity newEntity, sf::Texture* texture)
	{
		// Sprite Component part
		Sprite* sprite = m_SpriteManager->AddComponent(newEntity);
		sprite->SetTexture(texture);
	}
}

