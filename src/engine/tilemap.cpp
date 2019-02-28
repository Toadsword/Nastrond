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

#include <engine\tilemap.h>
#include <graphics/texture.h>
#include <engine/engine.h>
#include <engine/tile_asset.h>
#include <input/input.h>

namespace sfge
{
	Tilemap::Tilemap()
	{
	}

	void Tilemap::Init(TileManager* tileManager)
	{
		m_TileManager = tileManager;
	}

	void Tilemap::Update()
	{
		Tile* tile;
		Vec2f size = GetTilemapSize();
		for (int indexX = 0; indexX < size.x; indexX++)
		{
			for (int indexY = 0; indexY < size.y; indexY++)
			{
#ifdef OptiVector
				tile = m_TileManager->GetComponentPtr(m_Tiles[indexX * size.y + indexY]);
#else
				tile = m_TileManager->GetComponentPtr(m_Tiles[indexX][indexY]);
#endif
				if (tile != nullptr)
				{
					tile->Update();
				}
			}
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
		for(int indexX = 0; indexX < mapSize.x; indexX++)
		{
			j["map"][indexX] = nlohmann::detail::value_t::array;
			for (int indexY = 0; indexY < mapSize.y; indexY++)
			{
#ifdef OptiVector
				j["map"][indexX][indexY] = m_TileTypeIds[indexX * mapSize.y + indexY];
#else
				j["map"][indexX][indexY] = m_TileTypeIds[indexX][indexY];
#endif
			}
		}
		return j;
	}

	Vec2f Tilemap::GetTilemapSize()
	{
#ifdef OptiVector
		return m_TilemapSize;
#else
		if (m_Tiles.empty())
			return Vec2f();
		return Vec2f(m_Tiles.size(), m_Tiles[0].size());
#endif
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

#ifdef OptiVector
	void Tilemap::SetTileTypes(std::vector<TileTypeId> tileTypeIds)
	{
		m_TileTypeIds = tileTypeIds;
	}

	std::vector<TileTypeId>& Tilemap::GetTileTypes()
	{
		return m_TileTypeIds;
	}

	std::vector<Entity>& Tilemap::GetTiles()
	{
		return m_Tiles;
	}
#else
	void Tilemap::SetTileTypes(std::vector<std::vector<TileTypeId>> tileTypeIds)
	{
		m_TileTypeIds = tileTypeIds;
	}

	std::vector<std::vector<TileTypeId>>& Tilemap::GetTileTypes()
	{
		return m_TileTypeIds;
	}

	std::vector<std::vector<Entity>>& Tilemap::GetTiles()
	{
		return m_Tiles;
	}
#endif

	void Tilemap::AddTile(Vec2f pos, Entity entity)
	{
#ifdef OptiVector
		if(pos.x < m_TilemapSize.x && pos.y < m_TilemapSize.y)
			m_Tiles[static_cast<int>(pos.x) * m_TilemapSize.y + static_cast<int>(pos.y)] = entity;
#else
		if (pos.x < m_Tiles.size() && pos.y < m_Tiles[0].size())
			m_Tiles[pos.x][pos.y] = entity;
#endif
	}

	Entity Tilemap::GetTileAt(Vec2f pos)
	{
#ifdef OptiVector
		if (pos.x >= 0 && pos.y >= 0 && m_TilemapSize.x > pos.x && m_TilemapSize.y > pos.y)
			return m_Tiles[static_cast<int>(pos.x) * m_TilemapSize.y + static_cast<int>(pos.y)];
#else
		Vec2f size = GetTilemapSize();
		if (pos.x >= 0 && pos.y >= 0 && size.x > pos.x && size.y > pos.y)
			return m_Tiles[pos.x][pos.y];
#endif
		return INVALID_ENTITY;
	}

	Vec2f Tilemap::GetTileAt(Entity tileEntity)
	{
#ifdef OptiVector
		for (unsigned indexX = 0; indexX < m_TilemapSize.x; indexX++)
		{
			for (unsigned indexY = 0; indexY < m_TilemapSize.y; indexY++)
			{
				if (m_Tiles[indexX * m_TilemapSize.y + indexY] == tileEntity)
					return Vec2f(indexX, indexY);
			}
		}
#else
		Vec2f size = GetTilemapSize();
		for (unsigned indexX = 0; indexX < size.x; indexX++)
		{
			for (unsigned indexY = 0; indexY < size.y; indexY++)
			{
				if (m_Tiles[indexX][indexY] == tileEntity)
					return Vec2f(indexX, indexY);
			}
		}
#endif
		return Vec2f(0, 0);
	}

	void Tilemap::SetTileAt(Vec2f pos, TileTypeId newTileType)
	{
#ifdef OptiVector
		m_TileTypeIds[pos.x * m_TilemapSize.y + pos.y] = newTileType;
#else
		m_TileTypeIds[pos.x][pos.y] = newTileType;
#endif
	}

	void Tilemap::SetTileAt(Entity entity, TileTypeId newTileType)
	{
		SetTileAt(GetTileAt(entity), newTileType);
	}

	void Tilemap::ResizeTilemap(Vec2f newSize)
	{
#ifdef OptiVector
		std::vector<Entity> oldTiles = m_Tiles;
		//std::vector<std::vector<TileTypeId>> oldTileTypes = m_TileTypeIds;
		Vec2f oldSize = GetTilemapSize();

		m_Tiles = std::vector<Entity>(newSize.x * newSize.y, INVALID_ENTITY);
		m_TilemapSize = newSize;

		//m_TileTypeIds = std::vector<TileTypeId>{ std::vector<Entity>(newSize.x * newSize.y)};

		if (newSize.x > 0 && newSize.y > 0)
		{
			for (unsigned indexX = 0; indexX < newSize.x; indexX++)
			{
				for (unsigned indexY = 0; indexY < newSize.y; indexY++)
				{
					if (indexX < oldSize.x && indexY < oldSize.y)
					{
						m_Tiles[indexX * newSize.y + indexY] = oldTiles[indexX * oldSize.y + indexY];
						//m_TileTypeIds[indexX][indexY] = oldTileTypes[indexX][indexY];
					}
					/*else
					{
						m_Tiles[indexX * newSize.x + indexY] = INVALID_ENTITY;
						//m_TileTypeIds[indexX][indexY] = INVALID_TILE_TYPE;						
					}*/
				}
			}
		}
#else
		std::vector<std::vector<Entity>> oldTiles = m_Tiles;
		//std::vector<std::vector<TileTypeId>> oldTileTypes = m_TileTypeIds;
		Vec2f oldSize = GetTilemapSize();

		m_Tiles = std::vector<std::vector<Entity>>{ 
			static_cast<unsigned>(newSize.x), 
			std::vector<Entity>(static_cast<unsigned>(newSize.y)) 
		};

		/*
		m_TileTypeIds = std::vector<std::vector<TileTypeId>>{
			static_cast<unsigned>(newSize.x),
			std::vector<TileTypeId>(static_cast<unsigned>(newSize.y))
		};
		*/

		if(newSize.x > 0 && newSize.y > 0)
		{	
			for(unsigned indexX = 0; indexX < newSize.x; indexX++)
			{
				for(unsigned indexY = 0; indexY < newSize.y; indexY++)
				{
					if (indexX < oldSize.x && indexY < oldSize.y)
					{
						m_Tiles[indexX][indexY] = oldTiles[indexX][indexY];
						//m_TileTypeIds[indexX][indexY] = oldTileTypes[indexX][indexY];
					}
					else
					{
						m_Tiles[indexX][indexY] = INVALID_ENTITY;
						//m_TileTypeIds[indexX][indexY] = INVALID_TILE_TYPE;						
					}
				}		
			}
		}
#endif
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
			
			int aSize[2] = { tilemap->GetTileSize().x , tilemap->GetTileSize().y };
			ImGui::InputInt2("Tile size", aSize);
		}
	}

	void TilemapManager::Init()
	{
		SingleComponentManager::Init();
		m_Transform2dManager = m_Engine.GetTransform2dManager();
		m_TilemapSystem = m_Engine.GetTilemapSystem();
	}

	void TilemapManager::Update(float dt)
	{
		(void)dt;
		rmt_ScopedCPUSample(TilemapUpdate, 0)
		for (auto i = 0u; i < m_ConcernedEntities.size(); i++)
		{
			m_Components[m_ConcernedEntities[i] - 1].Update();
		}
		/*for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::TILEMAP))
			{
				m_Components[i].Update();
			}
		}*/
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
		/*
		* Component optimisation addition
		*/
		m_ConcernedEntities.push_back(entity);
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
	}

	void TilemapManager::UpdateTile(Entity tilemapEntity, Vec2f pos, TileTypeId tileTypeId)
	{
		auto* tilemap = GetComponentPtr(tilemapEntity);
		m_TilemapSystem->GetTileTypeManager()->SetTileTexture(tilemap->GetTileAt(pos), tileTypeId);
	}

	void TilemapManager::DestroyComponent(Entity entity)
	{
		RemoveConcernedEntity(entity);
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

#ifdef OptiVector
		std::vector<TileTypeId> tiletypeIds = std::vector<TileTypeId>(map.size() * map[0].size(), INVALID_TILE_TYPE);

		for (unsigned indexX = 0; indexX < map.size(); indexX++)
		{
			for (unsigned indexY = 0; indexY < map[indexX].size(); indexY++)
			{
				tiletypeIds[indexX * map[0].size() + indexY] = map[indexX][indexY].get<int>();
			}
		}
		InitializeMap(entity, tiletypeIds, Vec2f(map.size(), map[0].size()));
#else
		std::vector<std::vector<TileTypeId>> tiletypeIds = std::vector<std::vector<TileTypeId>>{
			map.size(),
			std::vector<TileTypeId>(map[0].size())
		};

		for (unsigned indexX = 0; indexX < tiletypeIds.size(); indexX++)
		{
			for (unsigned indexY = 0; indexY < tiletypeIds[indexX].size(); indexY++)
			{
				tiletypeIds[indexX][indexY] = map[indexX][indexY].get<int>();
			}
		}

		InitializeMap(entity, tiletypeIds);
#endif
	}

#ifdef OptiVector
	void TilemapManager::InitializeMap(Entity entity, std::vector<TileTypeId> tileTypeIds, Vec2f tilemapSize)
#else
	void TilemapManager::InitializeMap(Entity entity, std::vector<std::vector<TileTypeId>> tileTypeIds)
#endif
	{
		auto & tilemap = m_Components[entity - 1];
		EmptyMap(entity);

#ifndef OptiVector
		Vec2f tilemapSize = Vec2f(0, 0);
		if(tileTypeIds.size() > 0)
			tilemapSize = Vec2f(tileTypeIds.size(), tileTypeIds[0].size());
#endif
		if (!tileTypeIds.empty())
			tilemap.ResizeTilemap(tilemapSize);

		EntityManager* entityManager = m_Engine.GetEntityManager();
		TileManager* tileManager = m_TilemapSystem->GetTileManager();

		//Assigning positions
		for (unsigned indexX = 0; indexX < tilemapSize.y; indexX++)
		{
			for (unsigned indexY = 0; indexY < tilemapSize.x; indexY++)
			{
				const Entity newEntity = entityManager->CreateEntity(INVALID_ENTITY);
#ifdef OptiVector
				tileManager->AddComponent(newEntity, tileTypeIds[indexX * tilemapSize.x + indexY]);
#else
				tileManager->AddComponent(newEntity, tileTypeIds[indexX][indexY]);
#endif
				tilemap.AddTile(Vec2f(indexX, indexY), newEntity);
			}
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

		const Vec2f basePos = m_Transform2dManager->GetComponentPtr(entity)->Position;
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
		for (int indexX = 0; indexX < mapSize.x; indexX++)
		{
			for (int indexY = 0; indexY < mapSize.y; indexY++)
			{
#ifdef OptiVector
				const Entity tileEntity = tiles[indexX * mapSize.y + indexY];
#else
				const Entity tileEntity = tiles[indexX][indexY];
#endif
				if(tileEntity != INVALID_ENTITY)
				{
					const Vec2f newPos = basePos + xPos * indexX + yPos * indexY;
					m_Transform2dManager->GetComponentPtr(tileEntity)->Position = newPos;
				}
			}
		}
	}

	void TilemapManager::EmptyMap(Entity entity)
	{
		auto & tilemap = m_Components[entity - 1];
		const Vec2f mapSize = tilemap.GetTilemapSize();
#ifdef OptiVector
		std::vector<Entity>& map = tilemap.GetTiles();
		std::vector<TileTypeId>& mapTiles = tilemap.GetTileTypes();
#else
		std::vector<std::vector<Entity>>& map = tilemap.GetTiles();
		std::vector<std::vector<TileTypeId>>& mapTiles = tilemap.GetTileTypes();
#endif

		EntityManager* entityManager = m_Engine.GetEntityManager();

		for (int i = 0; i < mapSize.x; i++)
		{
			for (int j = 0; j < mapSize.y; j++)
			{
#ifdef OptiVector
				if(map[i * mapSize.y + j] != INVALID_ENTITY)
				{
					entityManager->DestroyEntity(map[i * mapSize.y + j]);
					map[i * mapSize.y + j] = INVALID_ENTITY;
					map[i * mapSize.y + j] = INVALID_TILE_TYPE;
				}
#else
				if (map[i][j] != INVALID_ENTITY)
				{
					entityManager->DestroyEntity(map[i][j]);
					map[i][j] = INVALID_ENTITY;
					map[i][j] = INVALID_TILE_TYPE;
				}
#endif
			}
		}
	}

	Vec2f TilemapManager::GetTilePositionFromMouse(Entity entity)
	{
		auto & tilemap = m_Components[entity - 1];
		const sf::Vector2i worldPosSf = m_Engine.GetInputManager()->GetMouseManager().GetWorldPosition();
		Vec2f worldPos = Vec2f(worldPosSf.x, worldPosSf.y);

		Vec2f tilemapPos = m_Transform2dManager->GetComponentPtr(entity)->Position;
		const Vec2f mapSize = tilemap.GetTilemapSize();

		const Vec2f tileSize = tilemap.GetTileSize();
		Vec2f xPos, yPos;
		Vec2f deltaPos = worldPos - tilemapPos;
		Vec2f indexToFind = Vec2f(0, 0);

		if (tilemap.GetIsometric())
		{
			// -1 car l'index commence dans tous les cas à 1, alors que nous on veut à 0
			// 0.4 parce que la magie
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

	Entity TilemapManager::GetTileEntityFromMouse(Entity entity)
	{
		auto & tilemap = m_Components[entity - 1];
		Vec2f pos = GetTilePositionFromMouse(entity);
		return tilemap.GetTileAt(pos);
	}

	void TilemapSystem::Init()
	{
		m_TileTypeManager.Init();
		m_TilemapManager.Init();
		m_TileManager.Init();
	}

	void TilemapSystem::Update(float dt)
	{
		//m_TilemapManager.Update(dt);
		//m_TileManager.Update(dt);
	}

	void TilemapSystem::FixedUpdate()
	{

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
		m_TileManager.Clear();
	}

	void TilemapSystem::Collect()
	{
		m_TileTypeManager.Collect();
		m_TilemapManager.Collect();
		m_TileManager.Collect();
	}

	json TilemapSystem::Save()
	{
		json j;
		
		j["tiletype"] = m_TileTypeManager.Save();
		j["tilemap"] = m_TilemapManager.Save();
		//m_TileManager.Save();
		
		return j;
	}

	TilemapManager * TilemapSystem::GetTilemapManager()
	{
		return &m_TilemapManager;
	}

	TileManager * TilemapSystem::GetTileManager()
	{
		return &m_TileManager;
	}

	TileTypeManager * TilemapSystem::GetTileTypeManager()
	{
		return &m_TileTypeManager;
	}
}