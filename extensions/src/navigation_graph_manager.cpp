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

#include <extensions/navigation_graph_manager.h>

namespace sfge::ext
{
NavigationGraphManager::NavigationGraphManager(Engine& engine) :
	System(engine) {}

void NavigationGraphManager::Init()
{
	m_Graphics2DManager = m_Engine.GetGraphics2dManager();

#ifdef DEBUG_MAP
	std::vector<std::vector<int>> map;

	const int size = 250u;

	for (size_t i = 0; i < size; i++)
	{
		std::vector<int> line;
		line.resize(size);

		for (size_t j = 0; j < size; j++)
		{
			line[j] = NORMAL_COST;
		}
		map.push_back(line);
	}

	for (size_t i = 0; i < size; i++)
	{
		const Vec2f pos(std::rand() % size, std::rand() % size);

		const auto width = std::rand() % 15;
		const auto height = std::rand() % 15;

		for (auto x = 0; x < width; x++)
		{
			for (auto y = 0; y < height; y++)
			{
				if (pos.x + x < size && pos.y + y < size)
				{
					map[pos.x + x][pos.y + y] = SOLID_COST;
				}
			}
		}
	}

	m_VertexArray = sf::VertexArray(sf::Quads, 4 * map.size() * map[0].size());

	m_MapSize = Vec2f(map.size(), map[0].size());

	BuildGraphFromArray(map);
#endif
}

void NavigationGraphManager::Update(float dt)
{
	for (size_t i = 0; i < m_MaxPathForOneUpdate; i++)
	{
		if (m_WaitingPaths.empty())
		{
			break;
		}

		auto waitingPath = m_WaitingPaths.front();
		m_WaitingPaths.pop();

		auto tmp = GetPathFromTo(waitingPath.origin, waitingPath.destination);
		waitingPath.path->assign(tmp.begin(), tmp.end());
	}
}

void NavigationGraphManager::FixedUpdate() {}

void NavigationGraphManager::Draw()
{
	rmt_ScopedCPUSample(NavigationGraphManagerDraw, 0);

	auto window = m_Graphics2DManager->GetWindow();

	window->draw(m_VertexArray);
}

void NavigationGraphManager::AskForPath(std::vector<Vec2f>* path, const Vec2f origin, const Vec2f destination)
{
	const WaitingPath waitingPath{path, destination, origin};
	m_WaitingPaths.push(waitingPath);
}

void NavigationGraphManager::BuildGraphFromArray(std::vector<std::vector<int>>& map)
{
	for (auto y = 0; y < map.size(); y++)
	{
		for (auto x = 0; x < map[y].size(); x++)
		{
			GraphNode node;

			node.cost = map[y][x];
			node.pos = Vec2f(x * m_TileExtends.x + m_TileExtends.x * 0.5f,
			                 y * m_TileExtends.y + m_TileExtends.y * 0.5f);

			// set position
			m_VertexArray[4 * ((y * map.size()) + x)].position = sf::Vector2f(
				node.pos.x - m_TileExtends.x * 0.5f, node.pos.y - m_TileExtends.y * 0.5f);
			m_VertexArray[4 * ((y * map.size()) + x) + 1].position = sf::Vector2f(
				node.pos.x + m_TileExtends.x * 0.5f, node.pos.y - m_TileExtends.y * 0.5f);
			m_VertexArray[4 * ((y * map.size()) + x) + 2].position = sf::Vector2f(
				node.pos.x + m_TileExtends.x * 0.5f, node.pos.y + m_TileExtends.y * 0.5f);
			m_VertexArray[4 * ((y * map.size()) + x) + 3].position = sf::Vector2f(
				node.pos.x - m_TileExtends.x * 0.5f, node.pos.y + m_TileExtends.y * 0.5f);

#ifndef DEBUG_MOD
			if (node.cost == SOLID_COST)
			{
				// set colors
				m_VertexArray[4 * ((y * map.size()) + x)].color = sf::Color::Red;
				m_VertexArray[4 * ((y * map.size()) + x) + 1].color = sf::Color::Red;
				m_VertexArray[4 * ((y * map.size()) + x) + 2].color = sf::Color::Red;
				m_VertexArray[4 * ((y * map.size()) + x) + 3].color = sf::Color::Red;
			}
			else
			{
				// set colors
				m_VertexArray[4 * ((y * map.size()) + x)].color = sf::Color::White;
				m_VertexArray[4 * ((y * map.size()) + x) + 1].color = sf::Color::White;
				m_VertexArray[4 * ((y * map.size()) + x) + 2].color = sf::Color::White;
				m_VertexArray[4 * ((y * map.size()) + x) + 3].color = sf::Color::White;
			}
#endif
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
#ifdef DEBUG_MOD
	for (auto y = 0; y < m_MapSize.y; y++) {
		for (auto x = 0; x < m_MapSize.x; x++) {
			GraphNode node = m_Graph[((y * m_MapSize.y) + x)];

			if (originIndex == ((y * m_MapSize.y) + x)) {
				m_VertexArray[4 * ((y * m_MapSize.y) + x)].color = sf::Color::Green;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 1].color = sf::Color::Green;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 2].color = sf::Color::Green;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 3].color = sf::Color::Green;
			}
			else if (destinationIndex == ((y * m_MapSize.y) + x)) {
				m_VertexArray[4 * ((y * m_MapSize.y) + x)].color = sf::Color::Magenta;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 1].color = sf::Color::Magenta;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 2].color = sf::Color::Magenta;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 3].color = sf::Color::Magenta;
			}
			else if (node.cost == SOLID_COST) {
				m_VertexArray[4 * ((y * m_MapSize.y) + x)].color = sf::Color::Red;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 1].color = sf::Color::Red;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 2].color = sf::Color::Red;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 3].color = sf::Color::Red;
			}
			else {
				m_VertexArray[4 * ((y * m_MapSize.y) + x)].color = sf::Color::White;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 1].color = sf::Color::White;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 2].color = sf::Color::White;
				m_VertexArray[4 * ((y * m_MapSize.y) + x) + 3].color = sf::Color::White;
			}
		}
	}
#endif

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

#ifdef DEBUG_MOD
		m_VertexArray[4 * indexCurrent].color = sf::Color::Yellow;
		m_VertexArray[4 * indexCurrent + 1].color = sf::Color::Yellow;
		m_VertexArray[4 * indexCurrent + 2].color = sf::Color::Yellow;
		m_VertexArray[4 * indexCurrent + 3].color = sf::Color::Yellow;
#endif
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

#ifdef DEBUG_MOD
		m_VertexArray[4 * currentNodeIndex].color = sf::Color::Cyan;
		m_VertexArray[4 * currentNodeIndex + 1].color = sf::Color::Cyan;
		m_VertexArray[4 * currentNodeIndex + 2].color = sf::Color::Cyan;
		m_VertexArray[4 * currentNodeIndex + 3].color = sf::Color::Cyan;
#endif
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
