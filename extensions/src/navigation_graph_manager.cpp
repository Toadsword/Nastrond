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

#include <extensions/navigation_graph_manager.h>
#include <engine/engine.h>
#include <iostream>
#include "SFML/Graphics/RenderStates.hpp"

namespace sf {
	class String;
}

namespace sfge::ext
{
	NavigationGraphManager::NavigationGraphManager(Engine & engine) :
	System(engine)
	{
	}

	void NavigationGraphManager::Init()
	{
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();

		std::vector<std::vector<int>> map{ 
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 0, 1, 0, 0, 1, 1, 1, 0},
			{0, 1, 0, 1, 0, 0, 1, 1, 1, 0},
			{0, 1, 0, 1, 0, 0, 1, 1, 1, 0},
			{0, 1, 0, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
			{0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
		};

		BuildGraphFromArray(map);
	}
	void NavigationGraphManager::Update(float dt)
	{
		(void)dt;
	}

	void NavigationGraphManager::FixedUpdate() {
	}

	void NavigationGraphManager::Draw() {
		rmt_ScopedCPUSample(NavigationGraphManagerDraw, 0);
		
		auto window = m_Graphics2DManager->GetWindow();

		sf::VertexArray quad(sf::Quads, 4);

		Vec2f extends = Vec2f(10, 10);

		// define the color of the quad's points
		quad[0].color = sf::Color::White;
		quad[1].color = sf::Color::White;
		quad[2].color = sf::Color::White;
		quad[3].color = sf::Color::White;

		for(int i = 0; i < m_Graph.size(); i++) {

			Vec2f pos = m_Graph[i].pos;

			// define the position of the quad's points
			quad[0].position = sf::Vector2f(pos.x - extends.x / 2.f, pos.y - extends.y / 2.f);
			quad[1].position = sf::Vector2f(pos.x + extends.x / 2.f, pos.y - extends.y / 2.f);
			quad[2].position = sf::Vector2f(pos.x + extends.x / 2.f, pos.y + extends.y / 2.f);
			quad[3].position = sf::Vector2f(pos.x - extends.x / 2.f, pos.y + extends.y / 2.f);

			window->draw(quad);
		}
	}

	void NavigationGraphManager::BuildGraphFromArray(const std::vector<std::vector<int>> map)
	{
		for(int y = 0; y < map.size(); y++) {
			for(int x = 0; x < map[y].size(); x++) {
				GraphNode node;
				node.cost = map[y][x];
				node.pos = Vec2f(x * 50 + 25, y * 50 + 25);

				m_Graph.push_back(node);
			}
		}

		std::cout << "size = " + std::to_string(m_Graph.size()) + "\n";
	}
}


