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

#include <extensions/dwarves_slots_manager.h>

sfge::ext::DwarvesSlotsManager::DwarvesSlotsManager(Engine & engine) : System(engine) {}

void sfge::ext::DwarvesSlotsManager::Init()
{
	m_Transform2DManager = m_Engine.GetTransform2dManager();
	m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
	m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();

	Configuration* configuration = m_Engine.GetConfig();
	Vec2f screenSize = sf::Vector2f(configuration->screenResolution.x, configuration->screenResolution.y);

	EntityManager* entityManager = m_Engine.GetEntityManager();
	entityManager->ResizeEntityNmb(m_entitiesnmb);

	//Load Texture
	std::string texturePath = "data/sprites/building.png";

	const auto textureId = m_TextureManager->LoadTexture(texturePath);
	const auto texture = m_TextureManager->GetTexture(textureId);


	for (unsigned i = 0u; i < m_entitiesnmb; i++)
	{
		const auto newEntitiy = entityManager->CreateEntity(i + 1);

		std::cout << "NEW ENTITY : " + (i + 1);
		std::cout << "/n";
		
		//add transform
		auto transformPtr = m_Transform2DManager->AddComponent(newEntitiy);
		transformPtr->Position = Vec2f(screenSize.x / 2, screenSize.y / 2);

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

void sfge::ext::DwarvesSlotsManager::Update(float dt)
{
}

void sfge::ext::DwarvesSlotsManager::FixedUpdate()
{
}

void sfge::ext::DwarvesSlotsManager::Draw()
{
	auto graphics2DManager = m_Engine.GetGraphics2dManager();
	auto window = graphics2DManager->GetWindow();
	
	sf::VertexArray line{ sf::LineStrip, 2 };
	line[0].position = sf::Vector2f(0, 0);
	line[1].position = sf::Vector2f(640, 360);
	window->draw(line);
}

