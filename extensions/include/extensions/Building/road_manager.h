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

#ifndef ROAD_MANAGER_H
#define ROAD_MANAGER_H

#include <engine/system.h>
#include <graphics/graphics2d.h>

#include <extensions/Building/building_utilities.h>
#include "engine/tile_asset.h"

namespace sfge::ext
{
	/**
	 * \author Robin Alves
	 */
	class RoadManager : public System
	{
	public:
		RoadManager(Engine& engine);

		void Init() override;

		void Update(float dt) override;

		void FixedUpdate() override;

		void Draw() override;
		
		void SpawnRoad(Vec2f position, int roadBitMask);

		void SpawnRoad(std::vector<TileTypeId> tilesTypeVector, const int LengthX, const int LengthY, Vec2f positionFirstTile, Vec2f size, int targetType);

		void DestroyRoad(const Entity entity);

		void GetNeighbors();


	private:
		bool CheckEmptySlot(const Entity entity, Transform2d* transform2d, int roadBitMask);

		void RoadTextureSelection(const Entity entity, Transform2d* transform2d, int roadBitMask);

		void SetupTexture(const Entity entity, sf::Texture* texture, const TextureId textureId, const std::string texturePath);


		Transform2dManager* m_Transform2DManager;
		TextureManager* m_TextureManager;
		SpriteManager* m_SpriteManager;
		EntityManager* m_EntityManager;
		Configuration* m_Configuration;

		bool m_Init = false;

		std::vector<Entity> m_EntityIndex;

		unsigned int m_BuildingIndexCount = 0;
		unsigned int m_NmbReservation = 0;

#pragma region Graphics section
		// Road cross 4 texture
		std::string m_CrossTexturePath;
		TextureId m_CrossTextureId;
		sf::Texture* m_CrossTexture;

		// Road threeWay texture
		std::string m_ThreeWayTexturePath;
		TextureId m_ThreeWayTextureId;
		sf::Texture* m_ThreeWayTexture;

		// Road OneWay texture
		std::string m_OneWayTexturePath;
		TextureId m_OneWayTextureId;
		sf::Texture* m_OneWayTexture;

		// Road End texture
		std::string m_EndTexturePath;
		TextureId m_EndTextureId;
		sf::Texture* m_EndTexture;

		// Road TurnNS texture
		std::string m_TurnNSTexturePath;
		TextureId m_TurnNSTextureId;
		sf::Texture* m_TurnNSTexture;

		// Road TurnWE texture
		std::string m_TurnWETexturePath;
		TextureId m_TurnWETextureId;
		sf::Texture* m_TurnWETexture;

		// Road TurnWE texture
		std::string m_SoloTexturePath;
		TextureId m_SoloTextureId;
		sf::Texture* m_SoloTexture;

		// Road TurnWE texture
		std::string m_GroundTexturePath;
		TextureId m_GroundTextureId;
		sf::Texture* m_GroundTexture;
#pragma endregion 
	};
}
#endif