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
#include "extensions/Building/building_manager.h"


namespace sfge::ext
{

	ProductionBuildingManager::ProductionBuildingManager(Engine& engine) : System(engine) {}

	ProductionBuildingManager::~ProductionBuildingManager()
	{
		std::cout << "Production Building Manager \n";
	}

	void ProductionBuildingManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();

		window = m_Engine.GetGraphics2dManager()->GetWindow();

		EntityManager* entityManager = m_Engine.GetEntityManager();

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

		m_MineVertexArray = sf::VertexArray(sf::Quads, 0);
		m_ExcavationPostVertexArray = sf::VertexArray(sf::Quads, 0);
		m_MushroomFarmVertexArray = sf::VertexArray(sf::Quads, 0);
		std::cout << "production building Manager \n";

		m_Init = true;

		for (int i = 0; i < 100; i++)
		{
			AddNewBuilding(Vec2f(0, 0), BuildingType::EXCAVATION_POST);
		}
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
		window->draw(m_MineVertexArray, m_MineTexture);
		window->draw(m_ExcavationPostVertexArray, m_ExcavationPostTexture);
		window->draw(m_MushroomFarmVertexArray, m_MushroomFarmTexture);
	}



	void ProductionBuildingManager::AddNewBuilding(Vec2f position, BuildingType buildingType)
	{
		auto* entityManager = m_Engine.GetEntityManager();

		auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);

		if (newEntity == INVALID_ENTITY)
		{
			Configuration* configuration = m_Engine.GetConfig();
			entityManager->ResizeEntityNmb(configuration->currentEntitiesNmb + 1);
			newEntity = entityManager->CreateEntity(INVALID_ENTITY);
		}
		
		//add transform
		auto transformPtr = m_Transform2DManager->AddComponent(newEntity);
		transformPtr->Position = Vec2f(position.x, position.y);

		if (m_BuildingIndexCount >= m_EntityIndex.size())
		{
			ResizeContainer(m_BuildingIndexCount + CONTAINER_EXTENDER);
		}

		if (!CheckEmptySlot(newEntity, buildingType, transformPtr))
		{
			m_BuildingIndexCount++;

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

			AttributionVertxArray(newBuilding, buildingType, transformPtr);
		}
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

				

				switch (buildingType)
				{
				case BuildingType::MINE:
					ResetVertexArray(&m_MineVertexArray, i);
					break;

				case BuildingType::EXCAVATION_POST:
					ResetVertexArray(&m_ExcavationPostVertexArray, i);
					break;

				case BuildingType::MUSHROOM_FARM:
					ResetVertexArray(&m_MushroomFarmVertexArray, i);
					break;
				}
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

	Entity ProductionBuildingManager::GetBuildingWithResources(BuildingType buildingType)
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_ResourcesInventories[i] >= m_StackSize && m_BuildingTypes[i] == buildingType)
			{
				return m_EntityIndex[i];
			}
		}
		return INVALID_ENTITY;
	}

	ResourceType ProductionBuildingManager::GetProducedResourceType(BuildingType buildingType)
	{
		switch (buildingType)
		{
		case BuildingType::MINE:
			return m_MineResourceType;
			break;

		case BuildingType::EXCAVATION_POST:
			return m_ExcavationPostResourceType;
			break;

		case BuildingType::MUSHROOM_FARM:
			return m_MushroomFarmResourceType;
			break;

		}
	}

	int ProductionBuildingManager::DwarfTakeResources(Entity entity, BuildingType buildingType)
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity && m_BuildingTypes[i] == buildingType)
			{
				m_ResourcesInventories[i] -= m_StackSize;
				return m_StackSize;
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

			if ((m_ProgressionCoolDowns[i] += m_DwarfSlots[i].dwarfIn) == m_CoolDown)
			{
				m_ProgressionCoolDowns[i] = 0;
				m_ResourcesInventories[i]++;

				if(m_ResourcesInventories[i] >= m_ReservedInventoriesResources[i] + m_StackSize)
				{
					m_ReservedInventoriesResources[i]++;
					
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
		m_ReservedInventoriesResources.resize(newSize);
		m_ProgressionCoolDowns.resize(newSize);
		m_ResourceTypes.resize(newSize);
		m_GeneralVertexIndex.resize(newSize);
	}

	void ProductionBuildingManager::ResizeContainerForVertexArray(size_t newSize, BuildingType buildingType)
	{
		switch (buildingType)
		{
		case BuildingType::MINE:
			m_MineVertexArray.resize(newSize * 4);
			m_MineVertexIndex.resize(newSize);
			break;
		case BuildingType::EXCAVATION_POST:
			m_ExcavationPostVertexArray.resize(newSize * 4);
			m_ExcavationPostVertexIndex.resize(newSize);
			break;
		case BuildingType::MUSHROOM_FARM:
			m_MushroomFarmVertexArray.resize(newSize * 4);
			m_MushroomFarmVertexIndex.resize(newSize);
			break;
		}
	}

	bool ProductionBuildingManager::CheckEmptySlot(Entity newEntity, BuildingType buildingType, Transform2d* transformPtr)
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

			AttributionVertxArray(i, buildingType, transformPtr);


			return true;
		}
		return false;
	}

	void ProductionBuildingManager::AttributionVertxArray(unsigned int entityIndex, BuildingType buildingType, Transform2d * transformPtr)
	{
		switch (buildingType)
		{
		case BuildingType::MINE:
			for (unsigned int i = 0; i < m_MineCount; i++)
			{
				if (!m_MineVertexIndex[i])
				{
					m_MineVertexIndex[i] = true;
					SetupVertexArray(i, &m_MineVertexArray, m_MineTexture, transformPtr);
					m_GeneralVertexIndex[entityIndex] = i;
					return;
				}
			}

			ResizeContainerForVertexArray(m_MineCount + CONTAINER_EXTENDER, buildingType);
			m_MineCount++;
			m_MineVertexIndex[m_MineCount - 1] = true;
			SetupVertexArray(m_MineCount - 1, &m_MineVertexArray, m_MineTexture, transformPtr);
			m_GeneralVertexIndex[entityIndex] = m_MineCount - 1;
			break;

		case BuildingType::EXCAVATION_POST:
			for (unsigned int i = 0; i < m_ExcavationPostCount; i++)
			{
				if (!m_ExcavationPostVertexIndex[i])
				{
					m_ExcavationPostVertexIndex[i] = true;
					SetupVertexArray(i, &m_ExcavationPostVertexArray, m_ExcavationPostTexture, transformPtr);
					m_GeneralVertexIndex[entityIndex] = i;
					return;
				}
			}

			ResizeContainerForVertexArray(m_ExcavationPostCount + CONTAINER_EXTENDER, buildingType);
			m_ExcavationPostCount++;
			m_ExcavationPostVertexIndex[m_ExcavationPostCount - 1] = true;
			SetupVertexArray(m_ExcavationPostCount - 1, &m_ExcavationPostVertexArray, m_ExcavationPostTexture, transformPtr);
			m_GeneralVertexIndex[entityIndex] = m_ExcavationPostCount - 1;
			break;

		case BuildingType::MUSHROOM_FARM:
			for (unsigned int i = 0; i < m_MushroomFarmCount; i++)
			{
				if (!m_MushroomFarmVertexIndex[i])
				{
					m_MushroomFarmVertexIndex[i] = true;
					SetupVertexArray(i, &m_MushroomFarmVertexArray, m_MushroomFarmTexture, transformPtr);
					m_GeneralVertexIndex[entityIndex] = i;
					return;
				}
			}

			ResizeContainerForVertexArray(m_MushroomFarmCount + CONTAINER_EXTENDER, buildingType);
			m_MushroomFarmCount++;
			m_MushroomFarmVertexIndex[m_MushroomFarmCount - 1] = true;
			SetupVertexArray(m_MushroomFarmCount - 1, &m_MushroomFarmVertexArray, m_MushroomFarmTexture, transformPtr);
			m_GeneralVertexIndex[entityIndex] = m_MushroomFarmCount - 1;
			break;
		}
	}

	void ProductionBuildingManager::SetupVertexArray(int index, sf::VertexArray* vertexArray, sf::Texture* texture, Transform2d * transformPtr)
	{
		const sf::Vector2f textureSize = sf::Vector2f(texture->getSize().x, texture->getSize().y);

		(*vertexArray)[4 * index].texCoords = sf::Vector2f(0, 0);
		(*vertexArray)[4 * index + 1].texCoords = sf::Vector2f(textureSize.x, 0);
		(*vertexArray)[4 * index + 2].texCoords = textureSize;
		(*vertexArray)[4 * index + 3].texCoords = sf::Vector2f(0, textureSize.y);

		(*vertexArray)[4 * index].position = transformPtr->Position - textureSize / 2.0f;
		(*vertexArray)[4 * index + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
		(*vertexArray)[4 * index + 2].position = transformPtr->Position + textureSize / 2.0f;
		(*vertexArray)[4 * index + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);
	}

	void ProductionBuildingManager::ResetVertexArray(sf::VertexArray * vertexArray, int index)
	{
		sf::Vector2f resetVertexArray = sf::Vector2f(0, 0);

		(*vertexArray)[4 * index].texCoords = resetVertexArray;
		(*vertexArray)[4 * index + 1].texCoords = resetVertexArray;
		(*vertexArray)[4 * index + 2].texCoords = resetVertexArray;
		(*vertexArray)[4 * index + 3].texCoords = resetVertexArray;

		(*vertexArray)[4 * index].position = resetVertexArray;
		(*vertexArray)[4 * index + 1].position = resetVertexArray;
		(*vertexArray)[4 * index + 2].position = resetVertexArray;
		(*vertexArray)[4 * index + 3].position = resetVertexArray;
	}
}

