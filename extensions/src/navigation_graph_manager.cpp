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
	}

	void NavigationGraphManager::BuildGraphFromArray(std::vector<std::vector<int>> map)
	{
		for(int y = 0; y < map.size(); y++) {
			std::string s;
			for(int x = 0; x < map.at(y).size(); x++) {
				s += std::to_string(map.at(y).at(x));
			}
			std::cout << s + "\n";
		}
	}
}


