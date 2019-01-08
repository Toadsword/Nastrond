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

		/*std::vector<std::vector<int>> map{ 
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 0, 1, 0, 1, 2, 2, 1, 0},
			{0, 1, 0, 1, 0, 1, 2, 2, 1, 0},
			{0, 1, 0, 1, 0, 1, 2, 2, 1, 0},
			{0, 1, 0, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
			{0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
		};*/

		std::vector<std::vector<int>> map{
			{1, 1, 1, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1}
		};

		BuildGraphFromArray(map);

		GetPathFromTo(Vec2f(100, 100), Vec2f(600, 300));
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

		for(int i = 0; i < m_Graph.size(); i++) {

			Vec2f pos = m_Graph[i].pos;

			// define the position of the quad's points
			quad[0].position = sf::Vector2f(pos.x - extends.x / 2.f, pos.y - extends.y / 2.f);
			quad[1].position = sf::Vector2f(pos.x + extends.x / 2.f, pos.y - extends.y / 2.f);
			quad[2].position = sf::Vector2f(pos.x + extends.x / 2.f, pos.y + extends.y / 2.f);
			quad[3].position = sf::Vector2f(pos.x - extends.x / 2.f, pos.y + extends.y / 2.f);

#ifdef DEBUG_MOD
			switch (m_Graph[i].state) {
			case GraphNodeDebug::VISITED: 
				quad[0].color = sf::Color::Yellow;
				quad[1].color = sf::Color::Yellow;
				quad[2].color = sf::Color::Yellow;
				quad[3].color = sf::Color::Yellow;
				break;
			case GraphNodeDebug::START_POSITION:
				quad[0].color = sf::Color::Green;
				quad[1].color = sf::Color::Green;
				quad[2].color = sf::Color::Green;
				quad[3].color = sf::Color::Green;
				break;
			case GraphNodeDebug::END_POSITION:
				quad[0].color = sf::Color::Magenta;
				quad[1].color = sf::Color::Magenta;
				quad[2].color = sf::Color::Magenta;
				quad[3].color = sf::Color::Magenta;
				break;
			case GraphNodeDebug::PASSAGE_POSITION:
				quad[0].color = sf::Color::Cyan;
				quad[1].color = sf::Color::Cyan;
				quad[2].color = sf::Color::Cyan;
				quad[3].color = sf::Color::Cyan;
				break;
			case GraphNodeDebug::OTHER:
				if (m_Graph[i].cost == GraphNode::SOLID_COST) {
					quad[0].color = sf::Color::Red;
					quad[1].color = sf::Color::Red;
					quad[2].color = sf::Color::Red;
					quad[3].color = sf::Color::Red;
				}
				else {
					quad[0].color = sf::Color::White;
					quad[1].color = sf::Color::White;
					quad[2].color = sf::Color::White;
					quad[3].color = sf::Color::White;
				}
				break;
			default: ;
			}
#else
			// define the color of the quad's points
			if (m_Graph[i].cost == GraphNode::SOLID_COST) {
				quad[0].color = sf::Color::Red;
				quad[1].color = sf::Color::Red;
				quad[2].color = sf::Color::Red;
				quad[3].color = sf::Color::Red;
			}
			else {
				quad[0].color = sf::Color::White;
				quad[1].color = sf::Color::White;
				quad[2].color = sf::Color::White;
				quad[3].color = sf::Color::White;
			}
#endif

			window->draw(quad);
#ifndef DEBUG_MOD
			for (int j = 0; j < m_Graph[i].neighborsIndex.size(); j++) {
				m_Graphics2DManager->DrawLine(m_Graph[i].pos, m_Graph[m_Graph[i].neighborsIndex[j]].pos, sf::Color::White);
			}
#endif
		}
	}

	/**
	 * \brief Create graph nodes from an array 2x2 of cost
	 * \param map array 2x2 of cost
	 */
	void NavigationGraphManager::BuildGraphFromArray(const std::vector<std::vector<int>> map)
	{
		for(int y = 0; y < map.size(); y++) {
			for(int x = 0; x < map[y].size(); x++) {

#ifdef DEBUG_MOD
				GraphNodeDebug node;
				node.state = GraphNodeDebug::State::OTHER;
#else
				GraphNode node;
#endif

				node.cost = map[y][x];
				node.pos = Vec2f(x * 50 + 25, y * 50 + 25);

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

				if(node.cost == GraphNode::SOLID_COST) {
					continue;
				}

				for(int i = -1; i < 2; i++) {
					for(int j = -1; j < 2; j++) {
						if (i == 0 && j == 0) continue;
						
						if (y + i < 0 || y + i >= map.size() || x + j < 0 || x + j >= map[y].size()) continue;

						const int indexNeighbor = (y + i) * map.size() + (x + j);

						if(m_Graph[indexNeighbor].cost == GraphNode::SOLID_COST) continue;

						//Add cross without checking other neighbors
						if (i == 0 || j == 0) {
							node.neighborsIndex.push_back(indexNeighbor);
						}else {
							if(m_Graph[y * map.size() + (x + j)].cost != GraphNode::SOLID_COST &&
							   m_Graph[(y + i) * map.size() + x].cost != GraphNode::SOLID_COST) {
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
	std::vector<Vec2f> NavigationGraphManager::GetPathFromTo(Vec2f origin, Vec2f destination)
	{
		float distanceOrigin = INFINITY;
		float distanceDestination = INFINITY;

		short indexOrigin, indexDestination;

		for(int i = 0; i < m_Graph.size(); i++) {
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

#ifdef DEBUG_MOD	
		GraphNodeDebug originNode;
		GraphNodeDebug destinationNode;
#else
		GraphNode originNode;
		GraphNode destinationNode;
#endif

		originNode = m_Graph[indexOrigin];
		destinationNode = m_Graph[indexDestination];

		PriorityQueue<short, float> openNodes;
		openNodes.Put(indexOrigin, 0);

		std::unordered_map<short, short> cameFrom;
		std::unordered_map<short, float> costSoFar;

		cameFrom[indexOrigin] = indexOrigin;
		costSoFar[indexOrigin] = 0;

		while(!openNodes.Empty()) {
			int indexCurrent = openNodes.Get();

			if(indexCurrent == indexDestination) {
				break;
			}

#ifdef DEBUG_MOD
			GraphNodeDebug currentNode = m_Graph[indexCurrent];
			m_Graph[indexCurrent].state = GraphNodeDebug::State::VISITED;
#else
			GraphNode currentNode = m_Graph[indexCurrent];
#endif

			for(int i = 0; i < currentNode.neighborsIndex.size(); i++) {
				int indexNext = currentNode.neighborsIndex[i];
				float newCost = costSoFar[indexCurrent] + m_Graph[indexNext].cost;

				if(costSoFar.find(indexNext) == costSoFar.end() ||
					newCost < costSoFar[indexNext]) {
					costSoFar[indexNext] = newCost;
					
					float priority = newCost + ComputeHeuristic(indexNext, indexDestination);
					openNodes.Put(indexNext, priority);
					cameFrom[indexNext] = indexCurrent;
				}
			}
		}
#ifdef DEBUG_MOD
		std::vector<GraphNodeDebug> path;
		m_Graph[indexDestination].state = GraphNodeDebug::State::END_POSITION;
		m_Graph[indexOrigin].state = GraphNodeDebug::State::START_POSITION;

		bool isEndPoint = true;
#else
		std::vector<GraphNode> path;
#endif

		short currentNodeIndex = indexDestination;
		while(currentNodeIndex != indexOrigin) {
#ifdef DEBUG_MOD
			if(isEndPoint) {
				m_Graph[indexDestination].state = GraphNodeDebug::State::END_POSITION;
				isEndPoint = false;
			}else {
				m_Graph[currentNodeIndex].state = GraphNodeDebug::State::PASSAGE_POSITION;
			}
#endif
			path.push_back(m_Graph[currentNodeIndex]);
			currentNodeIndex = cameFrom[currentNodeIndex];
		}
		path.push_back(m_Graph[indexOrigin]);
		std::reverse(path.begin(), path.end());
		
		return std::vector<Vec2f>();
	}

	float NavigationGraphManager::GetSquaredDistance(Vec2f v1, Vec2f v2) const {
		return (std::abs(v1.x - v2.x) * std::abs(v1.x - v2.x)) + (std::abs(v1.y - v2.y) * std::abs(v1.y - v2.y));
	}

	float NavigationGraphManager::ComputeHeuristic(short currentNode, short destinationNode) const {
		float D = 1; 
		float D2 = std::sqrt(2);
			
		float dx = std::abs(m_Graph[currentNode].pos.x / 50 - m_Graph[destinationNode].pos.x / 50); //Divide pos by tile size
		float dy = std::abs(m_Graph[currentNode].pos.y / 50 - m_Graph[destinationNode].pos.y / 50); //Divide pos by tile size

		return D * (dx + dy) + (D2 - 2 * D) * std::min(dx, dy);
	}
}


