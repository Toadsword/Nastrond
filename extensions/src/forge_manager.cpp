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

sfge::ext::ForgeManager::ForgeManager(Engine& engine): System(engine) {}

void sfge::ext::ForgeManager::Init()
{
	m_Transform2DManager = m_Engine.GetTransform2dManager();
	m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
	m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();

	Configuration* configuration = m_Engine.GetConfig();
	Vec2f screenSize = sf::Vector2f(configuration->screenResolution.x, configuration->screenResolution.y);

	auto* entityManager = m_Engine.GetEntityManager();

#ifdef TEST_SYSTEM_DEBUG
	entityManager->ResizeEntityNmb(m_entitiesNmb + 100);

	for (unsigned i = 0; i < m_entitiesNmb; i++)
	{
		SpawnForge(Vec2f(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y)));
	}
#endif
}

void sfge::ext::ForgeManager::Update(float dt)
{
}

void sfge::ext::ForgeManager::FixedUpdate()
{
	ProduceTools();
}

void sfge::ext::ForgeManager::Draw()
{
}

void sfge::ext::ForgeManager::SpawnForge(Vec2f pos)
{
	auto* entityManager = m_Engine.GetEntityManager();
	const auto newEntity = entityManager->CreateEntity(0);

	if (!CheckEmptySlot(newEntity))
	{
		size_t newForge = m_forgeEntityIndex.size() + 1;

		ResizeContainer(newForge);

		m_ironsInventories[newForge - 1].ressourceType = RessourceType::IRON;
		m_toolsInventories[newForge - 1].ressourceType = RessourceType::TOOL;
		m_progressionProdTool[newForge - 1].ressourceType = RessourceType::TOOL;

		m_forgeEntityIndex.push_back(newEntity);
	}

	//Load Texture
	std::string texturePath = "data/sprites/building.png";
	const auto textureId = m_TextureManager->LoadTexture(texturePath);
	const auto texture = m_TextureManager->GetTexture(textureId);

	//add transform
	auto transformPtr = m_Transform2DManager->AddComponent(newEntity);
	transformPtr->Position = Vec2f(pos.x, pos.y);

	//add texture
	auto sprite = m_SpriteManager->AddComponent(newEntity);
	sprite->SetTexture(texture);

	editor::SpriteInfo& spriteInfo = m_SpriteManager->GetComponentInfo(newEntity);
	spriteInfo.name = "sprite";
	spriteInfo.sprite = sprite;
	spriteInfo.textureId = textureId;
	spriteInfo.texturePath = texturePath;
}

bool sfge::ext::ForgeManager::AddDwarfToForge(Entity mineEntity)
{
	for (int i = 0; i < m_forgeEntityIndex.size(); i++)
	{
		if (m_forgeEntityIndex[i] == mineEntity)
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

void sfge::ext::ForgeManager::ResizeContainer(const size_t newSize)
{
	m_dwarfSlots.resize(newSize);
	m_forgeEntityIndex.resize(newSize);
	m_toolsInventories.resize(newSize);
	m_ironsInventories.resize(newSize);
	m_progressionProdTool.resize(newSize);
}

void sfge::ext::ForgeManager::ProduceTools()
{
	for (int i = 0; i < m_entitiesNmb; i++)
	{

		if(m_forgeEntityIndex[i] == NULL)
		{
			continue;
		}

		//Just for test
		for (int i = 0; i < m_entitiesNmb; i++)
		{
			m_ironsInventories[i].inventory = 1000;
		}

		if (m_ironsInventories[i].inventory <= 0 || m_toolsInventories[i].inventory + m_toolsInventories[i].packNumber * m_stackSize >= m_toolsInventories[i].maxCapacity)
		{
			continue;
		}

		m_progressionProdTool[i].FrameCoolDown += m_dwarfSlots[i].dwarfIn;

		if (m_progressionProdTool[i].FrameCoolDown >= m_FrameBeforAdd)
		{
			m_ironsInventories[i].inventory--;
			m_progressionProdTool[i].progression++;
			m_progressionProdTool[i].FrameCoolDown = 0;
		}

		if (m_progressionProdTool[i].progression >= m_progressionProdTool[i].goal)
		{
			m_toolsInventories[i].inventory++;
			m_progressionProdTool[i].progression = 0;

			if(m_toolsInventories[i].inventory >= m_stackSize)
			{
				m_toolsInventories[i].packNumber++;
				m_toolsInventories[i].inventory -= m_stackSize;
			}
		}

#ifdef DEBUG_CHECK_PRODUCTION

		std::cout << "Iron Inventory of mine " + std::to_string(i + 1) + " : " + std::to_string(m_toolsInventories[i].inventory) +
			" / and pack Number : " + std::to_string(m_toolsInventories[i].packNumber) + " / progression : " + std::to_string(m_progressionProdTool[i].progression) + "\n";
		if (i + 1 == m_entitiesNmb)
		{
			std::cout << "\n";
		}
#endif
	}
}

bool sfge::ext::ForgeManager::CheckEmptySlot(Entity newEntity)
{
		for (int i = 0; i < m_forgeEntityIndex.size(); i++)
		{
			if (m_forgeEntityIndex[i] == NULL)
			{
				m_forgeEntityIndex[i] = newEntity;
				const DwarfSlots newDwarfSlot;
				m_dwarfSlots[i] = newDwarfSlot;
				const RecieverInventory newIronInventory;
				m_ironsInventories[i] = newIronInventory;
				const GiverInventory newToolInventory;
				m_toolsInventories[i] = newToolInventory;
				const ProgressionProduction newProgressionProdTool;
				m_progressionProdTool[i] = newProgressionProdTool;

				m_ironsInventories[i].ressourceType = RessourceType::IRON;
				m_toolsInventories[i].ressourceType = RessourceType::TOOL;
				m_progressionProdTool[i].ressourceType = RessourceType::TOOL;

				return true;
			}
		}
		return false;
}


