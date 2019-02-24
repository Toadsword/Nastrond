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
#ifndef NAVIGATION_GRAPH_MANAGER_H
#define NAVIGATION_GRAPH_MANAGER_H

#include <vector>

#include <graphics/graphics2d.h>

#include <engine/system.h>
#include <engine/vector.h>


namespace sfge::ext
{
/**
 * \author Nicolas Schneider
 */
struct GraphNode final
{
	float cost;
	std::vector<unsigned int> neighborsIndex;
	Vec2f pos;
};

//#define DEBUG_MOD
#define DEBUG_MAP

/**
 * \author Nicolas Schneider
 */
class NavigationGraphManager final : public System
{
public:
	explicit NavigationGraphManager(Engine& engine);

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

	/**
	 * \brief Ask a path from the origin to a destination, store datas to be used when the path finder is free
	 * \param path 
	 * \param origin 
	 * \param destination 
	 */
	void AskForPath(std::vector<Vec2f>* path, Vec2f origin, Vec2f destination);

private:
	std::vector<Vec2f> GetPathFromTo(Vec2f& origin, Vec2f& destination);
	std::vector<Vec2f> GetPathFromTo(unsigned int originIndex, unsigned int destinationIndex);

	void BuildGraphFromArray(std::vector<std::vector<int>>& map);

	static float GetSquaredDistance(Vec2f& v1, Vec2f& v2);

	Graphics2dManager* m_Graphics2DManager;

	//Queue for waiting path
	struct WaitingPath final
	{
		std::vector<Vec2f>* path;
		Vec2f destination;
		Vec2f origin;
	};

	std::queue<WaitingPath> m_WaitingPaths;

	//Heuristic for pathfinding
	const float m_Heuristic1 = 1;
	const float m_Heuristic2 = sqrt(2.f);

	//Constant for cost
	const static short SOLID_COST = 0;
	const static short ROAD_COST = 1;
	const static short NORMAL_COST = 2;

	const int m_MaxPathForOneUpdate = 100'000;

	std::vector<GraphNode> m_Graph;
	sf::VertexArray m_VertexArray;

#ifdef DEBUG_MAP
	//Map info
	const Vec2f m_TileExtends = Vec2f(60, 60);
	Vec2f m_MapSize;
#endif
};
}

#endif
