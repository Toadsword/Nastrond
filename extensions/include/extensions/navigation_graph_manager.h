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
	short cost;
	std::vector<int> neighborsIndex;
	Vec2f pos;

	const static int SOLID_COST = 0;
	const static int ROAD_COST = 1;
	const static int NORMAL_COST = 2;
};

struct GraphNodeDebug : GraphNode {
	enum State {
		VISITED,
		START_POSITION,
		END_POSITION,
		PASSAGE_POSITION,
		OTHER,
	};

	State state;
};

#define DEBUG_MOD

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

	std::vector<Vec2f> GetPathFromTo(Vec2f origin, Vec2f destination);

private:
	void BuildGraphFromArray(const std::vector<std::vector<int>> map);

#if defined DEBUG_MOD
	std::vector<GraphNodeDebug> m_Graph;
#else
	std::vector<GraphNode> m_Graph;
#endif

	Graphics2dManager* m_Graphics2DManager;

	float GetSquaredDistance(Vec2f v1, Vec2f v2) const;
	float ComputeHeuristic(short index1, short index2) const;
};
}

#endif

