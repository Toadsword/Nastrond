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

#include <extensions/dwarf_manager.h>
#include <iostream>
#include <python/python_engine.h>

namespace sfge::ext
{
	DwarfManager::DwarfManager(sfge::Engine& engine) :
		System(engine)
	{
		
	}

	void DwarfManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
		m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();

		auto* pyEngine = m_Engine.GetPythonEngine();
		PySystem* tmpSystem = pyEngine->GetPySystemManager().GetPySystemFromClassName("NavigationGraphManager");

		/*
		 * TO CHANGE 
		 */
		m_NavigationGraphManager = pyEngine->GetPySystemManager().GetNavigationGraphManager();

		auto config = m_Engine.GetConfig();
		Vec2f screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);

		auto* entityManager = m_Engine.GetEntityManager();
		entityManager->ResizeEntityNmb(m_entitiesNmb);

		//Load texture
		std::string texturePath = "data/sprites/triangle.png";
		const auto textureId = m_TextureManager->LoadTexture(texturePath);
		const auto texture = m_TextureManager->GetTexture(textureId);

		for (auto i = 0u; i < m_entitiesNmb; i++)
		{
			const auto newEntity = entityManager->CreateEntity(i + 1);

			//Add transform
			auto transformPtr = m_Transform2DManager->AddComponent(newEntity);
			transformPtr->Position = Vec2f(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y));

			//Add sprite
			auto sprite = m_SpriteManager->AddComponent(newEntity);
			sprite->SetTexture(texture);

			auto& spriteInfo = m_SpriteManager->GetComponentInfo(newEntity);
			spriteInfo.name = "Sprite";
			spriteInfo.sprite = sprite;
			spriteInfo.textureId = textureId;
			spriteInfo.texturePath = texturePath;

			//Set state
			m_states[i] = State::IDLE;

			//Set Path
			m_paths[i] = std::vector<Vec2f>();
		}
	}
	void DwarfManager::Update(float dt)
	{
		for (auto i = 0u; i < m_entitiesNmb; i++)
		{
			if(m_states[i] == State::IDLE)
			{
				auto config = m_Engine.GetConfig();
				Vec2f screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);

				auto transformPtr = m_Engine.GetTransform2dManager()->GetComponentPtr(i + 1);
				m_paths[i] = m_NavigationGraphManager->GetPathFromTo(transformPtr->Position, Vec2f(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y)));
				m_states[i] = State::WALKING;
			}
		}
	}
	void DwarfManager::FixedUpdate()
	{
		for (auto i = 0u; i < m_entitiesNmb; i++)
		{
			if (m_states[i] == State::WALKING)
			{
				auto transformPtr = m_Engine.GetTransform2dManager()->GetComponentPtr(i + 1);

				Vec2f dir = m_paths[i][0] - transformPtr->Position;

				if(dir.GetMagnitude() < 5)
				{
					std::reverse(m_paths[i].begin(), m_paths[i].end());
					m_paths[i].pop_back();
					std::reverse(m_paths[i].begin(), m_paths[i].end());

					if(m_paths[i].empty())
					{
						m_states[i] = State::IDLE;
					}
				}
				else {

					transformPtr->Position += dir.Normalized() * 2;
				}
			}
		}
	}

	void DwarfManager::Draw()
	{
		return; 
		auto graphics2DManager = m_Engine.GetGraphics2dManager();
		auto window = graphics2DManager->GetWindow();

		std::vector<sf::Color> colors;
		colors.push_back(sf::Color::Black);
		colors.push_back(sf::Color::Blue);
		colors.push_back(sf::Color::Cyan);
		colors.push_back(sf::Color::Green);
		colors.push_back(sf::Color::Magenta);
		colors.push_back(sf::Color::Red);
		colors.push_back(sf::Color::Yellow);

		for(int i = 0u; i < m_entitiesNmb; i++) {
			const auto color = colors[i % colors.size()];

			sf::VertexArray lines{sf::LineStrip, m_paths[i].size()};
			for(int j = 0u; j < m_paths[i].size(); j++) {
				lines[j].position = m_paths[i][j];
				
				lines[j].color = color;
			}

			window->draw(lines);
		}
	}
}
