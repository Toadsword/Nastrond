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

#include <extensions/building_noentity_manager.h>

namespace sfge::ext
{

	BuildingNoEntityManager::BuildingNoEntityManager(Engine & engine) : System(engine) {}

	void BuildingNoEntityManager::Init()
	{
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();

		//Load texture
		m_TexturePath = "data/sprites/building.png";
		m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
		m_Texture = m_TextureManager->GetTexture(m_TextureId);

		m_VertexArray = sf::VertexArray(sf::Quads, 0);

		for (int i = 0; i < 1000; i++)
		{
			AddNewBuilding(Vec2f((std::rand() % static_cast<int>(600)), (std::rand() % static_cast<int>(600))));
		}
	}

	void BuildingNoEntityManager::Update(float dt)
	{
		(void)dt;
	}

	void BuildingNoEntityManager::FixedUpdate()
	{
		return;
		//Consume();
	}

	void BuildingNoEntityManager::Draw()
	{
		auto window = m_Engine.GetGraphics2dManager()->GetWindow();

		window->draw(m_VertexArray, m_Texture);
	}

	void BuildingNoEntityManager::AddNewBuilding(Vec2f pos)
	{

			const size_t newDwelling = m_EntityIndex.size();

			ResizeContainer(newDwelling + CONTAINER_EXTENDER);
			m_FoodInventories[newDwelling].resourceType = ResourceType::FOOD;
			m_CoolDownFramesProgression[newDwelling] = 0u;

			m_EntityIndex[newDwelling] = newDwelling + 1;

			const sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

			m_VertexArray[4 * newDwelling].texCoords = sf::Vector2f(0, 0);
			m_VertexArray[4 * newDwelling + 1].texCoords = sf::Vector2f(textureSize.x, 0);
			m_VertexArray[4 * newDwelling + 2].texCoords = textureSize;
			m_VertexArray[4 * newDwelling + 3].texCoords = sf::Vector2f(0, textureSize.y);

			m_VertexArray[4 * newDwelling].position = pos - textureSize / 2.0f;
			m_VertexArray[4 * newDwelling + 1].position = pos + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
			m_VertexArray[4 * newDwelling + 2].position = pos + textureSize / 2.0f;
			m_VertexArray[4 * newDwelling + 3].position = pos + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);
	}

	void BuildingNoEntityManager::Consume()
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
			else if (m_DwarfSlots[i].dwarfIn <= 0)
			{
				continue;
			}

			m_CoolDownFramesProgression[i]++;

			if (m_CoolDownFramesProgression[i] >= m_CoolDownFrames)
			{
				m_FoodInventories[i].inventory -= m_DwarfSlots[i].dwarfIn;
				if (m_FoodInventories[i].inventory < 0)
				{
					m_FoodInventories[i].inventory = 0;
					DecreaseHappiness();
				}
			}
		}
	}

	void BuildingNoEntityManager::ResizeContainer(const size_t newSize)
	{
		m_EntityIndex.resize(newSize, INVALID_ENTITY);
		m_DwarfSlots.resize(newSize);
		m_FoodInventories.resize(newSize);
		m_CoolDownFramesProgression.resize(newSize);
		m_VertexArray.resize(newSize * 4);
	}


	void BuildingNoEntityManager::DecreaseHappiness()
	{
		// TODO : Setup this function to decrease Happiness
	}
}
