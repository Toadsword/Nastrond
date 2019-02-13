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

#include <extensions/forge_manager.h>


namespace sfge::ext
{
	ForgeManager::ForgeManager(Engine& engine) : System(engine) {}

	void ForgeManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();

		//Load Texture
		m_TexturePath = "data/sprites/building.png";
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
		auto window = m_Engine.GetGraphics2dManager()->GetWindow();

		window->draw(m_VertexArray, m_Texture);
	}

	void ForgeManager::AddNewBuilding(Vec2f pos)
	{
		auto* entityManager = m_Engine.GetEntityManager();

		Configuration* configuration = m_Engine.GetConfig();
		entityManager->ResizeEntityNmb(configuration->currentEntitiesNmb + 1);

		const auto newEntity = entityManager->CreateEntity(0);

		//add transform
		auto transformPtr = m_Transform2DManager->AddComponent(newEntity);
		transformPtr->Position = Vec2f(pos.x, pos.y);

		if (!CheckEmptySlot(newEntity, transformPtr))
		{
			size_t newForge = m_EntityIndex.size();

			ResizeContainer(newForge + CONTAINER_EXTENDER);

			m_IronsInventories[newForge].resourceType = ResourceType::IRON;
			m_ToolsInventories[newForge].resourceType = ResourceType::TOOL;
			m_ProgressionProdTool[newForge].resourceType = ResourceType::TOOL;

			m_EntityIndex[newForge] = newEntity;

			const sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

			m_VertexArray[4 * newForge].texCoords = sf::Vector2f(0, 0);
			m_VertexArray[4 * newForge + 1].texCoords = sf::Vector2f(textureSize.x, 0);
			m_VertexArray[4 * newForge + 2].texCoords = textureSize;
			m_VertexArray[4 * newForge + 3].texCoords = sf::Vector2f(0, textureSize.y);

			m_VertexArray[4 * newForge].position = transformPtr->Position - textureSize / 2.0f;
			m_VertexArray[4 * newForge + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
			m_VertexArray[4 * newForge + 2].position = transformPtr->Position + textureSize / 2.0f;
			m_VertexArray[4 * newForge + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);

		}
	}

	bool ForgeManager::DestroyBuilding(Entity entity)
	{
		for (int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_EntityIndex[i] = INVALID_ENTITY;
				EntityManager* entityManager = m_Engine.GetEntityManager();
				entityManager->DestroyEntity(entity);

				sf::Vector2f resetSize = sf::Vector2f(0, 0);
				m_VertexArray[4 * i].texCoords = resetSize;
				m_VertexArray[4 * i + 1].texCoords = resetSize;
				m_VertexArray[4 * i + 2].texCoords = resetSize;
				m_VertexArray[4 * i + 3].texCoords = resetSize;
				return true;
			}
		}
		return false;
	}

	bool ForgeManager::AddDwarfToBuilding(Entity entity)
	{
		for (int i = 0; i < m_EntityIndex.size(); i++)
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

	void ForgeManager::DwarfEnterBuilding(Entity entity)
	{
		for (int i = 0; i < m_EntityIndex.size(); i++)
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
		for (int i = 0; i < m_EntityIndex.size(); i++)
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
		for (int i = 0; i < m_DwarfSlots.size(); i++)
		{
			if (m_DwarfSlots[i].dwarfAttributed < m_DwarfSlots[i].maxDwarfCapacity)
			{
				return m_EntityIndex[i];
			}
		}
		return INVALID_ENTITY;
	}

	Entity ForgeManager::GetBuildingWithResources()
	{
		for (int i = 0; i < m_ToolsInventories.size(); i++)
		{
			if (m_ToolsInventories[i].packNumber > 0)
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

	unsigned char ForgeManager::GetAmountOfResources()
	{
		return m_StackSize;
	}

	Entity ForgeManager::GetBuildingWithNeed()
	{
		return Entity();
	}

	int ForgeManager::GetResourcesBack(Entity entity)
	{
		for (unsigned int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_ToolsInventories[i].packNumber--;
				return m_ToolsInventories[i].packSize;
			}
		}
		return EMPTY_INVENTORY;
	}

	float ForgeManager::GiveResources(Entity entity, int nmbResources, ResourceType resourceType)
	{
		for (unsigned int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				if(m_IronsInventories[i].resourceType == resourceType)
				{
					m_IronsInventories[i].inventory += nmbResources;
					float tmpResourcesExcess = m_IronsInventories[i].inventory - m_IronsInventories[i].maxCapacity;

					if(tmpResourcesExcess > 0)
					{
						m_IronsInventories[i].inventory -= tmpResourcesExcess;
						return tmpResourcesExcess;
					}
				}
				else
				{
					return nmbResources;
				}
				
			}
		}
		return nmbResources;
	}

	void ForgeManager::ResizeContainer(const size_t newSize)
	{
		m_DwarfSlots.resize(newSize);
		m_EntityIndex.resize(newSize, INVALID_ENTITY);
		m_ToolsInventories.resize(newSize);
		m_IronsInventories.resize(newSize);
		m_ProgressionProdTool.resize(newSize);
		m_VertexArray.resize(newSize * 4);
	}

	void ForgeManager::ProduceTools()
	{
		for (int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == NULL)
			{
				continue;
			}

			if (m_IronsInventories[i].inventory <= 0 || m_ToolsInventories[i].inventory + m_ToolsInventories[i].packNumber * m_ToolsInventories[i].packSize >= m_ToolsInventories[i].maxCapacity)
			{
				continue;
			}

			m_ProgressionProdTool[i].FrameCoolDown += m_DwarfSlots[i].dwarfIn;

			if (m_ProgressionProdTool[i].FrameCoolDown >= m_CoolDownFrames)
			{
				m_IronsInventories[i].inventory--;
				m_ProgressionProdTool[i].progression++;
				m_ProgressionProdTool[i].FrameCoolDown = 0;
			}

			if (m_ProgressionProdTool[i].progression >= m_ProgressionProdTool[i].goal)
			{
				m_ToolsInventories[i].inventory++;
				m_ProgressionProdTool[i].progression = 0;

				if (m_ToolsInventories[i].inventory >= m_ToolsInventories[i].packSize)
				{
					m_ToolsInventories[i].packNumber++;
					m_ToolsInventories[i].inventory -= m_ToolsInventories[i].packSize;
				}
			}
		}
	}

	bool ForgeManager::CheckEmptySlot(Entity newEntity, Transform2d* transformPtr)
	{
		for (int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == NULL)
			{
				m_EntityIndex[i] = newEntity;
				const DwarfSlots newDwarfSlot;
				m_DwarfSlots[i] = newDwarfSlot;
				const ReceiverInventory newIronInventory;
				m_IronsInventories[i] = newIronInventory;
				const GiverInventory newToolInventory;
				m_ToolsInventories[i] = newToolInventory;
				const ProgressionProduction newProgressionProdTool;
				m_ProgressionProdTool[i] = newProgressionProdTool;

				m_IronsInventories[i].resourceType = ResourceType::IRON;
				m_ToolsInventories[i].resourceType = ResourceType::TOOL;
				m_ProgressionProdTool[i].resourceType = ResourceType::TOOL;

				m_ToolsInventories[i].packSize = m_StackSize;

				const sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

				m_VertexArray[4 * i].texCoords = sf::Vector2f(0, 0);
				m_VertexArray[4 * i + 1].texCoords = sf::Vector2f(textureSize.x, 0);
				m_VertexArray[4 * i + 2].texCoords = textureSize;
				m_VertexArray[4 * i + 3].texCoords = sf::Vector2f(0, textureSize.y);

				m_VertexArray[4 * i].position = transformPtr->Position - textureSize / 2.0f;
				m_VertexArray[4 * i + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
				m_VertexArray[4 * i + 2].position = transformPtr->Position + textureSize / 2.0f;
				m_VertexArray[4 * i + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);

				return true;
			}
		}
		return false;
	}
}