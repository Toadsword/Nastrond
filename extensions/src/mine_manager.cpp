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
	entityManager->ResizeEntityNmb(m_entitiesNmb);


	//Load Texture
	std::string texturePath = "data/sprites/building.png";

	const auto textureId = m_TextureManager->LoadTexture(texturePath);
	const auto texture = m_TextureManager->GetTexture(textureId);


	for (unsigned i = 0u; i < m_entitiesNmb; i++)
	{
		const auto newEntitiy = entityManager->CreateEntity(i + 1);

		//add transform
		auto transformPtr = m_Transform2DManager->AddComponent(newEntitiy);
		transformPtr->Position = Vec2f(0, 0);

		//add texture
		auto sprite = m_SpriteManager->AddComponent(newEntitiy);
		sprite->SetTexture(texture);

		editor::SpriteInfo& spriteInfo = m_SpriteManager->GetComponentInfo(newEntitiy);
		spriteInfo.name = "sprite";
		spriteInfo.sprite = sprite;
		spriteInfo.textureId = textureId;
		spriteInfo.texturePath = texturePath;
	}
}

void sfge::ext::MineManager::Update(float dt)
{
	
}

void sfge::ext::MineManager::FixedUpdate()
{
	for (unsigned int i = 0; i < m_entitiesNmb; i++)
	{
		GiverInventory tmpIronInventory = m_IronProduction[i];
		if (tmpIronInventory.packNumber * tmpIronInventory.packSize >= tmpIronInventory.MAX_CAPACITY)
		{
			continue;
		}

		tmpIronInventory.inventory += m_ProductionRate * 5;

		if (tmpIronInventory.inventory >= tmpIronInventory.packSize)
		{
			tmpIronInventory.inventory -= tmpIronInventory.packSize;
			tmpIronInventory.packNumber++;
		}

		std::cout << "Iron Inventory of mine " + std::to_string(i) + " : " + std::to_string(tmpIronInventory.inventory) + " / and pack Number : " + std::to_string(tmpIronInventory.packNumber) + "\n";
		m_IronProduction[i].packNumber = tmpIronInventory.packNumber;
		m_IronProduction[i].inventory = tmpIronInventory.inventory;
		if(i + 1 == m_entitiesNmb)
		{
			std::cout << "\n";
		}
	}
}

void sfge::ext::MineManager::Draw()
{
}


