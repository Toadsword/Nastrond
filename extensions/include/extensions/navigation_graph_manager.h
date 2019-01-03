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
#include "SFML/System/Vector2.hpp"
#include "engine/vector.h"
#include "graphics/graphics2d.h"

namespace sfge::ext
{
struct GraphNode {
	short cost;
	std::vector<int> neighborsIndex;
	Vec2f pos;

	const static int SOLID_COST = 0;
	const static int ROAD_COST = 1;
	const static int NORMAL_COST = 2;
};

class NavigationGraphManager : public System {
public:
	NavigationGraphManager(Engine& engine);

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

private:
	void BuildGraphFromArray(const std::vector<std::vector<int>> map);

	std::vector<GraphNode> m_Graph;

	Graphics2dManager* m_Graphics2DManager;
};
}

#endif

