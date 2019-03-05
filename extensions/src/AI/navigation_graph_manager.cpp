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
#include <graphics/tilemap.h>

#include <extensions/dwarf_manager.h>

namespace sfge::ext
{
NavigationGraphManager::NavigationGraphManager(Engine& engine) :
	System(engine) {}

NavigationGraphManager::~NavigationGraphManager()
{
#ifdef AI_DEBUG_COUNT_TIME
	std::cout << "[NavigationGraphManager]Update: " << m_TimerMilli / m_TimerCounter << "," << m_TimerMicro /
		m_TimerCounter << "\n";
#endif
#ifdef AI_PATH_FINDING_DEBUG_COUNT_TIME_PRECISE
	std::cout << "		GetNode: " << m_TmpGetNode_Ms / m_TimerCounter / 1000 << "," << m_TmpGetNode_Mc / m_TimerCounter % 1000<< "\n";
	std::cout << "		FindPath: " << m_TmpFindPath_Ms / m_TimerCounter / 1000 << "," << m_TmpFindPath_Mc / m_TimerCounter % 1000<< "\n";
#endif
}

void NavigationGraphManager::Init()
{
	m_Graphics2DManager = m_Engine.GetGraphics2dManager();
	m_DwarfManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<DwarfManager>(
		"DwarfManager");

	//Get everything needed
	auto* tilemapSystem = m_Engine.GetGraphics2dManager()->GetTilemapSystem();

	auto* tilemapManager = tilemapSystem->GetTilemapManager();

	auto* tilemap = tilemapManager->GetComponentPtr(2); 
	m_TileExtends = tilemap->GetTileSize();

	const auto tilemapSize = tilemap->GetTilemapSize();

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
	
#ifdef AI_PATH_FINDING_DRAW_NODES
	m_NodesQuads = sf::VertexArray{ sf::Quads, m_Graph.size() * 4 };
	for (auto i = 0; i < m_Graph.size(); i++)
	{
		const auto node = m_Graph[i];
		const auto index = i * 4;

		m_NodesQuads[index + 0].position = node.pos + sf::Vector2f(-tilemapSize.x, 0);
		m_NodesQuads[index + 1].position = node.pos + sf::Vector2f(0, -tilemapSize.y * 0.5f);
		m_NodesQuads[index + 2].position = node.pos + sf::Vector2f(tilemapSize.x, 0);
		m_NodesQuads[index + 3].position = node.pos + sf::Vector2f(0, tilemapSize.y * 0.5f);

		if (node.cost == SOLID_COST) {
			m_NodesQuads[index + 0].color = sf::Color::Red;
			m_NodesQuads[index + 1].color = sf::Color::Red;
			m_NodesQuads[index + 2].color = sf::Color::Red;
			m_NodesQuads[index + 3].color = sf::Color::Red;
		}
		else {
			m_NodesQuads[index + 0].color = sf::Color::White;
			m_NodesQuads[index + 1].color = sf::Color::White;
			m_NodesQuads[index + 2].color = sf::Color::White;
			m_NodesQuads[index + 3].color = sf::Color::White;
		}
	}
#endif

#ifdef AI_PATH_FINDING_DRAW_NODES_NEIGHBORS
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

	m_CameFrom.resize(m_Graph.size(), 0);
	m_CostSoFar.resize(m_Graph.size(), -1);
}

void NavigationGraphManager::Update(float dt)
{
	//TODO ne pas passer par un nombre fix, mais plutot alouer un temps maximum et faire un test a chaque fois pour savoir s'il reste suffisament de temps
#ifdef AI_DEBUG_COUNT_TIME
	auto t1 = std::chrono::high_resolution_clock::now();
#endif
	for (size_t i = 0; i < m_MaxPathForOneUpdate; i++)
	{
		if (m_WaitingPaths.empty())
		{
			break;
		}

#ifdef AI_PATH_FINDING_DRAW_DEBUG_NODES
		for(auto i = 0; i < m_Graph.size(); i++)
		{
			const auto node = m_Graph[i];
			const auto index = i * 4;

			if (node.cost == SOLID_COST) {
				m_NodesQuads[index + 0].color = sf::Color::Red;
				m_NodesQuads[index + 1].color = sf::Color::Red;
				m_NodesQuads[index + 2].color = sf::Color::Red;
				m_NodesQuads[index + 3].color = sf::Color::Red;
			}
			else {
				m_NodesQuads[index + 0].color = sf::Color::White;
				m_NodesQuads[index + 1].color = sf::Color::White;
				m_NodesQuads[index + 2].color = sf::Color::White;
				m_NodesQuads[index + 3].color = sf::Color::White;
			}
		}
#endif

		auto waitingPath = m_WaitingPaths.front();
		m_WaitingPaths.pop();

		const auto path = GetPathFromTo(waitingPath.origin, waitingPath.destination);
		m_DwarfManager->SetPath(waitingPath.index, path);
#ifdef  AI_PATH_FINDING_DEBUG_COUNT_TIME_PRECISE
		m_PathCalculated++;
#endif
	}

#ifdef AI_DEBUG_COUNT_TIME
	auto t2 = std::chrono::high_resolution_clock::now();
	const auto timerDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	m_TimerMilli += timerDuration / 1000;
	m_TimerMicro += timerDuration % 1000;
	m_TimerCounter++;
#endif
}

void NavigationGraphManager::FixedUpdate() {}

void NavigationGraphManager::Draw()
{
	rmt_ScopedCPUSample(NavigationGraphManagerDraw, 0);

	auto window = m_Graphics2DManager->GetWindow();

#ifdef AI_PATH_FINDING_DRAW_NODES
	window->draw(m_NodesQuads);
#endif

#ifdef AI_PATH_FINDING_DRAW_NODES_NEIGHBORS
	for (const auto& nodesNeighborsLine : m_NodesNeighborsLines)
	{
		window->draw(nodesNeighborsLine);
	}
#endif

}

void NavigationGraphManager::AskForPath(const unsigned int index, const Vec2f origin, const Vec2f destination)
{
	const WaitingPath waitingPath{index, destination, origin};
	m_WaitingPaths.push(waitingPath);
}

void NavigationGraphManager::BuildGraphFromArray(Tilemap* tilemap, std::vector<std::vector<int>>& map)
{
	std::vector<TileTypeId> tileTypes = tilemap->GetTileTypes();
	
	for (auto x = 0; x < map.size(); x++)
	{
		for (auto y = 0; y < map[x].size(); y++)
		{
			GraphNode node;
			
			const auto tile = tileTypes[y * map.size() + x];
			auto offset = Vec2f(0, 0);
			const auto TILE_SIZE = Vec2f(64, 32);
			Vec2f xPos = { TILE_SIZE.x / 2.0f, TILE_SIZE.y / 2.0f };
			Vec2f yPos = { -TILE_SIZE.x / 2.0f, TILE_SIZE.y / 2.0f };

			offset = xPos * x + yPos * y;
			
			if (tile > 2) {
				node.cost = SOLID_COST;
			}else
			{
				node.cost = NORMAL_COST;
			}
			node.pos = xPos * x + yPos * y;

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
					//Skip self
					if (i == 0 && j == 0) continue;

					//Skip if outside of bounds
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
						node.neighborsIndex.push_back(indexNeighbor);
					}
				}
			}
			m_Graph[index] = node;
		}
	}
}

