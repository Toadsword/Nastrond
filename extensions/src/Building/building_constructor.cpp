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
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 6, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
			json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1})
			});

		std::vector<int> jsonArray;
		const Vec2f TILE_SIZE = Vec2f(64, 32);
		Vec2f offset = Vec2f(0, 0);

		for (int y = 0; y < m_TmpTileMapJson["map"].size(); ++y)
		{
			for (int x = 0; x < m_TmpTileMapJson["map"][y].size(); ++x)
			{
				jsonArray.push_back(m_TmpTileMapJson["map"][y][x]);
			}
		}

		m_RoadManager->SpawnRoad(jsonArray, m_TmpTileMapJson["map"][0].size(), m_TmpTileMapJson["map"].size(), Vec2f(0, 0), Vec2f(64, 32), 1);

		for (int y = 0; y < m_TmpTileMapJson["map"].size(); y++)
		{
			for (int x = 0; x < m_TmpTileMapJson["map"][y].size(); x++)
			{
				Vec2f xPos = { TILE_SIZE.x / 2.0f, TILE_SIZE.y / 2.0f };
				Vec2f yPos = { -TILE_SIZE.x / 2.0f, TILE_SIZE.y / 2.0f };

				offset = xPos * x + yPos * y;

				if(m_TmpTileMapJson["map"][y][x] == 4)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::WAREHOUSE, offset);
				}
				else if(m_TmpTileMapJson["map"][y][x] == 5)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::DWELLING, offset);
				}
				else if(m_TmpTileMapJson["map"][y][x] == 6)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::FORGE, offset);
				}
				else if (m_TmpTileMapJson["map"][y][x] == 7)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::MINE, offset);
				}
				else if (m_TmpTileMapJson["map"][y][x] == 8)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::EXCAVATION_POST, offset);
				}
				else if (m_TmpTileMapJson["map"][y][x] == 9)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::MUSHROOM_FARM, offset);
				}

			}

		}
	}
}
