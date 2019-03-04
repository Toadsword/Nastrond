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
		m_TransformManager = m_Engine.GetTransform2dManager();
		m_RoadManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<RoadManager>(
			"RoadManager");

		m_Configuration = m_Engine.GetConfig();

		m_TilemapSystem = m_Engine.GetGraphics2dManager()->GetTilemapSystem();

		m_TilemapManager = m_TilemapSystem->GetTilemapManager();


		// TODO : Crash if the entity checked haven't a tilemap component
		//for(int i = 1; i <= m_Configuration->currentEntitiesNmb; i++)
		//{
		//	m_Tilemap = m_TilemapManager->GetComponentPtr(i);
		//	if(m_Tilemap != nullptr)
		//		break;
		//}

		//Get directly the tilemap (only way it worked)
		m_Tilemap = m_TilemapManager->GetComponentPtr(2);

		if (m_Tilemap != nullptr)
		{
			m_SizeTile = m_Tilemap->GetTileSize();
			SetupTileMap();
		}

		m_Init = true;
		Log::GetInstance()->Msg("Building Constructor initialized");
	}

	void BuildingConstructor::Update(float dt)
	{
		if (!m_Init)
		{
			Init();
		}
	}

	void BuildingConstructor::FixedUpdate()
	{

	}

	void BuildingConstructor::Draw()
	{

	}
	void BuildingConstructor::SetupTileMap()
	{
		Vec2f tilemapSize = m_Tilemap->GetTilemapSize();

		std::vector<TileTypeId> tileTypes = m_Tilemap->GetTileTypes();

		std::vector<int> jsonArray;
		const Vec2f TILE_SIZE = Vec2f(64, 32);
		Vec2f offset = Vec2f(0, 0);

		m_RoadManager->SpawnRoad(m_Tilemap->GetTileTypes(), tilemapSize.x, tilemapSize.y, m_TransformManager->GetComponentPtr(m_Tilemap->GetTileAt(Vec2f(0, 0)))->Position, m_SizeTile, 2);

		for (int y = 0; y < tilemapSize.y; y++)
		{
			for (int x = 0; x < tilemapSize.x; x++)
			{
				TileTypeId currentTileId = tileTypes[y * tilemapSize.y + x];
				Vec2f xPos = { TILE_SIZE.x / 2.0f, TILE_SIZE.y / 2.0f };
				Vec2f yPos = { -TILE_SIZE.x / 2.0f, TILE_SIZE.y / 2.0f };

				offset = xPos * x + yPos * y;

				if(currentTileId == BuildingId::WAREHOUSE)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::WAREHOUSE, offset);
				}
				else if(currentTileId == BuildingId::FORGE)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::FORGE, offset);
				}
				else if (currentTileId == BuildingId::MINE)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::MINE, offset);
				}
				else if (currentTileId == BuildingId::EXCAVATION_POST)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::EXCAVATION_POST, offset);
				}
				else if (currentTileId == BuildingId::MUSHROOM_FARM)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::MUSHROOM_FARM, offset);
				}
				else if (currentTileId == BuildingId::DWELLING)
				{
					m_BuildingManager->SpawnBuilding(BuildingType::DWELLING, offset);
				}

			}

		}
	}
}
