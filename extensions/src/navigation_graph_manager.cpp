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
#include "utility/priority_queue.h"

namespace sfge::ext
{
	NavigationGraphManager::NavigationGraphManager(Engine & engine) :
	System(engine)
	{
	}

	void NavigationGraphManager::Init()
	{
		m_Graphics2DManager = m_Engine.GetGraphics2dManager();

#ifdef DEBUG_MAP
		std::vector<std::vector<int>> map;

		int size = 250u;

		for(int i = 0; i < size; i++) {
			std::vector<int> line;
			for(int j = 0; j < size; j++) {
				line.push_back(1);
			}
			map.push_back(line);
		}

		for(int i = 0; i < size; i ++) {
			Vec2f pos(std::rand() % size, std::rand() % size);

			int width = std::rand() % 15;
			int height = std::rand() % 15;

			for(int x = 0; x < width; x++) {
				for(int y = 0; y < height; y++) {
					if(pos.x + x < size && pos.y +y < size) {
						map[pos.x + x][pos.y + y] = 0;
					}
				}
			}
		}

		vertexArray = sf::VertexArray(sf::Quads, 4 * map.size() * map[0].size());

		m_mapSize = Vec2f(map.size(), map[0].size());

		BuildGraphFromArray(map);
#endif
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
		window->draw(vertexArray);
		return;
#ifdef DEBUG_MOD
		for (GraphNodeDebug node : m_Graph) {
#else
		for (GraphNode node : m_Graph) {
#endif
			Vec2f pos = node.pos;

#ifdef DEBUG_MOD
			switch (node.state) {
			case GraphNodeDebug::VISITED: 
				DrawQuad(window, pos, sf::Color::Yellow);
				break;
			case GraphNodeDebug::START_POSITION:
				DrawQuad(window, pos, sf::Color::Green);
				break;
			case GraphNodeDebug::END_POSITION:
				DrawQuad(window, pos, sf::Color::Magenta);
				break;
			case GraphNodeDebug::PASSAGE_POSITION:
				DrawQuad(window, pos, sf::Color::Cyan);
				break;
			case GraphNodeDebug::OTHER:
				if (node.cost == SOLID_COST) {
					DrawQuad(window, pos, sf::Color::Red);
				}
				else {
					DrawQuad(window, pos, sf::Color::White);
				}
				break;
			default: ;
			}
#else
			// define the color of the quad's points
			if (node.cost == SOLID_COST) {
				DrawQuad(window, pos, sf::Color::Red);
			}
			else {
				//DrawQuad(window, pos, sf::Color::White);
			}
#endif

#ifndef DEBUG_MOD
			/*for (auto neighborsIndex : node.neighborsIndex) {
				m_Graphics2DManager->DrawLine(pos, m_Graph[neighborsIndex].pos, sf::Color::White);
			}*/
#endif
		}
	}

	void NavigationGraphManager::DrawQuad(sf::RenderWindow* window, Vec2f pos, sf::Color col)
	{
		sf::VertexArray quad(sf::Quads, 4);

		// set position
		quad[0].position = sf::Vector2f(pos.x - m_tileExtends.x * 0.5f, pos.y - m_tileExtends.y * 0.5f);
		quad[1].position = sf::Vector2f(pos.x + m_tileExtends.x * 0.5f, pos.y - m_tileExtends.y * 0.5f);
		quad[2].position = sf::Vector2f(pos.x + m_tileExtends.x * 0.5f, pos.y + m_tileExtends.y * 0.5f);
		quad[3].position = sf::Vector2f(pos.x - m_tileExtends.x * 0.5f, pos.y + m_tileExtends.y * 0.5f);

		// set colors
		quad[0].color = col;
		quad[1].color = col;
		quad[2].color = col;
		quad[3].color = col;

		window->draw(quad);

		m_Graphics2DManager->DrawLine(pos + Vec2f(-m_tileExtends.x, -m_tileExtends.y) * 0.5f, pos + Vec2f(m_tileExtends.x, -m_tileExtends.y) * 0.5f, sf::Color::Black);
		m_Graphics2DManager->DrawLine(pos + Vec2f(m_tileExtends.x, -m_tileExtends.y) * 0.5f, pos + Vec2f(m_tileExtends.x, m_tileExtends.y) * 0.5f, sf::Color::Black);
		m_Graphics2DManager->DrawLine(pos + Vec2f(m_tileExtends.x, m_tileExtends.y) * 0.5f, pos + Vec2f(-m_tileExtends.x, m_tileExtends.y) * 0.5f, sf::Color::Black);
		m_Graphics2DManager->DrawLine(pos + Vec2f(-m_tileExtends.x, m_tileExtends.y) * 0.5f, pos + Vec2f(-m_tileExtends.x, -m_tileExtends.y) * 0.5f, sf::Color::Black);
	}

	/**
	 * \brief Create graph nodes from an array 2x2 of cost
	 * \param map array 2x2 of cost
	 */
	void NavigationGraphManager::BuildGraphFromArray(std::vector<std::vector<int>>& map)
	{
		for(auto y = 0; y < map.size(); y++) {
			for(auto x = 0; x < map[y].size(); x++) {

#ifdef DEBUG_MOD
				GraphNodeDebug node;
				node.state = GraphNodeDebug::State::OTHER;
#else
				GraphNode node;
#endif

				node.cost = map[y][x] * m_tileExtends.x;
				node.pos = Vec2f(x * m_tileExtends.x + m_tileExtends.x * 0.5f, y * m_tileExtends.y + m_tileExtends.y * 0.5f);

				// set position
				vertexArray[4 * ((y * map.size()) + x)].position = sf::Vector2f(node.pos.x - m_tileExtends.x * 0.5f, node.pos.y - m_tileExtends.y * 0.5f);
				vertexArray[4 * ((y * map.size()) + x) + 1].position = sf::Vector2f(node.pos.x + m_tileExtends.x * 0.5f, node.pos.y - m_tileExtends.y * 0.5f);
				vertexArray[4 * ((y * map.size()) + x) + 2].position = sf::Vector2f(node.pos.x + m_tileExtends.x * 0.5f, node.pos.y + m_tileExtends.y * 0.5f);
				vertexArray[4 * ((y * map.size()) + x) + 3].position = sf::Vector2f(node.pos.x - m_tileExtends.x * 0.5f, node.pos.y + m_tileExtends.y * 0.5f);

				if(node.cost == SOLID_COST) {

					// set colors
					vertexArray[4 * ((y * map.size()) + x)].color = sf::Color::Red;
					vertexArray[4 * ((y * map.size()) + x) + 1].color = sf::Color::Red;
					vertexArray[4 * ((y * map.size()) + x) + 2].color = sf::Color::Red;
					vertexArray[4 * ((y * map.size()) + x) + 3].color = sf::Color::Red;
				}else {
					// set colors
					vertexArray[4 * ((y * map.size()) + x)].color = sf::Color::White;
					vertexArray[4 * ((y * map.size()) + x) + 1].color = sf::Color::White;
					vertexArray[4 * ((y * map.size()) + x) + 2].color = sf::Color::White;
					vertexArray[4 * ((y * map.size()) + x) + 3].color = sf::Color::White;
				}

				m_Graph.push_back(node);
			}
		}

		for (int y = 0; y < map.size(); y++) {
			for (int x = 0; x < map[y].size(); x++) {
				const int index = y * map.size() + x; 
#ifdef DEBUG_MOD
					GraphNodeDebug node = m_Graph[index];
#else
					GraphNode node = m_Graph[index];
#endif

				if(node.cost == SOLID_COST) {
					continue;
				}

				for(int i = -1; i < 2; i++) {
					for(int j = -1; j < 2; j++) {
						if (i == 0 && j == 0) continue;
						
						if (y + i < 0 || y + i >= map.size() || x + j < 0 || x + j >= map[y].size()) continue;

						const int indexNeighbor = (y + i) * map.size() + (x + j);

						if(m_Graph[indexNeighbor].cost == SOLID_COST) continue;

						//Add cross without checking other neighbors
						if (i == 0 || j == 0) {
							node.neighborsIndex.push_back(indexNeighbor);
						}else {
							if(m_Graph[y * map.size() + (x + j)].cost != SOLID_COST &&
							   m_Graph[(y + i) * map.size() + x].cost != SOLID_COST) {
								node.neighborsIndex.push_back(indexNeighbor);
							}
						}
					}
				}
				m_Graph[index] = node;
			}
		}
	}

	/**
	 * \brief Function to get a path from a point to another 
	 * \param origin position from where the path must start
	 * \param destination position where the agent want to go
	 * \return the path between the origin and the destination
	 */
	std::vector<Vec2f> NavigationGraphManager::GetPathFromTo(Vec2f& origin, Vec2f& destination)
	{
		float distanceOrigin = INFINITY;
		float distanceDestination = INFINITY;

		unsigned int indexOrigin = 0, indexDestination = 0;

		for(auto i = 0u; i < m_Graph.size(); i++) {
			if(m_Graph[i].cost == SOLID_COST)
			{
				continue;
			}

			float dist1 = GetSquaredDistance(origin, m_Graph[i].pos);

			if(dist1 < distanceOrigin) {
				distanceOrigin = dist1;
				indexOrigin = i;
			}

			float dist2 = GetSquaredDistance(destination, m_Graph[i].pos);

			if (dist2 < distanceDestination) {
				distanceDestination = dist2;
				indexDestination = i;
			}
		}

		return GetPathFromTo(indexOrigin, indexDestination);
	}

	std::vector<Vec2f> NavigationGraphManager::GetPathFromTo(const unsigned int originIndex,const unsigned int destinationIndex)
	{
#ifdef DEBUG_MOD	
		GraphNodeDebug destinationNode = m_Graph[destinationIndex];
#else
		GraphNode destinationNode = m_Graph[destinationIndex];
#endif

		PriorityQueue<unsigned int, float> openNodes;
		openNodes.Put(originIndex, 0);

		std::unordered_map<unsigned int, unsigned int> cameFrom;
		std::unordered_map<unsigned int, unsigned short> costSoFar;

		cameFrom[originIndex] = originIndex;
		costSoFar[originIndex] = 0;

		while (!openNodes.Empty()) {
			const auto indexCurrent = openNodes.Get();

			if (indexCurrent == destinationIndex) {
				break;
			}

#ifdef DEBUG_MOD
			m_Graph[indexCurrent].state = GraphNodeDebug::State::VISITED;
#endif
			for (auto neighborsIndex : m_Graph[indexCurrent].neighborsIndex) {
				const unsigned short newCost = costSoFar[indexCurrent] + m_Graph[neighborsIndex].cost;

				if (costSoFar.find(neighborsIndex) == costSoFar.end() ||
					newCost < costSoFar[neighborsIndex]) {
					costSoFar[neighborsIndex] = newCost;

					openNodes.Put(neighborsIndex, newCost + ComputeHeuristic(neighborsIndex, destinationIndex));
					cameFrom[neighborsIndex] = indexCurrent;
				}
			}
		}
#ifdef DEBUG_MOD
		std::vector<GraphNodeDebug> path;
		m_Graph[destinationIndex].state = GraphNodeDebug::State::END_POSITION;
		m_Graph[originIndex].state = GraphNodeDebug::State::START_POSITION;

		bool isEndPoint = true;
#else
		std::vector<GraphNode> path;
#endif
		std::vector<Vec2f> pathPos;
		auto currentNodeIndex = destinationIndex;
		while (currentNodeIndex != originIndex) {
			pathPos.push_back(m_Graph[currentNodeIndex].pos);

#ifdef DEBUG_MOD
			if (isEndPoint) {
				m_Graph[destinationIndex].state = GraphNodeDebug::State::END_POSITION;
				isEndPoint = false;
			}
			else {
				m_Graph[currentNodeIndex].state = GraphNodeDebug::State::PASSAGE_POSITION;
			}
#endif
			path.push_back(m_Graph[currentNodeIndex]);
			currentNodeIndex = cameFrom[currentNodeIndex];
		}
		path.push_back(m_Graph[originIndex]);
		pathPos.push_back(m_Graph[originIndex].pos);
		std::reverse(path.begin(), path.end());
		std::reverse(pathPos.begin(), pathPos.end());

		return pathPos;
	}

	float NavigationGraphManager::GetSquaredDistance(Vec2f& v1, Vec2f& v2) {
		return ((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y));
	}

	float NavigationGraphManager::ComputeHeuristic(unsigned int currentNode, unsigned int destinationNode) const {
		const auto dx = std::abs(m_Graph[currentNode].pos.x - m_Graph[destinationNode].pos.x);
		const auto dy = std::abs(m_Graph[currentNode].pos.y - m_Graph[destinationNode].pos.y);

		return HEURISTIC_1 * (dx + dy) + (HEURISTIC_2 - 2 * HEURISTIC_1) * std::min(dx, dy);
	}
}