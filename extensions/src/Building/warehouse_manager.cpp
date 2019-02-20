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

#include <extensions/Building/warehouse_manager.h>

namespace sfge::ext
{
	WarehouseManager::WarehouseManager(Engine& engine) : System(engine){}

	void WarehouseManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();

		m_Window = m_Engine.GetGraphics2dManager()->GetWindow();

		//Load Texture
		m_TexturePath = "data/sprites/warehouse.png";
		m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
		m_Texture = m_TextureManager->GetTexture(m_TextureId);

		m_VertexArray = sf::VertexArray(sf::Quads, 0);
	}

	void WarehouseManager::Update(float dt)
	{
	}

	void WarehouseManager::FixedUpdate()
	{
	}

	void WarehouseManager::AddNewBuilding(Vec2f position)
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

		if (CheckEmptySlot(newEntity, transformPtr))
		{
			return;
		}

		m_BuildingIndexCount++;


		if (m_BuildingIndexCount >= m_EntityIndex.size())
		{
			ResizeContainer(m_BuildingIndexCount + CONTAINER_EXTENDER);
		}


		const size_t newWarehouse = m_BuildingIndexCount - 1;

		m_EntityIndex[newWarehouse] = newEntity;

		SetupVertexArray(newWarehouse, transformPtr);
	}

	bool WarehouseManager::DestroyBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_EntityIndex[i] = INVALID_ENTITY;
				EntityManager* entityManager = m_Engine.GetEntityManager();
				entityManager->DestroyEntity(entity);

				ResetVertexArray(i);
				return true;
			}
		}
		return false;
	}

	bool WarehouseManager::AddDwarfToBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
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

	bool WarehouseManager::RemoveDwarfToBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_DwarfSlots[i].dwarfAttributed--;
				return true;
			}
		}
		return false;
	}

	void WarehouseManager::DwarfEnterBuilding(Entity entity)
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

	void WarehouseManager::DwarfExitBuilding(Entity entity)
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

	Entity WarehouseManager::GetFreeSlotInBuilding()
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_DwarfSlots[i].dwarfAttributed < m_DwarfSlots[i].maxDwarfCapacity)
			{
				return m_EntityIndex[i];
			}
		}
		return INVALID_ENTITY;
	}

	Entity WarehouseManager::GetWarehouseWithResources(ResourceType resourceType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				continue;
			}

			short currentinventory;

			switch (resourceType)
			{
			case ResourceType::IRON:
				currentinventory = m_IronInventories[i] - m_ReservedExportStackNumberIron[i] * GetStackSizeByResourceType(resourceType);

				if (GetStackSizeByResourceType(resourceType) <= currentinventory)
				{
					return m_EntityIndex[i];
				}
				break;

			case ResourceType::FOOD:
				currentinventory = m_MushroomInventories[i] - m_ReservedExportStackNumberMushroom[i] * GetStackSizeByResourceType(resourceType);

				if (GetStackSizeByResourceType(resourceType) <= currentinventory)
				{
					return m_EntityIndex[i];
				}
				break;


			case ResourceType::STONE:
				currentinventory = m_StoneInventories[i] - m_ReservedExportStackNumberStone[i] * GetStackSizeByResourceType(resourceType);

				if (GetStackSizeByResourceType(resourceType) <= currentinventory)
				{
					return m_EntityIndex[i];
				}
				break;


			case ResourceType::TOOL:
				currentinventory = m_ToolInventories[i] - m_ReservedExportStackNumberTool[i] * GetStackSizeByResourceType(resourceType);

				if (GetStackSizeByResourceType(resourceType) <= currentinventory)
				{
					return m_EntityIndex[i];
				}
				break;
			}
		}
		return INVALID_ENTITY;
	}

	Entity WarehouseManager::GetWarehouseWithFreeSpaceAvailable(ResourceType resourceType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				continue;
			}

			unsigned short currentinventory;

			switch (resourceType)
			{
			case ResourceType::IRON:
				currentinventory = m_IronInventories[i] + m_ReservedImportStackNumberIron[i] * GetStackSizeByResourceType(resourceType);

				if(GetStackSizeByResourceType(resourceType) <= m_MaxCapacity - currentinventory)
				{
					return m_EntityIndex[i];
				}
				break;

			case ResourceType::FOOD:
				currentinventory = m_MushroomInventories[i] + m_ReservedImportStackNumberMushroom[i] * GetStackSizeByResourceType(resourceType);

				if (GetStackSizeByResourceType(resourceType) <= m_MaxCapacity - currentinventory)
				{
					return m_EntityIndex[i];
				}
				break;


			case ResourceType::STONE:
				currentinventory = m_StoneInventories[i] + m_ReservedImportStackNumberStone[i] * GetStackSizeByResourceType(resourceType);

				if (GetStackSizeByResourceType(resourceType) <= m_MaxCapacity - currentinventory)
				{
					return m_EntityIndex[i];
				}
				break;


			case ResourceType::TOOL:
				currentinventory = m_ToolInventories[i] + m_ReservedImportStackNumberTool[i] * GetStackSizeByResourceType(resourceType);

				if (GetStackSizeByResourceType(resourceType) <= m_MaxCapacity - currentinventory)
				{
					return m_EntityIndex[i];
				}
				break;
			}
		}
		return INVALID_ENTITY;
	}

	void WarehouseManager::DwarfTakesResources(Entity entity, ResourceType resourceType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if(m_EntityIndex[i] == entity)
			{
				switch(resourceType)
				{
				case ResourceType::IRON:
					m_IronInventories[i] -= GetStackSizeByResourceType(resourceType);
					m_ReservedExportStackNumberIron[i]--;
					break;

				case ResourceType::FOOD:
					m_MushroomInventories[i] -= GetStackSizeByResourceType(resourceType);
					m_ReservedExportStackNumberMushroom[i]--;
					break;

				case ResourceType::STONE:
					m_StoneInventories[i] -= GetStackSizeByResourceType(resourceType);
					m_ReservedExportStackNumberStone[i]--;
					break;

				case ResourceType::TOOL:
					m_ToolInventories[i] -= GetStackSizeByResourceType(resourceType);
					m_ReservedExportStackNumberTool[i]--;
					break;
				}
			}
		}
	}

	void WarehouseManager::DwarfPutsResources(Entity entity, ResourceType resourceType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				switch (resourceType)
				{
				case ResourceType::IRON:
					m_IronInventories[i] += GetStackSizeByResourceType(resourceType);
					m_ReservedImportStackNumberIron[i]--;
					break;

				case ResourceType::FOOD:
					m_MushroomInventories[i] += GetStackSizeByResourceType(resourceType);
					m_ReservedImportStackNumberMushroom[i]--;
					break;

				case ResourceType::STONE:
					m_StoneInventories[i] += GetStackSizeByResourceType(resourceType);
					m_ReservedImportStackNumberStone[i]--;
					break;

				case ResourceType::TOOL:
					m_ToolInventories[i] += GetStackSizeByResourceType(resourceType);
					m_ReservedImportStackNumberTool[i]--;
					break;
				}
			}
		}
	}
	void WarehouseManager::ReserveFill(Entity entity, ResourceType resourceType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				switch (resourceType)
				{
				case ResourceType::IRON:
					m_ReservedImportStackNumberIron[i]++;
					break;

				case ResourceType::FOOD:
					m_ReservedImportStackNumberMushroom[i]++;
					break;

				case ResourceType::STONE:
					m_ReservedImportStackNumberStone[i]++;
					break;

				case ResourceType::TOOL:
					m_ReservedImportStackNumberTool[i]++;
					break;
				}
			}
		}
	}
	void WarehouseManager::ReserveEmpty(Entity entity, ResourceType resourceType)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				switch (resourceType)
				{
				case ResourceType::IRON:
					m_ReservedExportStackNumberIron[i]++;
					break;

				case ResourceType::FOOD:
					m_ReservedExportStackNumberMushroom[i]++;
					break;

				case ResourceType::STONE:
					m_ReservedExportStackNumberStone[i]++;
					break;

				case ResourceType::TOOL:
					m_ReservedExportStackNumberTool[i]++;
					break;
				}
			}
		}
	}
	void WarehouseManager::ResizeContainer(const size_t newSize)
	{
		m_EntityIndex.resize(newSize, INVALID_ENTITY);
		m_DwarfSlots.resize(newSize, DwarfSlots());

		m_IronInventories.resize(newSize, 0);
		m_StoneInventories.resize(newSize, 0);
		m_ToolInventories.resize(newSize, 0);
		m_MushroomInventories.resize(newSize, 0);

		m_ReservedExportStackNumberIron.resize(newSize, 0);
		m_ReservedExportStackNumberStone.resize(newSize, 0);
		m_ReservedExportStackNumberTool.resize(newSize, 0);
		m_ReservedExportStackNumberMushroom.resize(newSize, 0);

		m_ReservedImportStackNumberIron.resize(newSize, 0);
		m_ReservedImportStackNumberStone.resize(newSize, 0);
		m_ReservedImportStackNumberTool.resize(newSize, 0);
		m_ReservedImportStackNumberMushroom.resize(newSize, 0);

	}
	bool WarehouseManager::CheckEmptySlot(Entity newEntity, Transform2d * transformPtr)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == NULL)
			{
				m_EntityIndex[i] = newEntity;
				m_DwarfSlots[i] = DwarfSlots();

				m_IronInventories[i] = 0;
				m_StoneInventories[i] = 0;
				m_ToolInventories[i] = 0;
				m_MushroomInventories[i] = 0;

				m_ReservedExportStackNumberIron[i] = 0;
				m_ReservedExportStackNumberStone[i] = 0;
				m_ReservedExportStackNumberTool[i] = 0;
				m_ReservedExportStackNumberMushroom[i] = 0;

				m_ReservedImportStackNumberIron[i] = 0;
				m_ReservedImportStackNumberStone[i] = 0;
				m_ReservedImportStackNumberTool[i] = 0;
				m_ReservedImportStackNumberMushroom[i] = 0;

				SetupVertexArray(i, transformPtr);
				return true;
			}
		}
		return false;
	}
	void WarehouseManager::SetupVertexArray(unsigned int forgeIndex, Transform2d * transformPtr)
	{
		const sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

		m_VertexArray[4 * forgeIndex].texCoords = sf::Vector2f(0, 0);
		m_VertexArray[4 * forgeIndex + 1].texCoords = sf::Vector2f(textureSize.x, 0);
		m_VertexArray[4 * forgeIndex + 2].texCoords = textureSize;
		m_VertexArray[4 * forgeIndex + 3].texCoords = sf::Vector2f(0, textureSize.y);

		m_VertexArray[4 * forgeIndex].position = transformPtr->Position - textureSize / 2.0f;
		m_VertexArray[4 * forgeIndex + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
		m_VertexArray[4 * forgeIndex + 2].position = transformPtr->Position + textureSize / 2.0f;
		m_VertexArray[4 * forgeIndex + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);
	}

	void WarehouseManager::ResetVertexArray(int forgeIndex)
	{
		sf::Vector2f resetSize = sf::Vector2f(0, 0);

		m_VertexArray[4 * forgeIndex].texCoords = resetSize;
		m_VertexArray[4 * forgeIndex + 1].texCoords = resetSize;
		m_VertexArray[4 * forgeIndex + 2].texCoords = resetSize;
		m_VertexArray[4 * forgeIndex + 3].texCoords = resetSize;

		m_VertexArray[4 * forgeIndex].position = resetSize;
		m_VertexArray[4 * forgeIndex + 1].position = resetSize;
		m_VertexArray[4 * forgeIndex + 2].position = resetSize;
		m_VertexArray[4 * forgeIndex + 3].position = resetSize;

	}
}