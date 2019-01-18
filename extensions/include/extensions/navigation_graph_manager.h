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

#include "engine/system.h"
#include <vector>
#include "engine/vector.h"
#include "graphics/graphics2d.h"

namespace sfge::ext
{
	/**
	 * \author Nicolas Schneider
	 */
	struct GraphNode {
		float cost;
		std::vector<unsigned int> neighborsIndex;
		Vec2f pos;
	};

//#define DEBUG_MOD
#define DEBUG_MAP
/**
 * \author Nicolas Schneider
 */
	class NavigationGraphManager : public System {
	public:
		NavigationGraphManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		std::vector<Vec2f> GetPathFromTo(Vec2f& origin, Vec2f& destination);
		std::vector<Vec2f> GetPathFromTo(unsigned int originIndex, unsigned int destinationIndex);

	private:
		void BuildGraphFromArray(std::vector<std::vector<int>>& map);

		static float GetSquaredDistance(Vec2f& v1, Vec2f& v2);

		void DrawQuad(sf::RenderWindow* window, Vec2f pos, sf::Color col);

		Graphics2dManager* m_Graphics2DManager;

		//Heuristic for pathfinding
		const float HEURISTIC_1 = 1;
		const float HEURISTIC_2 = sqrt(2);

		//Constant for cost
		const static short SOLID_COST = 0;
		const static short ROAD_COST = 1;
		const static short NORMAL_COST = 1;

		
		std::vector<GraphNode> m_Graph;
		sf::VertexArray vertexArray;

#ifdef DEBUG_MAP
		//Map info
		const Vec2f m_tileExtends = Vec2f(6, 6);
		Vec2f m_mapSize;
#endif
	};
}

#endif