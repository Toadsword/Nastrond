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

#include <imgui.h>

#include <graphics/graphics2d.h>
#include <graphics/tilemap.h>
#include <graphics/tile_asset.h>
#include <graphics/texture.h>
#include <engine/engine.h>
#include <input/input.h>

namespace sfge
{
	Tilemap::Tilemap()
	{
	}

	void Tilemap::Init()
	{
	}

	void Tilemap::Update()
	{
	}

	void Tilemap::Draw(sf::RenderWindow &window)
	{
		for(auto sprite : m_TileSprites)
		{
			window.draw(sprite);
		}
	}

	json Tilemap::Save()
	{
		json j;
		j["type"] = static_cast<int>(ComponentType::TILEMAP);
		j["map_size"] = nlohmann::detail::value_t::array;
		Vec2f mapSize = GetTilemapSize();
		j["map_size"][0] = mapSize.x;
		j["map_size"][1] = mapSize.y;

		j["is_isometric"] = GetIsometric();
		Vec2f tileSize = GetTileSize();
		j["tile_size"][0] = tileSize.x;
		j["tile_size"][1] = tileSize.y;

		j["layer"] = GetLayer();

		j["map"] = nlohmann::detail::value_t::array;
		
		for(int indexY = 0; indexY < mapSize.y; indexY++)
		{
			for (int indexX = 0; indexX < mapSize.x; indexX++)
			{
				j["map"][indexX] = nlohmann::detail::value_t::array;
				j["map"][indexX][indexY] = m_TileTypeIds[indexY * mapSize.x + indexX];
			}
		}
		return j;
	}

	Vec2f Tilemap::GetTilemapSize()
	{
		return m_TilemapSize;
	}

	void Tilemap::SetTileSize(Vec2f newSize)
	{
		if(newSize.x > 0 && newSize.y > 0)
			m_TileSize = newSize;
	}

	Vec2f Tilemap::GetTileSize()
	{
		return m_TileSize;
	}

	void Tilemap::SetLayer(int newLayer)
	{
		if(newLayer > 0)
			m_Layer = newLayer;
	}

	int Tilemap::GetLayer()
	{
		return m_Layer;
	}

	void Tilemap::SetIsometric(bool newIso)
	{
		m_IsIsometric = newIso;
	}

	bool Tilemap::GetIsometric()
	{
		return m_IsIsometric;
	}

	void Tilemap::SetTileTypes(std::vector<TileTypeId> tileTypeIds)
	{
		m_TileTypeIds = tileTypeIds;
	}

	std::vector<TileTypeId>& Tilemap::GetTileTypes()
	{
		return m_TileTypeIds;
	}

	std::vector<TileId>& Tilemap::GetTiles()
	{
		return m_Tiles;
	}

	TileId Tilemap::GetTileAt(Vec2f pos)
	{
		if (pos.x >= 0 && pos.y >= 0 && m_TilemapSize.x > pos.x && m_TilemapSize.y > pos.y)
			return m_Tiles[static_cast<int>(pos.y) * m_TilemapSize.x + static_cast<int>(pos.x)];
		return INVALID_ENTITY;
	}

	Vec2f Tilemap::GetTileAt(TileId tileId)
	{
		const unsigned sizeY = m_TilemapSize.y;
		const unsigned size = m_TilemapSize.x * m_TilemapSize.y;
		for (unsigned index = 0; index < size; index++)
		{
			if (m_Tiles[index] == tileId)
				return Vec2f(index % sizeY, index / sizeY);
		}
		return Vec2f(0, 0);
	}

	void Tilemap::SetTileAt(Vec2f pos, TileTypeId newTileType)
	{
		m_TileTypeIds[pos.y * m_TilemapSize.x + pos.x] = newTileType;
	}

	void Tilemap::SetTileAt(TileId tileId, TileTypeId newTileType)
	{
		m_TileTypeIds[tileId] = newTileType;
	}

