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

#ifndef BUILDING_CONSTRUCTOR_H
#define BUILDING_CONSTRUCTOR_H

#include <engine/system.h>
#include <python/python_engine.h>

#include <extensions/Building/building_manager.h>
#include <extensions/Building/road_manager.h>
#include "engine/tilemap.h"


namespace sfge::ext
{
	/**
	 * \author Robin Alves
	 */
	class BuildingConstructor : public System
	{
	public:
		BuildingConstructor(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;

		enum BuildingId : TileTypeId
		{
			WAREHOUSE = 3,
			FORGE = 4,
			MINE = 5,
			EXCAVATION_POST = 6,
			MUSHROOM_FARM = 7,
			DWELLING = 8
		};

	private:
		json m_TmpTileMapJson;

		void SetupTileMap();

		Configuration* m_Configuration = nullptr;
		Transform2dManager* m_TransformManager = nullptr;
		BuildingManager* m_BuildingManager = nullptr;
		RoadManager* m_RoadManager = nullptr;

		Tilemap* m_Tilemap = nullptr;
		Vec2f m_SizeTile;
		TilemapManager* m_TilemapManager = nullptr;
		TilemapSystem* m_TilemapSystem = nullptr;

		bool m_Init = false;
	};
}
#endif
