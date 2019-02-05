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

 /*******************************
  * Author : Duncan Bourquard
  * Date : 05.02.2019
  */

#ifndef SFGE_TILE_H_
#define SFGE_TILE_H_

#include <engine/component.h>
#include <engine/transform2d.h>
#include <engine/tile_asset.h>

namespace sfge
{
class Tilemap;
class TilemapManager;

/**
 * \brief Tile class. Represent a single square of a Tilemap
 * \author Duncan Bourquard
 */
class Tile : public LayerComponent
{
public:
	Tile();

	void Update();

	void SetParentTilemap(Entity parent);
	Entity GetParentTilemap();

	void SetLayer(int newLayer);
	int GetLayer();

	void SetType(TileTypeId newType);
	TileTypeId GetType();

protected:
	int m_Layer = -1;
	TileTypeId m_Type = INVALID_TILE_TYPE;
	Entity m_ParentTilemapEntity = INVALID_ENTITY;
};

namespace editor
{
	/**
	 * \brief ComponentInfo of Tile class.
	 * \author Duncan Bourquard
	 */
	struct TileInfo : ComponentInfo
	{
		void DrawOnInspector() override;
		Tile* tile = nullptr;
	};
}

/**
 * \brief Manager of Tile class.
 * \author Duncan Bourquard
 */
class TileManager :
	public SingleComponentManager<Tile, editor::TileInfo, ComponentType::TILE>
{
public:
	using SingleComponentManager::SingleComponentManager;
	void Init() override;
	void Update(float dt) override;

	void Collect() override;
	Tile* AddComponent(Entity entity) override;
	Tile* AddComponent(Entity entity, TileTypeId tileType);
	void CreateComponent(Entity entity, TileTypeId tileType);
	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;
	void OnResize(size_t new_size) override;

protected:
	Transform2dManager* m_Transform2dManager = nullptr;
	TilemapManager* m_TilemapManager = nullptr;
};	
}
#endif