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

		//Get directly the tilemap (only way to make it work)
		m_Tilemap = m_TilemapManager->GetComponentPtr(2);

		if (m_Tilemap != nullptr)
		{
			m_SizeTile = m_Tilemap->GetTileSize();
			SetupTileMap();
		}
			std::cout << "Setup Tilema\n";

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

		std::vector<TileTypeId> &tileTypes = m_Tilemap->GetTileTypes();

		m_RoadManager->SpawnRoad(tileTypes, tilemapSize.x, tilemapSize.y, m_Tilemap->GetTilePosition(Vec2f(0, 0)), m_SizeTile, 2);

		Vec2f position;

		for (int y = 0; y < tilemapSize.y; y++)
		{
			for (int x = 0; x < tilemapSize.x; x++)
			{
				
				/////////////////OLD VERSION/////////////////
				TileTypeId currentTileId = tileTypes[y * tilemapSize.x + x];

				Vec2f xPos = { m_SizeTile.x / 2.0f, m_SizeTile.y / 2.0f };
				Vec2f yPos = { -m_SizeTile.x / 2.0f, m_SizeTile.y / 2.0f };


				position = m_Tilemap->GetTilePosition(Vec2f(x, y));


				/////////////////NEW VERSION/////////////////
				//TileTypeId currentTileId = m_Tilemap->GetTileAt(Vec2f(x, y));

				//position = m_Tilemap->GetTilePosition(Vec2f(x, y));

				switch (currentTileId)
				{
				case BuildingId::WAREHOUSE:
					m_BuildingManager->SpawnBuilding(BuildingType::WAREHOUSE, position);
					break;

				case BuildingId::FORGE:
					m_BuildingManager->SpawnBuilding(BuildingType::FORGE, position);
					break;

				case BuildingId::MINE:
					m_BuildingManager->SpawnBuilding(BuildingType::MINE, position);
					break;

				case BuildingId::EXCAVATION_POST:
					m_BuildingManager->SpawnBuilding(BuildingType::EXCAVATION_POST, position);
					break;

				case BuildingId::MUSHROOM_FARM:
					m_BuildingManager->SpawnBuilding(BuildingType::MUSHROOM_FARM, position);
					break;

				case BuildingId::DWELLING:
					m_BuildingManager->SpawnBuilding(BuildingType::DWELLING, position);
					break;
				}

			}

		}
	}
}
