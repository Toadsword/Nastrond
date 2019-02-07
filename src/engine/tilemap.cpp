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
		Vec2f size = GetSize();
		for (int indexX = 0; indexX < size.x; indexX++)
		{
			for (int indexY = 0; indexY < size.y; indexY++)
			{
				tile = m_TileManager->GetComponentPtr(m_Tiles[indexX][indexY]);
				if (tile != nullptr)
				{
					tile->Update();
				}
			}
		}
	}

	Vec2f Tilemap::GetSize()
	{
		if (m_Tiles.empty())
			return Vec2f();
		return Vec2f(m_Tiles.size(), m_Tiles[0].size());
	}

	void Tilemap::SetTileScale(Vec2f newScale)
	{
		m_TileScale = newScale;
	}

	Vec2f Tilemap::GetTileScale()
	{
		return m_TileScale;
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

	void Tilemap::AddTile(Vec2f pos, Entity entity)
	{
		if(pos.x < m_Tiles.size() && pos.y < m_Tiles[0].size())
			m_Tiles[pos.x][pos.y] = entity;
	}

	Entity Tilemap::GetTileAt(Vec2f pos)
	{
		return m_Tiles[pos.x][pos.y];
	}

	void Tilemap::ResizeTilemap(Vec2f newSize)
	{
		std::vector<std::vector<Entity>> oldTiles = m_Tiles;
		//std::vector<std::vector<TileTypeId>> oldTileTypes = m_TileTypeIds;
		Vec2f oldSize = GetSize();

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
	}

	void editor::TilemapInfo::DrawOnInspector()
	{
		ImGui::Separator();
		ImGui::Text("Tilemap");

		int layer = tilemap->GetLayer();
		ImGui::InputInt("Layer", &layer);

		bool isIsometric = tilemap->GetIsometric();
		ImGui::Checkbox("Is isometric", &isIsometric);
		
		int aScale[2] = { tilemap->GetTileScale().x , tilemap->GetTileScale().y };
		ImGui::InputInt2("Tile scale", aScale);
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
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::TILEMAP))
			{
				m_Components[i].Update();
			}
		}
	}

	void TilemapManager::Clear()
	{
	}

	void TilemapManager::Collect()
	{
	}

	Tilemap * TilemapManager::AddComponent(Entity entity)
	{
		auto& tilemap = GetComponentRef(entity);
		auto& tilemapInfo = GetComponentInfo(entity);

		if (!m_Engine.GetEntityManager()->HasComponent(entity, ComponentType::TRANSFORM2D))
			m_Engine.GetTransform2dManager()->AddComponent(entity);

		tilemapInfo.tilemap = &tilemap;

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

		if (CheckJsonExists(tilemapJson, "tile_scale") && CheckJsonParameter(tilemapJson, "tile_scale", nlohmann::detail::value_t::array))
		{
			Vec2f tileScale = Vec2f();
			tileScale.x = tilemapJson["tile_scale"][0].get<unsigned>();
			tileScale.y = tilemapJson["tile_scale"][1].get<unsigned>();
			newTilemap.SetTileScale(tileScale);
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
	}

	void TilemapManager::InitializeMap(Entity entity, std::vector<std::vector<TileTypeId>> tileTypeIds)
	{
		auto & tilemap = m_Components[entity - 1];
		EmptyMap(entity);

		if (!tileTypeIds.empty())
			tilemap.ResizeTilemap(Vec2f(tileTypeIds.size(), tileTypeIds[0].size()));

		EntityManager* entityManager = m_Engine.GetEntityManager();
		TileManager* tileManager = m_TilemapSystem->GetTileManager();

		//Assigning positions
		for (unsigned indexX = 0; indexX < tileTypeIds.size(); indexX++)
		{
			for (unsigned indexY = 0; indexY < tileTypeIds[indexX].size(); indexY++)
			{
				const Entity newEntity = entityManager->CreateEntity(INVALID_ENTITY);
				tileManager->AddComponent(newEntity, tileTypeIds[indexX][indexY]);
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
		const Vec2f mapSize = tilemap.GetSize();
		const auto& tiles = tilemap.GetTiles();

		const Vec2f basePos = m_Transform2dManager->GetComponentPtr(entity)->Position;
		const Vec2f tileScale = tilemap.GetTileScale();
		Vec2f xPos, yPos;

		//Layout of tilemap
		if (tilemap.GetIsometric())
		{
			xPos = { tileScale.x / 2.0f, tileScale.y / 2.0f };
			yPos = { -tileScale.x / 2.0f, tileScale.y / 2.0f };
		}
		else
		{
			xPos = { tileScale.x , 0 };
			yPos = { 0, tileScale.y };
		}
		for (int indexX = 0; indexX < mapSize.x; indexX++)
		{
			for (int indexY = 0; indexY < mapSize.y; indexY++)
			{
				const Entity tileEntity = tiles[indexX][indexY];
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
		const Vec2f mapSize = tilemap.GetSize();
		std::vector<std::vector<Entity>>& map = tilemap.GetTiles();
		std::vector<std::vector<TileTypeId>>& mapTiles = tilemap.GetTileTypes();

		EntityManager* entityManager = m_Engine.GetEntityManager();

		for (int i = 0; i < mapSize.x; i++)
		{
			for (int j = 0; j < mapSize.y; j++)
			{
				if(map[i][j] != INVALID_ENTITY)
				{
					entityManager->DestroyEntity(map[i][j]);
					map[i][j] = INVALID_ENTITY;
					map[i][j] = INVALID_TILE_TYPE;
				}
			}
		}
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