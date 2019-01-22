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
  * Date : 16.01.2019
  */

#ifndef SFGE_TILEMAP_H_
#define SFGE_TILEMAP_H_

 //tool_engine
#include <engine/component.h>
#include <engine/transform2d.h>
#include <engine/tile.h>
#include <engine/tile_asset.h>

namespace sfge
{
class Tilemap
{
public:
	Tilemap();

	void Init(TilemapSystem* tilemapSystem);
	void Update();

	void SetSize(sf::Vector2<unsigned> newSize);
	sf::Vector2<unsigned> GetSize();
	void SetLayer(int newLayer);
	int GetLayer();
	std::vector<std::vector<Entity>>& GetTiles();

	void AddTile(Vec2f pos, Entity entity);

protected:
	std::vector<std::vector<Entity>> m_Tiles;
	int m_Layer = 0;
	sf::Vector2<unsigned> m_Size = {100, 100};

	TilemapSystem* m_TilemapSystem;
};


namespace editor
{
struct TilemapInfo : ComponentInfo
{
	void DrawOnInspector() override;
	Tilemap* tilemap = nullptr;
	int layer = 0;
	sf::Vector2<unsigned> size = { 100, 100 };
};
}

class TilemapManager :
	public SingleComponentManager<Tilemap, editor::TilemapInfo, ComponentType::TILEMAP>
{
public:
	using SingleComponentManager::SingleComponentManager;
	void Init() override;
	void Update(float dt) override;

	void Clear();
	void Collect() override;
	Tilemap* AddComponent(Entity entity) override;
	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;
	void OnResize(size_t new_size) override;

	void InitializeMap(Entity entity, json& map);
	void EmptyMap(Entity entity);
protected:
	Transform2dManager* m_Transform2dManager = nullptr;
	TileManager* m_TileManager = nullptr;
};


class TilemapSystem: System
{
public:
	using System::System;

	/**
		* \brief Initialize the Tilemap Manager
		*/
	void Init() override;

	/**
		* \brief Update the Tilemap Manager
		* \param dt Delta time since last frame
		*/
	void Update(float dt) override;

	void FixedUpdate() override;

	/**
	* \brief Destroy the window and other
	*/
	void Destroy() override;

	void Clear() override;

	void Collect() override;

	TilemapManager* GetTilemapManager();
	TileManager* GetTileManager();
	TileTypeManager* GetTileTypeManager();

protected:
	TilemapManager m_TilemapManager { m_Engine };
	TileManager m_TileManager { m_Engine };
	TileTypeManager m_TileTypeManager { m_Engine };
};
}

#endif /* INCLUDE_TILEMAP_H_ */
