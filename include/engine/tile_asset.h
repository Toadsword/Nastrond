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

#ifndef SFGE_TILE_ASSET_H_
#define SFGE_TILE_ASSET_H_

 //tool_engine
#include <engine/component.h>

namespace sfge
{

using TextureId = unsigned;
class TextureManager;
class SpriteManager;

using TileTypeId = unsigned;
const TileTypeId INVALID_TILE_TYPE = 0U;

/**
* \brief The Texture Manager is the cache of all the textures used for sprites or other 
* \author Duncan Bourquard
*/
class TileTypeManager : public System
{
public:
	using System::System;

	TileTypeManager& operator=(const TileTypeManager&) = delete;
	/**
	 * \brief Load all the tile types in the data in Shipping mode
	 */
	void Init() override;

	/**
	* \brief load the tile types from the disk
	* \param filename The filename string containing the tile types used in the game
	* \return The strictly positive tile type id > 0, if equals 0 then the tile types was not loaded
	*/
	TileTypeId LoadTileType(std::string filename);

	/**
	* \brief load the tile types from the data passed in argument
	* \param jsonData Datas containing the tile types used
	* \return The strictly positive tile type id > 0, if equals 0 then the tile types was not loaded
	*/
	TileTypeId LoadTileType(json& jsonData);

	/**
	* \brief Set a texture to the current passed tile.
	* \param tileTypeId The tiletype id to apply to the tile
	* \param tileId The tile entity to apply texture or animation
	* \return true if successful, false otherwise
	*/
	bool SetTileTexture(Entity tileId, TileTypeId tileTypeId);

	void Clear() override;

	void Collect() override;

protected:
	TextureManager* m_TextureManager;
	SpriteManager* m_SpriteManager;
private:
	std::vector<TextureId> m_TexturesId = std::vector<TextureId>(INIT_ENTITY_NMB, 0);
	std::vector<size_t> m_TileTypeId = std::vector<size_t>(INIT_ENTITY_NMB, 0);
};
}

#endif /* INCLUDE_ENGINE_TILEMAP_H_ */
