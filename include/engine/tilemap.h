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

#define Opti1

namespace sfge
{
/**
 * \author : Duncan Bourquard
 * \version : 1.0
 * \date : 30.01.2019
 */

class Tilemap
{
public:
	/**
	 * \brief Constructor of tilemap.
	 */
	Tilemap();

	/**
	 * \brief Init the tilemap.
	 * \param tileManager Pointer to the Tilemanager created in the engine.
	 */
	void Init(TileManager* tileManager);
	
	/**
	 * \brief Update the tilemap and all the tiles within.
	 */
	void Update();

	json Save();
	
	/**
	 * \brief Getter of the size of the tilemap.
	 * \return the size of the tilemap.
	 */
	Vec2f GetTilemapSize();

	void SetTileSize(Vec2f newSize);
	Vec2f GetTileSize();

	void SetLayer(int newLayer);
	int GetLayer();

	void SetIsometric(bool newIso);
	bool GetIsometric();

#ifdef Opti1
	/**
	 * \brief Set all the tile types passed in parameter to the tilemap. Usually used to "paste" copies datas to the tilemap.
	 * \param tileTypeIds The datas containing the new tiletypes
	 */
	void SetTileTypes(std::vector<TileTypeId> tileTypeIds);
	/**
	 * \brief Get all the tiletypes datas of the tilemap
	 * \return Vector of TileTypeId
	 */
	std::vector<TileTypeId>& GetTileTypes();
	
	/**
	 * \brief Get all the tileEntities of the tilemap, at their position
	 * \return Vector of Entities, poiting to Tile entities of the tilemap
	 */
	std::vector<Entity>& GetTiles();
#else
	/**
	 * \brief Set all the tile types passed in parameter to the tilemap. Usually used to "paste" copies datas to the tilemap.
	 * \param tileTypeIds The datas containing the new tiletypes
	 */
	void SetTileTypes(std::vector<std::vector<TileTypeId>> tileTypeIds);
	
	/**
	 * \brief Get all the tiletypes datas of the tilemap
	 * \return Vector of TileTypeId
	 */
	std::vector<std::vector<TileTypeId>>& GetTileTypes();
	/**
	 * \brief Get all the tileEntities of the tilemap, at their position
	 * \return Vector of Entities, poiting to Tile entities of the tilemap
	 */
	std::vector<std::vector<Entity>>& GetTiles();
#endif
	/**
	 * \brief Assign a Tile entity to a position in the tilemap. If the position is out of bounds, does nothing.
	 * \param pos Position of the tile.
	 * \param entity Tile Entity.
	 */
	void AddTile(Vec2f pos, Entity entity);

	/**
	 * \brief Returns the EntityId positionned at specified position
	 * \param pos Position of the tile.
	 * \return Entity wanted
	 */
	Entity GetTileAt(Vec2f pos);
	/**
	 * \brief Returns the EntityId positionned at specified position
	 * \param tileEntity Entity of the tile.
	 * \return Position of the passed tile
	 */
	Vec2f GetTileAt(Entity tileEntity);

	/**
	 * \brief Set a new tiletype at the specified position
	 * \param pos Position of the tile.
	 * \param newTileType
	 * \return Entity wanted
	 */
	void SetTileAt(Vec2f pos, TileTypeId newTileType);
	/**
	 * \brief Set a new tiletype at the specified position
	 * \param entity Entity of the tile, contained by the tilemap.
	 * \param newTileType
	 * \return Entity wanted
	 */
	void SetTileAt(Entity entity, TileTypeId newTileType);

	/**
	 * \brief Resize the limit size of the tilemap
	 * \param newSize New size of the tilemap
	 */
	void ResizeTilemap(Vec2f newSize);

protected:
	/**
	 * \brief Contains all the entities "tile" stored in the tilemap
	 */
#ifdef Opti1
	std::vector<Entity> m_Tiles;
	Vec2f m_TilemapSize = { 0, 0 };
#else
	std::vector<std::vector<Entity>> m_Tiles;
#endif
	/**
	 * \brief Contains all the tiletypeIds of the tilemap
	 */
#ifdef Opti1
	std::vector<TileTypeId> m_TileTypeIds;
#else
	std::vector<std::vector<TileTypeId>> m_TileTypeIds;
#endif
	/**
	 * \brief Size of a tile in the tilemap in pixel
	 */
	Vec2f m_TileSize = {1, 1};
	/**
	 * \brief Layer of the tilemap (Concerns the layer of drawing)
	 */
	int m_Layer = 0;
	/**
	 * \brief Pointer to the TileManager
	 */
	TileManager* m_TileManager;
	/**
	 * \brief Displays the tilemap as an isometric one or not
	 */
	bool m_IsIsometric = false;
};

namespace editor
{
struct TilemapInfo : ComponentInfo
{
	void DrawOnInspector() override;
	Tilemap* tilemap = nullptr;
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

	json Save();

	Tilemap* AddComponent(Entity entity) override;
	void CreateComponent(json& componentJson, Entity entity) override;
	void UpdateTile(Entity tilemapEntity, Vec2f pos, TileTypeId tileTypeId);
	void DestroyComponent(Entity entity) override;
	void OnResize(size_t new_size) override;

	/**
	 * \brief Create a tilemap with the json passed 
	 * \param entity 
	 * \param map Json tilemap data to construct the tilemap
	 */
	void InitializeMap(Entity entity, json& map);
	/**
	 * \brief Create a tilemap with the json passed
	 * \param entity
	 * \param tileTypeIds c++ data struct to construct the tilemap
	 * \param tilemapSize Size of the tilemap
	 */
#ifdef Opti1
	void InitializeMap(Entity entity, std::vector<TileTypeId> tileTypeIds, Vec2f tilemapSize);
#else
	void InitializeMap(Entity entity, std::vector<std::vector<TileTypeId>> tileTypeIds);
#endif
	
	/**
	 * \brief Set the position of all the tiles within the tilemap
	 * \param entity containing a tilemap
	 */
	void SetupTilePosition(Entity entity);

	/**
	 * \brief Empty the map and destroys all the entity tiles attached to it.
	 * \param entity containing the tilemap
	 */
	void EmptyMap(Entity entity);

	Vec2f GetTilePositionFromMouse(Entity entity);
	Entity GetTileEntityFromMouse(Entity entity);
protected:
	Transform2dManager* m_Transform2dManager = nullptr;
	TilemapSystem* m_TilemapSystem = nullptr;
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

	void Destroy() override;

	void Clear() override;

	void Collect() override;

	json Save();

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