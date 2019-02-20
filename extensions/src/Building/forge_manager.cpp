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

#include <extensions/Building/forge_manager.h>
#include <extensions/Building/building_manager.h>


namespace sfge::ext
{
	ForgeManager::ForgeManager(Engine& engine) : System(engine) {}

	void ForgeManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
		m_BuildingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<BuildingManager>(
			"BuildingManager");

		m_Window = m_Engine.GetGraphics2dManager()->GetWindow();


		//Load Texture
		m_TexturePath = "data/sprites/forge.png";
		m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
		m_Texture = m_TextureManager->GetTexture(m_TextureId);

		m_VertexArray = sf::VertexArray(sf::Quads, 0);
	}

	void ForgeManager::Update(float dt)
	{
		(void) dt;
	}

	void ForgeManager::FixedUpdate()
	{
		ProduceTools();
	}

	void ForgeManager::Draw()
	{
		m_Window->draw(m_VertexArray, m_Texture);
	}

	void ForgeManager::SpawnBuilding(Vec2f pos)
	{
		auto* entityManager = m_Engine.GetEntityManager();


		auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);

		if(newEntity == INVALID_ENTITY)
		{
			Configuration* configuration = m_Engine.GetConfig();
			entityManager->ResizeEntityNmb(configuration->currentEntitiesNmb + 1);
			newEntity = entityManager->CreateEntity(INVALID_ENTITY);
		}

		//add transform
		auto transformPtr = m_Transform2DManager->AddComponent(newEntity);
		transformPtr->Position = Vec2f(pos.x, pos.y);

		if(CheckEmptySlot(newEntity, transformPtr))
		{
			return;
		}

		m_BuildingIndexCount++;


		if (m_BuildingIndexCount >= m_EntityIndex.size())
		{
			ResizeContainer(m_BuildingIndexCount + CONTAINER_EXTENDER);
		}


		const size_t newForge = m_BuildingIndexCount - 1;

		m_EntityIndex[newForge] = newEntity;

		SetupVertexArray(newForge, transformPtr);
	}

	bool ForgeManager::DestroyBuilding(Entity entity)
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

	bool ForgeManager::AddDwarfToBuilding(Entity entity)
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

	bool ForgeManager::RemoveDwarfToBuilding(Entity entity)
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

	void ForgeManager::DwarfEnterBuilding(Entity entity)
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

	void ForgeManager::DwarfExitBuilding(Entity entity)
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

	Entity ForgeManager::GetFreeSlotInBuilding()
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

	ResourceType ForgeManager::GetProducedResourceType()
	{
		return m_ResourceTypeProduced;
	}

	ResourceType ForgeManager::GetNeededResourceType()
	{
		return m_ResourceTypeNeeded;
	}

	void ForgeManager::DwarfTakesResources(Entity entity)
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_ResourcesInventoriesGiver[i] -= GetStackSizeByResourceType(m_ResourceTypeProduced);
				m_ReservedExportStackNumber[i]--;
			}
		}
	}

	void ForgeManager::DwarfPutsResources(Entity entity)
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_ResourcesInventoriesReceiver[i] += GetStackSizeByResourceType(m_ResourceTypeNeeded);
				m_ReservedImportStackNumber[i]--;
			}
		}
	}

	void ForgeManager::ResizeContainer(const size_t newSize)
	{
		m_EntityIndex.resize(newSize, INVALID_ENTITY);

		m_DwarfSlots.resize(newSize, DwarfSlots());

		m_ResourcesInventoriesGiver.resize(newSize, 0);
		m_ResourcesInventoriesReceiver.resize(newSize, 0);

		m_ProgressionConsumption.resize(newSize, 0);
		m_ProgressionCoolDown.resize(newSize, 0);

		m_ReservedExportStackNumber.resize(newSize, 0);
		m_ReservedImportStackNumber.resize(newSize, 0);

		m_VertexArray.resize(newSize * 4);
	}

	void ForgeManager::ProduceTools()
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == NULL || m_ResourcesInventoriesReceiver[i] <= 0 || m_ResourcesInventoriesGiver[i] >= m_MaxCapacityGiver)
			{
				continue;
			}

			if((m_ProgressionCoolDown[i] += m_DwarfSlots[i].dwarfIn) >= m_CoolDown)
			{
				m_ProgressionCoolDown[i] = 0;
				m_ResourcesInventoriesReceiver[i]--;

				if(m_ResourcesInventoriesReceiver[i] <= m_MaxCapacityReceiver - (m_ReservedImportStackNumber[i] * GetStackSizeByResourceType(ResourceType::IRON) + GetStackSizeByResourceType(ResourceType::IRON)))
				{
					// TODO : ADAPT WITHOUT THE RESOURCE QUANTITY
					m_BuildingManager->RegistrationBuildingToBeFill(m_EntityIndex[i], BuildingType::FORGE, m_ResourceTypeNeeded, 0);
				}

				m_ProgressionConsumption[i]++;

				if(m_ProgressionConsumption[i] >= m_ConsumptionGoal)
				{
					m_ProgressionConsumption[i] = 0;
					m_ResourcesInventoriesGiver[i]++;

					if(m_ResourcesInventoriesGiver[i] >= m_ReservedExportStackNumber[i] * GetStackSizeByResourceType(m_ResourceTypeProduced) + GetStackSizeByResourceType(m_ResourceTypeProduced))
					{
						// TODO : ADAPT WITHOUT THE RESOURCE QUANTITY FOR THE EXPORT
						m_BuildingManager->RegistrationBuildingToBeEmptied(m_EntityIndex[i], BuildingType::FORGE, m_ResourceTypeProduced, 0);
					}
				}
			}
		}
	}

	bool ForgeManager::CheckEmptySlot(Entity newEntity, Transform2d* transformPtr)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == NULL)
			{
				m_EntityIndex[i] = newEntity;
				m_DwarfSlots[i] = DwarfSlots();

				m_ResourcesInventoriesReceiver[i] = 0;
				m_ResourcesInventoriesGiver[i] = 0;

				m_ProgressionCoolDown[i] = 0;
				m_ProgressionConsumption[i] = 0;

				m_ReservedExportStackNumber[i] = 0;
				m_ReservedImportStackNumber[i] = 0;

				SetupVertexArray(i, transformPtr);
				return true;
			}
		}
		return false;
	}
	void ForgeManager::SetupVertexArray(unsigned int forgeIndex, Transform2d* transformPtr)
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
	void ForgeManager::ResetVertexArray(int forgeIndex)
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