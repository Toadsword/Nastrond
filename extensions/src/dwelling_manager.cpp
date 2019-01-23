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

sfge::ext::DwellingManager::DwellingManager(Engine & engine) : System(engine) {}

void sfge::ext::DwellingManager::Init()
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

#ifdef TEST_SYSTEM_DEBUG

	for(auto i = 0; i < m_entitiesNmb; i++)
	{
		AddNewDwelling(Vec2f(Vec2f(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y))));
	}
#endif
}

void sfge::ext::DwellingManager::Update(float dt)
{
}

void sfge::ext::DwellingManager::FixedUpdate()
{
}

void sfge::ext::DwellingManager::Draw()
{
}

void sfge::ext::DwellingManager::AddNewDwelling(Vec2f pos)
{
	auto* entityManager = m_Engine.GetEntityManager();
	Configuration* configuration = m_Engine.GetConfig();
	//entityManager->ResizeEntityNmb(configuration->currentEntitiesNmb + 1);

	const auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);

	size_t newDwelling = m_dwellingEntityIndex.size() + 1;

	ResizeContainer(newDwelling);
	
	m_dwellingEntityIndex.push_back(newEntity);

	//add transform
	auto* transformPtr = m_Transform2DManager->AddComponent(newEntity);
	transformPtr->Position = Vec2f(pos.x, pos.y);
	transformPtr->Scale = Vec2f(0.1f, 0.1f);

	//add texture
	auto* sprite = m_SpriteManager->AddComponent(newEntity);
	sprite->SetTexture(m_Texture);

	auto& spriteInfo = m_SpriteManager->GetComponentInfo(newEntity);
	spriteInfo.name = "sprite dwelling";
	spriteInfo.sprite = sprite;
	spriteInfo.textureId = m_TextureId;
	spriteInfo.texturePath = m_TexturePath;

	//setup container
	//m_foodInventory[newDwelling].ressourceType = RessourceType::FOOD;
	
}

bool sfge::ext::DwellingManager::AddDwarfToDwelling(Entity dwellingEntity)
{
	for (int i = 0; i < m_dwellingEntityIndex.size(); i++)
	{
		if (m_dwellingEntityIndex[i] == dwellingEntity)
		{
			if (m_dwarfSlots[i].dwarfAttributed < m_dwarfSlots[i].maxDwarfCapacity)
			{
				m_dwarfSlots[i].dwarfAttributed++;
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

void sfge::ext::DwellingManager::ResizeContainer(const size_t newSize)
{
	m_dwellingEntityIndex.resize(newSize);
	m_dwarfSlots.resize(newSize);
	m_foodInventory.resize(newSize);
}

