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

#ifdef TEST_SYSTEM_DEBUG
		for (auto i = 0; i < m_EntitiesNmb; i++)
		{
			AddNewBuilding(Vec2f(Vec2f(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y))));
		}
#endif
	}

	void DwellingManager::Update(float dt)
	{
	}

	void DwellingManager::FixedUpdate()
	{
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

		size_t newDwelling = m_DwellingEntityIndex.size();

		ResizeContainer(newDwelling + 1);

		m_DwellingEntityIndex[newDwelling] = newEntity;


		////add texture
		//auto* sprite = m_SpriteManager->AddComponent(newEntity);
		//sprite->SetTexture(m_Texture);

		//auto& spriteInfo = m_SpriteManager->GetComponentInfo(newEntity);
		//spriteInfo.name = "sprite dwelling";
		//spriteInfo.sprite = sprite;
		//spriteInfo.textureId = m_TextureId;
		//spriteInfo.texturePath = m_TexturePath;

		//setup container
		//m_foodInventory[newDwelling].resourceType = RessourceType::FOOD;

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

	bool DwellingManager::AddDwarfToBuilding(Entity dwellingEntity)
	{
		for (int i = 0; i < m_DwellingEntityIndex.size(); i++)
		{
			if (m_DwellingEntityIndex[i] == dwellingEntity)
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
			return false;
		}
	}

	bool DwellingManager::RemoveDwarfToBuilding(Entity mineEntity)
	{
		return false;
	}

	void DwellingManager::ResizeContainer(const size_t newSize)
	{
		m_DwellingEntityIndex.resize(newSize);
		m_DwarfSlots.resize(newSize);
		m_FoodInventory.resize(newSize);
		m_VertexArray.resize(newSize * 4);

	}

	bool DwellingManager::CheckEmptySlot(Entity newEntity)
	{
		for (int i = 0; i < m_DwellingEntityIndex.size(); i++)
		{
			if (m_DwellingEntityIndex[i] == NULL)
			{
				m_DwellingEntityIndex[i] = newEntity;
				const DwarfSlots newDwarfSlot;
				m_DwarfSlots[i] = newDwarfSlot;
				const ReceiverInventory newFoodIventory;
				m_FoodInventory[i] = newFoodIventory;

				m_FoodInventory[i].resourceType = ResourceType::FOOD;

				return true;
			}
		}
		return false;
	}
}