	void Tilemap::SetTilePosition(TileId tileId, Vec2f position)
	{
		m_TilePositions[tileId] = position;
		m_TileSprites[tileId].setPosition(position);
	}

	void Tilemap::SetTilePosition(Vec2f tilePos, Vec2f position)
	{
		m_TilePositions[tilePos.y * m_TilemapSize.x + tilePos.x] = position;
	}

	Vec2f Tilemap::GetTilePosition(TileId tileId)
	{
		return m_TilePositions[tileId];
	}

	Vec2f Tilemap::GetTilePosition(Vec2f tilePos)
	{
		return m_TilePositions[tilePos.y * m_TilemapSize.x + tilePos.x];
	}

	sf::Sprite* Tilemap::GetSprite(TileId tileId)
	{
		return &m_TileSprites[tileId];
	}

	void Tilemap::SetTexture(TileId tileId, sf::Texture* texture)
	{
		m_TileSprites[tileId].setTexture(*texture);
		m_TileSprites[tileId].setOrigin(sf::Vector2f(m_TileSprites[tileId].getLocalBounds().width, m_TileSprites[tileId].getLocalBounds().height) / 2.0f);
		m_TileSprites[tileId].setPosition(m_TilePositions[tileId]);
	}

	void Tilemap::ResizeTilemap(Vec2f newSize)
	{
		std::vector<TileTypeId> oldTileTypes = m_TileTypeIds;
		std::vector<Vec2f> oldPosition = m_TilePositions;
		std::vector<sf::Sprite> oldSprites = m_TileSprites;
		Vec2f oldSize = GetTilemapSize();

		m_TilemapSize = newSize;

		m_Tiles = std::vector<TileId>(newSize.x * newSize.y, 0);
		m_TileTypeIds = std::vector<TileTypeId>(newSize.x * newSize.y, INVALID_TILE_TYPE);
		m_TilePositions = std::vector<Vec2f>(newSize.x * newSize.y, Vec2f());
		m_TileSprites = std::vector<sf::Sprite>(newSize.x * newSize.y, sf::Sprite());

		if (newSize.x > 0 && newSize.y > 0)
		{
			for (unsigned i = 0; i < newSize.x * newSize.y; i++)
				m_Tiles[i] = i;

			unsigned size = std::min(newSize.x * newSize.y, oldSize.x * oldSize.y);
			for (unsigned i = 0; i < size; i++)
				m_TileTypeIds[i] = oldTileTypes[i];

			for (unsigned i = 0; i < size; i++)
				m_TilePositions[i] = oldPosition[i];

			for (unsigned i = 0; i < size; i++)
				m_TileSprites[i] = oldSprites[i];
		}
	}

	void editor::TilemapInfo::DrawOnInspector()
	{
		ImGui::Separator();
		ImGui::Text("Tilemap");
		if(tilemap)
		{
			int layer = tilemap->GetLayer();
			ImGui::InputInt("Layer", &layer);

			bool isIsometric = tilemap->GetIsometric();
			ImGui::Checkbox("Is isometric", &isIsometric);
			
			int aTileSize[2] = { tilemap->GetTileSize().x , tilemap->GetTileSize().y };
			ImGui::InputInt2("Tile size", aTileSize);

			int aTilemapSize[2] = { tilemap->GetTilemapSize().x , tilemap->GetTilemapSize().y };
			ImGui::InputInt2("Tilemap size", aTilemapSize);
		}
	}

	void TilemapManager::Init()
	{
		SingleComponentManager::Init();
		//m_Transform2dManager = m_Engine.GetTransform2dManager();
		m_TilemapSystem = m_Engine.GetGraphics2dManager()->GetTilemapSystem();
	}

