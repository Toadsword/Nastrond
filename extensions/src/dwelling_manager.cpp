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

#include <extensions/dwelling_manager.h>

namespace sfge::ext
{
	DwellingManager::DwellingManager(Engine & engine) : System(engine) {}

	void DwellingManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
		m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();

		Configuration* configuration = m_Engine.GetConfig();
		Vec2f screenSize = sf::Vector2f(configuration->screenResolution.x, configuration->screenResolution.y);

		//Load texture
		m_TexturePath = "data/sprites/building.png";
		m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
		m_Texture = m_TextureManager->GetTexture(m_TextureId);

		m_VertexArray = sf::VertexArray(sf::Quads, 0);

		std::cout << "Dwelling Manager \n";
	}

	void DwellingManager::Update(float dt)
	{
	}

	void DwellingManager::FixedUpdate()
	{
		Consume();
	}

	void DwellingManager::Draw()
	{
		auto window = m_Engine.GetGraphics2dManager()->GetWindow();

		window->draw(m_VertexArray, m_Texture);
	}

	void DwellingManager::AddNewBuilding(Vec2f pos)
	{
		auto* entityManager = m_Engine.GetEntityManager();
		Configuration* configuration = m_Engine.GetConfig();
		entityManager->ResizeEntityNmb(configuration->currentEntitiesNmb + 1);

		const auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);
		
		//add transform
		auto* transformPtr = m_Transform2DManager->AddComponent(newEntity);
		transformPtr->Position = Vec2f(pos.x, pos.y);

		if (!CheckEmptySlot(newEntity, transformPtr))
		{
			const size_t newDwelling = m_EntityIndex.size();

			ResizeContainer(newDwelling + CONTAINER_EXTENDER);
			m_FoodInventories[newDwelling].resourceType = ResourceType::FOOD;
			m_CoolDownFramesProgression[newDwelling] = 0u;

			m_EntityIndex[newDwelling] = newEntity;

			const sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

			m_VertexArray[4 * newDwelling].texCoords = sf::Vector2f(0, 0);
			m_VertexArray[4 * newDwelling + 1].texCoords = sf::Vector2f(textureSize.x, 0);
			m_VertexArray[4 * newDwelling + 2].texCoords = textureSize;
			m_VertexArray[4 * newDwelling + 3].texCoords = sf::Vector2f(0, textureSize.y);

			m_VertexArray[4 * newDwelling].position = transformPtr->Position - textureSize / 2.0f;
			m_VertexArray[4 * newDwelling + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
			m_VertexArray[4 * newDwelling + 2].position = transformPtr->Position + textureSize / 2.0f;
			m_VertexArray[4 * newDwelling + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);
		}
	}

	bool DwellingManager::AddDwarfToBuilding(Entity dwellingEntity)
	{
		for (int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == dwellingEntity)
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

	bool DwellingManager::RemoveDwarfToBuilding(Entity entity)
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

	Entity DwellingManager::GetFreeSlotInBuilding()
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

	void DwellingManager::DwarfEnterBuilding(Entity entity)
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

	void DwellingManager::DwarfExitBuilding(Entity entity)
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

	std::vector<ResourceType> DwellingManager::GetNeededResourceType()
	{
		std::vector<ResourceType> resourceTypes;
		resourceTypes.push_back(m_ResourceTypeNeeded);
		return resourceTypes;
	}

	float DwellingManager::GiveResources(Entity entity, int nmbResources, ResourceType resourceType)
	{
		for (unsigned int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				if (m_FoodInventories[i].resourceType == resourceType)
				{
					m_FoodInventories[i].inventory += nmbResources;
					float resourcesExcess = m_FoodInventories[i].inventory - m_FoodInventories[i].maxCapacity;

					if (resourcesExcess > 0)
					{
						m_FoodInventories[i].inventory -= resourcesExcess;
						return resourcesExcess;
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

	void DwellingManager::Consume()
	{
		for (unsigned int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				continue;
			}

			if (m_FoodInventories[i].inventory <= 0)
			{
				DecreaseHappiness();
				continue;
			}
			else if(m_DwarfSlots[i].dwarfIn <= 0)
			{
				continue;
			}

			m_CoolDownFramesProgression[i]++;

			if(m_CoolDownFramesProgression[i] >= m_CoolDownFrames)
			{
				m_FoodInventories[i].inventory -= m_DwarfSlots[i].dwarfIn;
				if(m_FoodInventories[i].inventory < 0)
				{
					m_FoodInventories[i].inventory = 0;
					DecreaseHappiness();
				}
			}
		}
	}

	void DwellingManager::ResizeContainer(const size_t newSize)
	{
		m_EntityIndex.resize(newSize, INVALID_ENTITY);
		m_DwarfSlots.resize(newSize);
		m_FoodInventories.resize(newSize);
		m_CoolDownFramesProgression.resize(newSize);
		m_VertexArray.resize(newSize * 4);
	}

	bool DwellingManager::CheckEmptySlot(Entity newEntity, Transform2d* transformPtr)
	{
		for (int i = 0; i < m_EntityIndex.size(); i++)
		{
			if (m_EntityIndex[i] == NULL)
			{
				m_EntityIndex[i] = newEntity;
				const DwarfSlots newDwarfSlot;
				m_DwarfSlots[i] = newDwarfSlot;
				const ReceiverInventory newFoodInventory;
				m_FoodInventories[i] = newFoodInventory;
				const unsigned int newCoolDown = 0;
				m_CoolDownFramesProgression[i] = 0;


				m_FoodInventories[i].resourceType = ResourceType::FOOD;

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


	void DwellingManager::DecreaseHappiness()
	{
		// TODO : Setup this function to decrease Happiness
	}
}
