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
#include <graphics/tile_asset.h>
#include <sfml/Graphics.hpp>

namespace sfge
{
/**
 * \author : Duncan Bourquard
 * \version : 1.2
 * \date : 04.03.2019
 */

using TileId = unsigned;
	
class Tilemap
{
public:
	/**
	 * \brief Constructor of tilemap.
	 */
	Tilemap();

	/**
	 * \brief Init the tilemap.
	 */
	void Init();
	
	/**
	 * \brief Update the tilemap and all the tiles within.
	 */
	void Update();

	/**
	 * \brief Draw all the tiles on the window
	 */
	void Draw(sf::RenderWindow &window);

	/**
	 * \brief Save the tilemap.
	 * \return Json containing all the needed informations.
	 */
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
	std::vector<TileId>& GetTiles();

	/**
	 * \brief Returns the TileId positionned at specified position
	 * \param pos Position of the tile.
	 * \return TileId wanted
	 */
	TileId GetTileAt(Vec2f pos);

	/**
	 * \brief Returns the TileId positionned at specified position
	 * \param tileId TileId of the tile.
	 * \return Position of the passed tile
	 */
	Vec2f GetTileAt(TileId tileId);

	/**
	 * \brief Set a new tiletype at the specified position
	 * \param pos Position of the tile.
	 * \param newTileType
	 */
	void SetTileAt(Vec2f pos, TileTypeId newTileType);

	/**
	 * \brief Set a new tiletype at the specified position
	 * \param tileId TileId of the tile, contained by the tilemap.
	 * \param newTileType
	 */
	void SetTileAt(TileId tileId, TileTypeId newTileType);

	void SetTilePosition(TileId tileId, Vec2f position);
	void SetTilePosition(Vec2f tilePos, Vec2f position);
	Vec2f GetTilePosition(TileId tileId);
	Vec2f GetTilePosition(Vec2f tilePos);

	sf::Sprite* GetSprite(TileId tileId);
	void SetTexture(TileId tileId, sf::Texture* texture);

	/**
	 * \brief Resize the limit size of the tilemap
	 * \param newSize New size of the tilemap
	 */
	void ResizeTilemap(Vec2f newSize);

protected:
	/**
	 * \brief Contains all the entities "tile" stored in the tilemap
	 */
	std::vector<TileId> m_Tiles;
	Vec2f m_TilemapSize = { 0, 0 };

	/**
	 * \brief Contains all the tiletypeIds of the tilemap
	 */
	std::vector<TileTypeId> m_TileTypeIds;

	/**
	 * \brief Contains all the sprites of the tilemap
	 */
	std::vector<Vec2f> m_TilePositions;

	/**
	 * \brief Contains all the sprites of the tilemap
	 */
	std::vector<sf::Sprite> m_TileSprites;

	/**
	 * \brief Size of a tile in the tilemap in pixel
	 */
	Vec2f m_TileSize = {1, 1};

	/**
	 * \brief Layer of the tilemap (Concerns the layer of drawing)
	 */
	int m_Layer = 0;

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
	void Draw(sf::RenderWindow &window);

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
	void InitializeMap(Entity entity, std::vector<TileTypeId> tileTypeIds, Vec2f tilemapSize);
	
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

	/**
	 * \brief Calculate the tile the mouse currently pointes at and returns its position.
	 * \param entity Entity containing the tilemap
	 * \return Position in the tilemap
	 */
	Vec2f GetTilePositionFromMouse(Entity entity);

	/**
	 * \brief Calculate the tile the mouse currently pointes at and returns the tile entity.
	 * \param entity Entity containing the tilemap
	 * \return Entity of the tile
	 */
	TileId GetTileEntityFromMouse(Entity entity);

	/**
	 * \brief Update the draw order of the tilemaps. Usually called when the layer order is changed.
	 */
	void UpdateDrawOrderTilemaps();

	/**
	 * \brief Returns all the current existing tilemaps.
	 * \return list of entities containing a tilemap.
	 */
	std::vector<Entity> GetAllTilemaps();
protected:
	std::vector<Entity> m_Tilemaps;
	std::vector<Entity> m_OrderToDrawTilemaps;

	//	Transform2dManager* m_Transform2dManager = nullptr;
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

	void DrawTilemaps(sf::RenderWindow &window);

	void Destroy() override;

	void Clear() override;

	void Collect() override;

	json Save();

	TilemapManager* GetTilemapManager();
	TileTypeManager* GetTileTypeManager();

protected:
	TilemapManager m_TilemapManager { m_Engine };
	TileTypeManager m_TileTypeManager { m_Engine };
};
}
#endif /* INCLUDE_TILEMAP_H_ */