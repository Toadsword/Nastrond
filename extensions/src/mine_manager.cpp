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

sfge::ext::MineManager::MineManager(Engine& engine) : System(engine) {}

void sfge::ext::MineManager::Init()
{
	m_Transform2DManager = m_Engine.GetTransform2dManager();
	m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
	m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();

	Configuration* configuration = m_Engine.GetConfig();
	Vec2f screenSize = sf::Vector2f(configuration->screenResolution.x, configuration->screenResolution.y);

	EntityManager* entityManager = m_Engine.GetEntityManager();

#ifdef TEST_SYSTEM_DEBUG
	entityManager->ResizeEntityNmb(m_entitiesNmb + 100);

	for(int i = 0; i < m_entitiesNmb; i++)
	{
		AddNewMine(Vec2f(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y)));
	}
#endif
}

void sfge::ext::MineManager::Update(float dt)
{
	
}

void sfge::ext::MineManager::FixedUpdate()
{
	RessourcesProduction();
}

void sfge::ext::MineManager::Draw()
{
}



void sfge::ext::MineManager::AddNewMine(Vec2f pos)
{
	auto* entityManager = m_Engine.GetEntityManager();
	const auto newEntity = entityManager->CreateEntity(0);

	size_t newForge = m_mineEntityIndex.size() + 1;

	ResizeContainer(newForge);

	m_mineEntityIndex.push_back(newEntity);

	//Load Texture
	std::string texturePath = "data/sprites/building.png";
	const auto textureId = m_TextureManager->LoadTexture(texturePath);
	const auto texture = m_TextureManager->GetTexture(textureId);

	//add transform
	auto transformPtr = m_Transform2DManager->AddComponent(newEntity);
	transformPtr->Position = Vec2f(pos.x, pos.y);
	transformPtr->Scale = Vec2f(0.1f, 0.1f);

	//add texture
	auto sprite = m_SpriteManager->AddComponent(newEntity);
	sprite->SetTexture(texture);

	editor::SpriteInfo& spriteInfo = m_SpriteManager->GetComponentInfo(newEntity);
	spriteInfo.name = "sprite";
	spriteInfo.sprite = sprite;
	spriteInfo.textureId = textureId;
	spriteInfo.texturePath = texturePath;

	m_IronProduction[newForge - 1].ressourceType = RessourceType::IRON;
}

bool sfge::ext::MineManager::AddDwarfToMine(Entity mineEntity)
{
	for (int i = 0; i < m_mineEntityIndex.size(); i++)
	{
		if (m_mineEntityIndex[i] == mineEntity)
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

bool sfge::ext::MineManager::DestroyMine(Entity mineEntity)
{
	for(int i = 0; i < m_mineEntityIndex.size(); i++)
	{
		if(m_mineEntityIndex[i] == mineEntity)
		{
			m_mineEntityIndex[i] = NULL;
			EntityManager* entityManager = m_Engine.GetEntityManager();
			entityManager->DestroyEntity(mineEntity);
			return true;
		}
	}
	return false;
}

void sfge::ext::MineManager::RessourcesProduction()
{
	for (unsigned i = 0; i < m_entitiesNmb; i++)
	{
		if(m_mineEntityIndex[i] == NULL)
		{
			continue;
		}

		GiverInventory tmpIronInventory = m_IronProduction[i];

		//Check if the inventory is full
		if (!(tmpIronInventory.packNumber * m_packSize >= tmpIronInventory.maxCapacity))
		{
			//Produce Iron by checking the number of dwarf in the building
			tmpIronInventory.inventory += m_ProductionRate * m_dwarfSlots[i].dwarfIn;

			if (tmpIronInventory.inventory >= m_packSize)
			{
				tmpIronInventory.inventory -= m_packSize;
				tmpIronInventory.packNumber++;
				IronStackAvalaible(i + 1);
			}

			m_IronProduction[i].packNumber = tmpIronInventory.packNumber;
			m_IronProduction[i].inventory = tmpIronInventory.inventory;
		}

#ifdef DEBUG_CHECK_PRODUCTION

		std::cout << "Iron Inventory of mine " + std::to_string(i + 1) + " : " + std::to_string(tmpIronInventory.inventory) +
			" / and pack Number : " + std::to_string(tmpIronInventory.packNumber) + "\n";
		if (i + 1 == m_entitiesNmb)
		{
			std::cout << "\n";
		}
#endif
	}
}

void sfge::ext::MineManager::ResizeContainer(size_t newSize)
{
	m_mineEntityIndex.resize(newSize);
	m_dwarfSlots.resize(newSize);
	m_IronProduction.resize(newSize);
}

void sfge::ext::MineManager::IronStackAvalaible(Entity entity)
{
	
}

