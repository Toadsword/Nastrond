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

#include <extensions/excavation_post_manager.h>

namespace sfge::ext
{
	ExcavationPostManager::ExcavationPostManager(Engine& engine) : System(engine) {}

	void ExcavationPostManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();

		EntityManager* entityManager = m_Engine.GetEntityManager();

		//Load Texture
		m_TexturePath = "data/sprites/building.png";
		m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
		m_Texture = m_TextureManager->GetTexture(m_TextureId);

		m_VertexArray = sf::VertexArray(sf::Quads, 0);
	}

	void ExcavationPostManager::Update(float dt) {}

	void ExcavationPostManager::FixedUpdate()
	{
		Produce();
	}

	void ExcavationPostManager::Draw()
	{
		auto window = m_Engine.GetGraphics2dManager()->GetWindow();

		window->draw(m_VertexArray, m_Texture);
	}

	void ExcavationPostManager::AddNewBuilding(Vec2f position)
	{
		auto* entityManager = m_Engine.GetEntityManager();

		Configuration* configuration = m_Engine.GetConfig();
		entityManager->ResizeEntityNmb(configuration->currentEntitiesNmb + 1);

		const auto newEntity = entityManager->CreateEntity(0);

		//add transform
		auto transformPtr = m_Transform2DManager->AddComponent(newEntity);
		transformPtr->Position = Vec2f(position.x, position.y);

		if (!CheckEmptySlot(newEntity, transformPtr))
		{
			const size_t newExcavationPost = m_EntityIndex.size();

			ResizeContainer(newExcavationPost + 1);
			m_StoneInventories[newExcavationPost].resourceType = ResourceType::IRON;

			m_StoneInventories[newExcavationPost].packSize = m_StackSize;

			m_EntityIndex[newExcavationPost] = newEntity;

			const sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

			m_VertexArray[4 * newExcavationPost].texCoords = sf::Vector2f(0, 0);
			m_VertexArray[4 * newExcavationPost + 1].texCoords = sf::Vector2f(textureSize.x, 0);
			m_VertexArray[4 * newExcavationPost + 2].texCoords = textureSize;
			m_VertexArray[4 * newExcavationPost + 3].texCoords = sf::Vector2f(0, textureSize.y);

			m_VertexArray[4 * newExcavationPost].position = transformPtr->Position - textureSize / 2.0f;
			m_VertexArray[4 * newExcavationPost + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
			m_VertexArray[4 * newExcavationPost + 2].position = transformPtr->Position + textureSize / 2.0f;
			m_VertexArray[4 * newExcavationPost + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);
		}

	}

	bool ExcavationPostManager::DestroyBuilding(Entity entity)
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

	bool ExcavationPostManager::AddDwarfToBuilding(Entity entity)
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

	bool ExcavationPostManager::RemoveDwarfToBuilding(Entity entity)
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

	void ExcavationPostManager::DwarfEnterBuilding(Entity entity)
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

	void ExcavationPostManager::DwarfExitBuilding(Entity entity)
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

	Entity ExcavationPostManager::GetFreeSlotInBuilding()
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

	Entity ExcavationPostManager::GetBuildingWithResources()
	{
		for (int i = 0; i < m_StoneInventories.size(); i++)
		{
			if (m_StoneInventories[i].packNumber > 0)
			{
				return m_EntityIndex[i];
			}
		}
		return INVALID_ENTITY;
	}

	ResourceType ExcavationPostManager::GetProducedResourceType()
	{
		return m_ResourceType;
	}

	int ExcavationPostManager::GetResourcesBack(Entity entity)
	{
		for (unsigned int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_StoneInventories[i].packNumber--;
				return m_StoneInventories[i].packSize;
			}
		}
		return EMPTY_INVENTORY;
	}

	void ExcavationPostManager::ResizeContainer(const size_t newSize)
	{
		m_DwarfSlots.resize(newSize);
		m_EntityIndex.resize(newSize, INVALID_ENTITY);
		m_StoneInventories.resize(newSize);
		m_VertexArray.resize(newSize * 4);
	}

	void ExcavationPostManager::Produce()
	{
		for (unsigned int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				continue;
			}

			//Check if the inventory is full
			if (!(m_StoneInventories[i].packNumber * m_StoneInventories[i].packSize >= m_StoneInventories[i].maxCapacity))
			{
				//Produce Stone by checking the number of dwarf in the building
				m_StoneInventories[i].inventory += m_ProductionRate * m_DwarfSlots[i].dwarfIn;

				if (m_StoneInventories[i].inventory >= m_StoneInventories[i].packSize)
				{
					m_StoneInventories[i].inventory -= m_StoneInventories[i].packSize;
					m_StoneInventories[i].packNumber++;
				}
			}
		}
	}

	bool ExcavationPostManager::CheckEmptySlot(Entity newEntity, Transform2d * transformPtr)
	{
		for (int i = 0; i < m_EntityIndex.size(); ++i)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				m_EntityIndex[i] = newEntity;
				const DwarfSlots newDwarfSlot;
				m_DwarfSlots[i] = newDwarfSlot;
				const GiverInventory newIronInventory;
				m_StoneInventories[i] = newIronInventory;

				m_StoneInventories[i].resourceType = ResourceType::STONE;

				m_StoneInventories[i].packSize = m_StackSize;

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