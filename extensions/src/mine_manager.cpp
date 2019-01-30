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

#include <extensions/mine_manager.h>


namespace sfge::ext
{
	MineManager::MineManager(Engine& engine) : System(engine) {}

	void MineManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
		m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();

		EntityManager* entityManager = m_Engine.GetEntityManager();

		//Load Texture
		m_TexturePath = "data/sprites/building.png";
		m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
		m_Texture = m_TextureManager->GetTexture(m_TextureId);

		m_VertexArray = sf::VertexArray(sf::Quads, 0);
	}

	void MineManager::Update(float dt) {}

	void MineManager::FixedUpdate()
	{
		RessourcesProduction();

#ifdef TEST_SYSTEM_DEBUG
		m_FrameInProgress++;
		if(m_FrameInProgress >= m_FramesBeforeAdd && m_EntitiesNmb > m_EntitiesCount)
		{
			m_EntitiesCount++;
			AddNewBuilding(Vec2f(std::rand() % static_cast<int>(1280), std::rand() % static_cast<int>(720)));
			m_FrameInProgress = 0;
		}
#endif
	}

	void MineManager::Draw()
	{
		auto window = m_Engine.GetGraphics2dManager()->GetWindow();

		window->draw(m_VertexArray, m_Texture);
	}



	void MineManager::AddNewBuilding(Vec2f pos)
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
			const size_t newMine = m_EntityIndex.size();

			ResizeContainer(newMine + 1);
			m_IronInventory[newMine].resourceType = ResourceType::IRON;

			m_IronInventory[newMine].packSize = m_StackSize;

			m_EntityIndex[newMine] = newEntity;

			const sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

			m_VertexArray[4 * newMine].texCoords = sf::Vector2f(0, 0);
			m_VertexArray[4 * newMine + 1].texCoords = sf::Vector2f(textureSize.x, 0);
			m_VertexArray[4 * newMine + 2].texCoords = textureSize;
			m_VertexArray[4 * newMine + 3].texCoords = sf::Vector2f(0, textureSize.y);

			m_VertexArray[4 * newMine].position = transformPtr->Position - textureSize / 2.0f;
			m_VertexArray[4 * newMine + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
			m_VertexArray[4 * newMine + 2].position = transformPtr->Position + textureSize / 2.0f;
			m_VertexArray[4 * newMine + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);
		}
	}

	bool MineManager::DestroyBuilding(Entity entity)
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

	bool MineManager::AddDwarfToBuilding(Entity entity)
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

	bool MineManager::RemoveDwarfToBuilding(Entity entity)
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


	Entity MineManager::GetFreeSlotInBuilding()
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

	Entity MineManager::GetBuildingWithResources()
	{
		for (int i = 0; i < m_IronInventory.size(); i++)
		{
			if (m_IronInventory[i].packNumber > 0)
			{
				return m_EntityIndex[i];
			}
		}
		return INVALID_ENTITY;
	}

	ResourceType MineManager::GetResourceType()
	{
		return m_ResourceType;
	}

	int MineManager::GetResourcesBack(Entity entity)
	{
		for (unsigned int i = 0; i < m_EntityIndex.size(); i++)
		{
			if(m_EntityIndex[i] == entity)
			{
				m_IronInventory[i].packNumber--;
				return m_IronInventory[i].packSize;
			}
		}
		return 0;
	}

	void MineManager::RessourcesProduction()
	{
		for (unsigned int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				continue;
			}

			//Check if the inventory is full
			if (!(m_IronInventory[i].packNumber * m_IronInventory[i].packSize >= m_IronInventory[i].maxCapacity))
			{
				//Produce Iron by checking the number of dwarf in the building
				m_IronInventory[i].inventory += m_ProductionRate * m_DwarfSlots[i].dwarfIn;

				if (m_IronInventory[i].inventory >= m_IronInventory[i].packSize)
				{
					m_IronInventory[i].inventory -= m_IronInventory[i].packSize;
					m_IronInventory[i].packNumber++;
				}
			}

#ifdef DEBUG_CHECK_PRODUCTION

			std::cout << "Iron Inventory of mine " + std::to_string(i + 1) + " : " + std::to_string(m_IronInventory[i].inventory) +
				" / and pack Number : " + std::to_string(m_IronInventory[i].packNumber) + "\n";

			if (i + 1 == m_entitiesNmb)
			{
				std::cout << "\n";
			}
#endif
		}
	}

	void MineManager::ResizeContainer(size_t newSize)
	{
		m_EntityIndex.resize(newSize);
		m_DwarfSlots.resize(newSize);
		m_IronInventory.resize(newSize);
		m_VertexArray.resize(newSize * 4);
	}

	bool MineManager::CheckEmptySlot(Entity newEntity, Transform2d* transformPtr)
	{
		for (int i = 0; i < m_EntityIndex.size(); ++i)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				m_EntityIndex[i] = newEntity;
				const DwarfSlots newDwarfSlot;
				m_DwarfSlots[i] = newDwarfSlot;
				const GiverInventory newIronInventory;
				m_IronInventory[i] = newIronInventory;

				m_IronInventory[i].resourceType = ResourceType::IRON;

				m_IronInventory[i].packSize = m_StackSize;

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