	void TilemapManager::Update(float dt)
	{
		rmt_ScopedCPUSample(TilemapUpdate, 0)
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::TILEMAP))
			{
				m_Components[i].Update();
			}
		}
	}

	void TilemapManager::Draw(sf::RenderWindow &window)
	{
		rmt_ScopedCPUSample(TilemapDraw, 0)
		for(Entity tilemap : m_OrderToDrawTilemaps)
		{
			m_Components[tilemap].Draw(window);
		}
	}

	void TilemapManager::Clear()
	{
	}

	void TilemapManager::Collect()
	{
	}

	json TilemapManager::Save()
	{
		json j;
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::TILEMAP))
			{
				j[i] = m_Components[i].Save();
			}
		}
		return j;
	}

	Tilemap * TilemapManager::AddComponent(Entity entity)
	{
		auto& tilemap = GetComponentRef(entity);
		auto& tilemapInfo = GetComponentInfo(entity);

		if (!m_Engine.GetEntityManager()->HasComponent(entity, ComponentType::TRANSFORM2D))
			m_Engine.GetTransform2dManager()->AddComponent(entity);

		tilemapInfo.tilemap = &tilemap;

		m_Tilemaps.push_back(entity - 1);
		UpdateDrawOrderTilemaps();

		m_EntityManager->AddComponentType(entity, ComponentType::TILEMAP);
		return &tilemap;
	}

	void TilemapManager::CreateComponent(json & componentJson, Entity entity)
	{
		json tilemapJson;

		if (CheckJsonExists(componentJson, "path") && CheckJsonParameter(componentJson, "path", nlohmann::detail::value_t::string))
		{
			std::string path = componentJson["path"].get<std::string>();
			{
				std::ostringstream oss;
				oss << "Loading scene from: " << path;
				Log::GetInstance()->Msg(oss.str());
			}
			const auto tilemapJsonPtr = LoadJson(path);

			if (tilemapJsonPtr != nullptr)
				tilemapJson = *tilemapJsonPtr;
			else
			{
				Log::GetInstance()->Error("Couldn't load tilemap from path : '" + path + "'.");
				return;
			}
			
		}
		else
			tilemapJson = componentJson;

		if(CheckJsonExists(tilemapJson, "reference_path") && CheckJsonParameter(tilemapJson, "reference_path", nlohmann::detail::value_t::string))
		{
			m_TilemapSystem->GetTileTypeManager()->LoadTileType(tilemapJson["reference_path"].get<std::string>());		
		}

		auto & newTilemap = m_Components[entity - 1];
		auto & newTilemapInfo = m_ComponentsInfo[entity - 1];

		newTilemapInfo.tilemap = &newTilemap;

		if (!m_Engine.GetEntityManager()->HasComponent(entity, ComponentType::TRANSFORM2D))
			m_Engine.GetTransform2dManager()->AddComponent(entity);

		if (CheckJsonExists(tilemapJson, "is_isometric") && CheckJsonParameter(tilemapJson, "is_isometric", nlohmann::detail::value_t::boolean))
		{
			newTilemap.SetIsometric(tilemapJson["is_isometric"].get<bool>());
		}
		
		if (CheckJsonExists(tilemapJson, "layer") && CheckJsonParameter(tilemapJson, "layer", nlohmann::detail::value_t::number_integer))
		{
			newTilemap.SetLayer(tilemapJson["layer"].get<int>());
		}

		if (CheckJsonExists(tilemapJson, "tile_size") && CheckJsonParameter(tilemapJson, "tile_size", nlohmann::detail::value_t::array))
		{
			Vec2f tileSize = Vec2f();
			tileSize.x = tilemapJson["tile_size"][0].get<unsigned>();
			tileSize.y = tilemapJson["tile_size"][1].get<unsigned>();
			newTilemap.SetTileSize(tileSize);
		}

		if (CheckJsonExists(tilemapJson, "map_size") && CheckJsonParameter(tilemapJson, "map_size", nlohmann::detail::value_t::array))
		{
			Vec2f mapSize = Vec2f();
			mapSize.x = tilemapJson["map_size"][0].get<unsigned>();
			mapSize.y = tilemapJson["map_size"][1].get<unsigned>();
			newTilemap.ResizeTilemap(mapSize);
		}

		if(CheckJsonExists(tilemapJson, "map") && CheckJsonParameter(tilemapJson, "map", nlohmann::detail::value_t::array))
		{
			InitializeMap(entity, tilemapJson["map"]);
		}

		m_Tilemaps.push_back(entity - 1);
		UpdateDrawOrderTilemaps();
	}

	void TilemapManager::UpdateTile(Entity tilemapEntity, Vec2f pos, TileTypeId tileTypeId)
	{
		auto* tilemap = GetComponentPtr(tilemapEntity);
		m_TilemapSystem->GetTileTypeManager()->SetTileTexture(tilemapEntity, tilemap->GetTileAt(pos), tileTypeId);
	}

	void TilemapManager::DestroyComponent(Entity entity)
	{
		(void) entity;
	}

	void TilemapManager::OnResize(size_t new_size)
	{
		m_Components.resize(new_size);
		m_ComponentsInfo.resize(new_size);
	}

	void TilemapManager::InitializeMap(Entity entity, json & map)
	{
		if (map.empty())
			return;

		std::vector<TileTypeId> tiletypeIds = std::vector<TileTypeId>(map.size() * map[0].size(), INVALID_TILE_TYPE);

		Vec2f mapSize = Vec2f(map.size(), map[0].size());
		for (unsigned indexY = 0; indexY < mapSize.y; indexY++)
		{
			for (unsigned indexX = 0; indexX < mapSize.x; indexX++)
			{
				tiletypeIds[indexY * mapSize.x + indexX] = map[indexX][indexY].get<int>();
			}
		}
		InitializeMap(entity, tiletypeIds, mapSize);
	}

	void TilemapManager::InitializeMap(Entity entity, std::vector<TileTypeId> tileTypeIds, Vec2f tilemapSize)
	{
		auto & tilemap = m_Components[entity - 1];
		EmptyMap(entity);

		if (!tileTypeIds.empty())
			tilemap.ResizeTilemap(tilemapSize);

		//Init Assigns textures to the tilemap
		for (int i = 0; i < tileTypeIds.size(); i++)
		{
			TextureId textId = m_TilemapSystem->GetTileTypeManager()->GetTextureFromTileType(tileTypeIds[i]);
			tilemap.SetTexture(i, m_Engine.GetGraphics2dManager()->GetTextureManager()->GetTexture(textId));
		}

		SetupTilePosition(entity);

		tilemap.SetTileTypes(tileTypeIds);
	}

	void TilemapManager::SetupTilePosition(Entity entity)
	{
		if (entity == INVALID_ENTITY)
			return;

		auto & tilemap = m_Components[entity - 1];
		const Vec2f mapSize = tilemap.GetTilemapSize();
		const auto& tiles = tilemap.GetTiles();

		const Vec2f basePos = m_Engine.GetTransform2dManager()->GetComponentPtr(entity)->Position;
		const Vec2f tileSize = tilemap.GetTileSize();
		Vec2f xPos, yPos;

		//Layout of tilemap
		if (tilemap.GetIsometric())
		{
			xPos = { tileSize.x / 2.0f, tileSize.y / 2.0f };
			yPos = { -tileSize.x / 2.0f, tileSize.y / 2.0f };
		}
		else
		{
			xPos = { tileSize.x , 0 };
			yPos = { 0, tileSize.y };
		}
		for (int indexY = 0; indexY < mapSize.y; indexY++)
		{
			for (int indexX = 0; indexX < mapSize.x; indexX++)
			{
				const TileId tileId = tiles[indexY * mapSize.x + indexX];
				const Vec2f newPos = basePos + xPos * indexX + yPos * indexY;
				tilemap.SetTilePosition(tileId, newPos);				
			}
		}
	}

	void TilemapManager::EmptyMap(Entity entity)
	{
		auto & tilemap = m_Components[entity - 1];
		const Vec2f mapSize = tilemap.GetTilemapSize();

		std::vector<Entity>& map = tilemap.GetTiles();
		std::vector<TileTypeId>& mapTiles = tilemap.GetTileTypes();
		
		const unsigned size = mapSize.x * mapSize.y;
		for (int i = 0; i < size; i++)
		{
			map[i] = INVALID_ENTITY;
			map[i] = INVALID_TILE_TYPE;
		}
	}

	Vec2f TilemapManager::GetTilePositionFromMouse(Entity entity)
	{
		auto & tilemap = m_Components[entity - 1];
		const sf::Vector2i worldPosSf = m_Engine.GetInputManager()->GetMouseManager().GetWorldPosition();
		Vec2f worldPos = Vec2f(worldPosSf.x, worldPosSf.y);

		Vec2f tilemapPos = m_Engine.GetTransform2dManager()->GetComponentPtr(entity)->Position;
		const Vec2f mapSize = tilemap.GetTilemapSize();

		const Vec2f tileSize = tilemap.GetTileSize();
		Vec2f xPos, yPos;
		Vec2f deltaPos = worldPos - tilemapPos;
		Vec2f indexToFind = Vec2f(0, 0);

		if (tilemap.GetIsometric())
		{
			// Offsets magic numbers
			// -1 because the index begins at 1, but it begins at 0 in the vector
			// 0.4 because magic
			indexToFind.x = (deltaPos.x / tileSize.x + deltaPos.y / tileSize.y) - 1.0f;  
			indexToFind.y = (deltaPos.y / tileSize.y - deltaPos.x / tileSize.x) + 0.4f;
		}
		else
		{
			indexToFind.x = deltaPos.x / tileSize.x;
			indexToFind.y = deltaPos.y / tileSize.y;
		}
		return indexToFind;
	}

	TileId TilemapManager::GetTileEntityFromMouse(Entity entity)
	{
		auto & tilemap = m_Components[entity - 1];
		Vec2f tilePosInTilemap = GetTilePositionFromMouse(entity);
		return tilemap.GetTileAt(tilePosInTilemap);
	}

	void TilemapManager::UpdateDrawOrderTilemaps()
	{
		m_OrderToDrawTilemaps = m_Tilemaps;

		//Sort their layers
		for (int i = 0; i < m_OrderToDrawTilemaps.size() - 1; i++)
		{
			for (int j = 0; j < m_OrderToDrawTilemaps.size() - i - 1; j++)
			{
				GetComponentPtr(m_OrderToDrawTilemaps[j])->GetLayer();
				if (GetComponentPtr(m_OrderToDrawTilemaps[j])->GetLayer() > GetComponentPtr(m_OrderToDrawTilemaps[j + 1])->GetLayer())
				{
					int tmp = m_OrderToDrawTilemaps[j];
					m_OrderToDrawTilemaps[j] = m_OrderToDrawTilemaps[j + 1];
					m_OrderToDrawTilemaps[j + 1] = tmp;
				}
			}
		}
	}

	void TilemapSystem::Init()
	{
		m_TileTypeManager.Init();
		m_TilemapManager.Init();
	}

	void TilemapSystem::Update(float dt)
	{
		m_TilemapManager.Update(dt);
	}

	void TilemapSystem::DrawTilemaps(sf::RenderWindow& window)
	{
		m_TilemapManager.Draw(window);
	}

	void TilemapSystem::Destroy()
	{
		Clear();
		Collect();
	}

	void TilemapSystem::Clear()
	{
		m_TileTypeManager.Clear();
		m_TilemapManager.Clear();
	}

	void TilemapSystem::Collect()
	{
		m_TileTypeManager.Collect();
		m_TilemapManager.Collect();
	}

	json TilemapSystem::Save()
	{
		json j;
		
		j["tiletype"] = m_TileTypeManager.Save();
		j["tilemap"] = m_TilemapManager.Save();
		
		return j;
	}

	TilemapManager * TilemapSystem::GetTilemapManager()
	{
		return &m_TilemapManager;
	}

	TileTypeManager * TilemapSystem::GetTileTypeManager()
	{
		return &m_TileTypeManager;
	}
}