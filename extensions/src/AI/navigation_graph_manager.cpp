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

#include <engine/engine.h>
#include <utility/priority_queue.h>

#include <extensions/AI/navigation_graph_manager.h>
#include "engine/tilemap.h"

namespace sfge::ext
{
NavigationGraphManager::NavigationGraphManager(Engine& engine) :
	System(engine) {}

void NavigationGraphManager::Init()
{
	m_Graphics2DManager = m_Engine.GetGraphics2dManager();

	//Get everything needed
	auto* tilemapSystem = m_Engine.GetTilemapSystem();

	auto* tilemapManager = tilemapSystem->GetTilemapManager();

	auto* tilemap = tilemapManager->GetComponentPtr(2); //TODO a changer dès que Duncan a une meilleur solution
	m_TileExtends = tilemap->GetTileScale();

	const auto tilemapSize = tilemap->GetSize();
	std::cout << "tile size = ( " << m_TileExtends.x << ", " << m_TileExtends.y << ")\n";
	std::cout << "tilemap size = ( " << tilemapSize.x << ", " << tilemapSize.y << ")\n";

	std::vector<std::vector<int>> map;
	map.resize(tilemapSize.x);

	//Loop through tilemap
	for(auto x = 0; x < tilemapSize.x; x++)
	{
		map[x].resize(tilemapSize.y);
		for(auto y = 0; y < tilemapSize.y; y++)
		{
			map[x][y] = NORMAL_COST;
		}
	}
	BuildGraphFromArray(tilemap, map);

#ifdef AI_PATHFINDING_DRAW_NODES
	m_NodesQuads = sf::VertexArray{ sf::Quads, m_Graph.size() * 4 };
	for (auto i = 0; i < m_Graph.size(); i++)
	{
		const auto node = m_Graph[i];
		const auto index = i * 4;

		m_NodesQuads[index + 0].position = node.pos + sf::Vector2f(-10, -10);
		m_NodesQuads[index + 1].position = node.pos + sf::Vector2f(10, -10);
		m_NodesQuads[index + 2].position = node.pos + sf::Vector2f(10, 10);
		m_NodesQuads[index + 3].position = node.pos + sf::Vector2f(-10, 10);

		m_NodesQuads[index + 0].color = sf::Color::White;
		m_NodesQuads[index + 1].color = sf::Color::White;
		m_NodesQuads[index + 2].color = sf::Color::White;
		m_NodesQuads[index + 3].color = sf::Color::White;
	}
#endif

#ifdef AI_PATHFINDING_DRAW_NODES_NEIGHBORS
	m_NodesNeighborsLines.resize(m_Graph.size());
	for (const auto& node : m_Graph)
	{
		const auto nodePos = node.pos;

		for (auto neighborsIndex : node.neighborsIndex)
		{
			const auto nodeNeighbor = m_Graph[neighborsIndex];

			auto line = sf::VertexArray{ sf::Lines, 2 };
			line[0].position = nodePos;
			line[0].color = sf::Color::White;

			line[1].position = nodeNeighbor.pos;
			line[1].color = sf::Color::White;

			m_NodesNeighborsLines.push_back(line);
		}
	}
#endif

}

void NavigationGraphManager::Update(float dt)
{
	//TODO ne pas passer par un nombre fix, mais plutôt alouer un temps maximum et faire un test à chaque fois pour savoir s'il reste suffisament de temps
	for (size_t i = 0; i < m_MaxPathForOneUpdate; i++)
	{
		if (m_WaitingPaths.empty())
		{
			break;
		}

		auto waitingPath = m_WaitingPaths.front();
		m_WaitingPaths.pop();

		//auto tmp = GetPathFromTo(waitingPath.origin, waitingPath.destination);
		//waitingPath.path->assign(tmp.begin(), tmp.end());
	}
}

void NavigationGraphManager::FixedUpdate() {}

void NavigationGraphManager::Draw()
{
	rmt_ScopedCPUSample(NavigationGraphManagerDraw, 0);

	auto window = m_Graphics2DManager->GetWindow();

#ifdef AI_PATHFINDING_DRAW_NODES
	window->draw(m_NodesQuads);
#endif

#ifdef AI_PATHFINDING_DRAW_NODES_NEIGHBORS
	for (const auto& nodesNeighborsLine : m_NodesNeighborsLines)
	{
		window->draw(nodesNeighborsLine);
	}
#endif

}

void NavigationGraphManager::AskForPath(std::vector<Vec2f>* path, const Vec2f origin, const Vec2f destination)
{
	const WaitingPath waitingPath{path, destination, origin};
	m_WaitingPaths.push(waitingPath);
}

void NavigationGraphManager::BuildGraphFromArray(Tilemap* tilemap, std::vector<std::vector<int>>& map)
{
	auto transformManager = m_Engine.GetTransform2dManager();

	for (auto x = 0; x < map.size(); x++)
	{
		for (auto y = 0; y < map[x].size(); y++)
		{
			GraphNode node;
			
			node.cost = map[x][y];
			node.pos = transformManager->GetComponentPtr(tilemap->GetTileAt(Vec2f(x, y)))->Position;

			m_Graph.push_back(node);
		}
	}

	for (size_t y = 0; y < map.size(); y++)
	{
		for (size_t x = 0; x < map[y].size(); x++)
		{
			const int index = y * map.size() + x;

			auto node = m_Graph[index];

			if (node.cost == SOLID_COST)
			{
				continue;
			}

			for (auto i = -1; i < 2; i++)
			{
				for (auto j = -1; j < 2; j++)
				{
					if (i == 0 && j == 0) continue;

					if (y + i < 0 || y + i >= map.size() || x + j < 0 || x + j >= map[y].size()) continue;

					const int indexNeighbor = (y + i) * map.size() + (x + j);

					if (m_Graph[indexNeighbor].cost == SOLID_COST) continue;

					//Add cross without checking other neighbors
					if (i == 0 || j == 0)
					{
						node.neighborsIndex.push_back(indexNeighbor);
					}
					else
					{
						if (m_Graph[y * map.size() + (x + j)].cost != SOLID_COST &&
							m_Graph[(y + i) * map.size() + x].cost != SOLID_COST)
						{
							node.neighborsIndex.push_back(indexNeighbor);
						}
					}
				}
			}
			m_Graph[index] = node;
		}
	}
}

std::vector<Vec2f> NavigationGraphManager::GetPathFromTo(Vec2f& origin, Vec2f& destination)
{
	auto distanceOrigin = INFINITY;
	auto distanceDestination = INFINITY;

	unsigned int indexOrigin = 0, indexDestination = 0;

	for (auto i = 0u; i < m_Graph.size(); i++)
	{
		if (m_Graph[i].cost == SOLID_COST)
		{
			continue;
		}

		const auto dist1 = GetSquaredDistance(origin, m_Graph[i].pos);

		if (dist1 < distanceOrigin)
		{
			distanceOrigin = dist1;
			indexOrigin = i;
		}

		const auto dist2 = GetSquaredDistance(destination, m_Graph[i].pos);

		if (dist2 < distanceDestination)
		{
			distanceDestination = dist2;
			indexDestination = i;
		}
	}

	return GetPathFromTo(indexOrigin, indexDestination);
}

std::vector<Vec2f> NavigationGraphManager::GetPathFromTo(const unsigned int originIndex,
                                                         const unsigned int destinationIndex)
{
	PriorityQueue<unsigned int, float> openNodes;
	openNodes.Put(originIndex, 0);

	std::unordered_map<unsigned int, unsigned int> cameFrom;
	std::unordered_map<unsigned int, short> costSoFar;

	cameFrom[originIndex] = originIndex;
	costSoFar[originIndex] = 0;

	while (!openNodes.Empty())
	{
		const auto indexCurrent = openNodes.Get();

		if (indexCurrent == destinationIndex)
		{
			break;
		}

		for (auto indexNext : m_Graph[indexCurrent].neighborsIndex)
		{
			const auto distance = (m_Graph[indexNext].pos - m_Graph[indexCurrent].pos).GetMagnitude();

			const auto newCost = costSoFar[indexCurrent] + distance;

			if (costSoFar.find(indexNext) == costSoFar.end() ||
				newCost < costSoFar[indexNext])
			{
				costSoFar[indexNext] = newCost;
				//Breaking tie value
				const auto dx1 = std::abs(m_Graph[indexNext].pos.x - m_Graph[destinationIndex].pos.x);
				const auto dy1 = std::abs(m_Graph[indexNext].pos.y - m_Graph[destinationIndex].pos.y);
				const auto dx2 = std::abs(m_Graph[originIndex].pos.x - m_Graph[destinationIndex].pos.x);
				const auto dy2 = std::abs(m_Graph[originIndex].pos.y - m_Graph[destinationIndex].pos.y);
				const auto cross = abs(dx1 * dy2 - dx2 * dy1);

				//Heuristic
				auto heuristic = m_Heuristic1 * (dx1 + dy1) + (m_Heuristic2 - 2 * m_Heuristic1) * std::min(dx1, dy1);
				heuristic += cross * 0.001f;

				const auto priority = newCost + heuristic;
				openNodes.Put(indexNext, priority);

				cameFrom[indexNext] = indexCurrent;
			}
		}
	}

	std::vector<GraphNode> path;

	std::vector<Vec2f> pathPos;
	auto currentNodeIndex = destinationIndex;
	while (currentNodeIndex != originIndex)
	{
		pathPos.push_back(m_Graph[currentNodeIndex].pos);

		path.push_back(m_Graph[currentNodeIndex]);
		currentNodeIndex = cameFrom[currentNodeIndex];
	}
	path.push_back(m_Graph[originIndex]);
	pathPos.push_back(m_Graph[originIndex].pos);

	return pathPos;
}

float NavigationGraphManager::GetSquaredDistance(Vec2f& v1, Vec2f& v2)
{
	return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
}
}
