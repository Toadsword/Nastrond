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

#include <extensions/Building/building_constructor.h>
#include <iostream>

namespace sfge::ext
{
	BuildingConstructor::BuildingConstructor(Engine& engine) : System(engine) {}

	void BuildingConstructor::Init()
	{
		m_BuildingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<BuildingManager>(
			"BuildingManager");
		m_RoadManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<RoadManager>(
			"RoadManager");

		SetupTileMap();

		Log::GetInstance()->Msg("Building Constructor initialized");
	}

	void BuildingConstructor::Update(float dt)
	{

	}

	void BuildingConstructor::FixedUpdate()
	{

	}

	void BuildingConstructor::Draw()
	{

	}
	void BuildingConstructor::SetupTileMap()
	{
		m_TmpTileMapJson["map"] = json::array(
			{
			json::array({1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1}),
			json::array({1, 3, 3, 1, 5, 1, 1, 3, 1, 1, 1, 3, 3, 1, 5, 1, 1, 3, 1, 1}),
			json::array({1, 3, 3, 1, 3, 3, 3, 1, 1, 1, 1, 3, 3, 1, 3, 3, 1, 1, 1, 1}),
			json::array({1, 1, 5, 1, 3, 3, 3, 1, 1, 1, 1, 1, 5, 1, 3, 1, 1, 1, 1, 1}),
			json::array({1, 1, 3, 3, 3, 1, 3, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 3, 1, 3, 3, 3, 6, 1, 1, 1, 1, 3, 1, 1, 3, 1, 6, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 3, 1, 3, 3, 3, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1}),
			json::array({1, 3, 3, 1, 5, 1, 1, 3, 1, 1, 1, 3, 3, 1, 5, 1, 1, 3, 1, 1}),
			json::array({1, 3, 3, 1, 3, 3, 1, 1, 1, 1, 1, 3, 3, 1, 3, 3, 1, 1, 1, 1}),
			json::array({1, 1, 5, 1, 3, 1, 1, 1, 1, 1, 1, 1, 5, 1, 3, 1, 1, 1, 1, 1}),
			json::array({1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 3, 1, 1, 1, 1, 6, 1, 1, 1, 1, 3, 1, 1, 1, 1, 6, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1})
			});
		std::vector<int> jsonArray;
		for (int y = 0; y < m_TmpTileMapJson["map"].size(); ++y)
		{
			for (int x = 0; x < m_TmpTileMapJson["map"][y].size(); ++x)
			{
				jsonArray.push_back(m_TmpTileMapJson["map"][y][x]);
			}
		}

		m_RoadManager->SpawnRoad(jsonArray, m_TmpTileMapJson["map"][0].size(), Vec2f(0, 0), Vec2f(64, 32), 1, 3);


		for (int y = 0; y < m_TmpTileMapJson["map"].size(); y++)
		{
			for (int x = 0; x < m_TmpTileMapJson["map"][y].size(); x++)
			{
				float decalX = 64;
				float decalY = 16;

				
				if(y % 2 == 1)
				{
					decalX = x * decalX - (decalX / 2);
					std::cout << std::to_string(decalX) + " \n";
				}
				else
				{
					decalX = x * decalX;
				}

				if(m_TmpTileMapJson["map"][y][x] == 4)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::WAREHOUSE, Vec2f(decalX, y * decalY));
				}
				else if(m_TmpTileMapJson["map"][y][x] == 5)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::DWELLING, Vec2f(decalX, y * decalY));
				}
				else if(m_TmpTileMapJson["map"][y][x] == 6)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::FORGE, Vec2f(decalX, y * decalY));
				}
			}

		}
	}
}