std::vector<Vec2f> NavigationGraphManager::GetPathFromTo(Vec2f& origin, Vec2f& destination)
{
#ifdef  AI_PATH_FINDING_DEBUG_COUNT_TIME_PRECISE
	const auto t1 = std::chrono::high_resolution_clock::now();
#endif
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
#ifdef  AI_PATH_FINDING_DEBUG_COUNT_TIME_PRECISE
	const auto t2 = std::chrono::high_resolution_clock::now();

	const auto timerDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	m_TmpGetNode_Ms += timerDuration;
	m_TmpGetNode_Mc += timerDuration;
#endif

	return GetPathFromTo(indexOrigin, indexDestination);
}

float Sqrt(float x) {
	auto i = *reinterpret_cast<int*>(&x);
	i = 0x5f3759df - (i >> 1);
	auto r = *reinterpret_cast<float*>(&i);
	r = r * (1.5f - 0.5f*x*r*r);
	return r * x;
}

std::vector<Vec2f> NavigationGraphManager::GetPathFromTo(const unsigned int originIndex,
                                                         const unsigned int destinationIndex)
{
#ifdef  AI_PATH_FINDING_DEBUG_COUNT_TIME_PRECISE
	const auto t1 = std::chrono::high_resolution_clock::now();
#endif
#ifdef AI_PATH_FINDING_DRAW_DEBUG_NODES
	int nb = 0;
#endif

	HeapPriorityQueue openNodes;
	openNodes.Insert(originIndex, 0);

	m_CameFrom[originIndex] = originIndex;
	m_CostSoFar[originIndex] = 0;

	const auto posX = m_Graph[destinationIndex].pos.x;
	const auto posY = m_Graph[destinationIndex].pos.y;

	const auto dx2 = std::abs(m_Graph[originIndex].pos.x - posX);
	const auto dy2 = std::abs(m_Graph[originIndex].pos.y - posY);

	while (!openNodes.Empty())
	{
		const auto indexCurrent = openNodes.Min();
		openNodes.RemoveMin();

		if (indexCurrent == destinationIndex)
		{
			break;
		}

		for (auto indexNext : m_Graph[indexCurrent].neighborsIndex)
		{
#ifdef AI_PATH_FINDING_DRAW_DEBUG_NODES
			const auto node = m_Graph[indexNext];
			const auto indexVertex = indexNext * 4;

			m_NodesQuads[indexVertex + 0].color = sf::Color::Yellow;
			m_NodesQuads[indexVertex + 1].color = sf::Color::Yellow;
			m_NodesQuads[indexVertex + 2].color = sf::Color::Yellow;
			m_NodesQuads[indexVertex + 3].color = sf::Color::Yellow;
#endif
			const auto distance = (m_Graph[indexNext].pos - m_Graph[indexCurrent].pos);

			const auto newCost = m_CostSoFar[indexCurrent] + Sqrt(distance.x * distance.x + distance.y * distance.y);

			if (m_CostSoFar[indexNext] == 100'000 ||
				newCost < m_CostSoFar[indexNext])
			{
				m_CostSoFar[indexNext] = newCost;
				//Breaking tie value
				const auto dx1 = std::abs(m_Graph[indexNext].pos.x - posX);
				const auto dy1 = std::abs(m_Graph[indexNext].pos.y - posY);
				const auto cross = abs(dx1 * dy2 - dx2 * dy1);

				//Heuristic
				auto heuristic = m_Heuristic1 * (dx1 + dy1) + (m_Heuristic2 - 2 * m_Heuristic1) * std::min(dx1, dy1);
				heuristic += cross * 0.001f;

				const auto priority = newCost + heuristic;
				openNodes.Insert(indexNext, priority);

				m_CameFrom[indexNext] = indexCurrent;
			}
		}
	}

	std::vector<Vec2f> pathPos;
	auto currentNodeIndex = destinationIndex;
	
	while (currentNodeIndex != originIndex)
	{
		pathPos.push_back(m_Graph[currentNodeIndex].pos);

		currentNodeIndex = m_CameFrom[currentNodeIndex];
	}
	pathPos.push_back(m_Graph[originIndex].pos);

#ifdef AI_PATH_FINDING_DRAW_DEBUG_NODES
	std::cout << "Tested = " << nb << "\n";
#endif
#ifdef  AI_PATH_FINDING_DEBUG_COUNT_TIME_PRECISE

	const auto t2 = std::chrono::high_resolution_clock::now();

	const auto timerDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
#ifdef AI_PATH_FINDING_DRAW_DEBUG_NODES
	std::cout << "Time path finding = " << timerDuration << "\n";
#endif
	m_TmpFindPath_Ms += timerDuration;
	m_TmpFindPath_Mc += timerDuration;
#endif

	for (auto& i : m_CostSoFar)
	{
		i = 100'000;
	}

	return pathPos;
}

float NavigationGraphManager::GetSquaredDistance(Vec2f& v1, Vec2f& v2)
{
	return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
}
}
