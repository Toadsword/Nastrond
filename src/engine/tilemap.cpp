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
	for (int indexX = 0; indexX < m_Size.x; indexX++)
	{
		for (int indexY = 0; indexY < m_Size.y; indexY++)
		{
			tile = m_TileManager->GetComponentPtr(m_Tiles[indexX][indexY]);
			if (tile != nullptr)
			{
				tile->Update();
			}
		}
	}
}

void Tilemap::SetSize(Vec2f newSize)
{
	if (newSize.x <= 0 || newSize.y <= 0)
		return;

	m_Size = newSize;
	m_Tiles = std::vector<std::vector<Entity>>{ (unsigned)newSize.x, std::vector<Entity>((unsigned)newSize.y)};
}

Vec2f Tilemap::GetSize()
{
	return m_Size;
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

std::vector<std::vector<Entity>>& Tilemap::GetTiles()
{
	return m_Tiles;
}

void Tilemap::AddTile(Vec2f pos, Entity entity)
{
	m_Tiles[pos.x][pos.y] = entity;
}

void editor::TilemapInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Tilemap");
	ImGui::InputInt("Layer", &layer);
	ImGui::Checkbox("isIsometric", &isIsometric);
	
	int aSize[2] = { size.x, size.y };
	ImGui::InputInt2("Size", aSize);
	int aScale[2] = { tileScale.x , tileScale.y };
	ImGui::InputInt2("tileScaleX", aScale);
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
			Log::GetInstance()->Error("Couldn't load timemap from path : '" + path + "'.");
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

	if (!m_Engine.GetEntityManager()->HasComponent(entity, ComponentType::TRANSFORM2D))
		m_Engine.GetTransform2dManager()->AddComponent(entity);

	if (CheckJsonExists(tilemapJson, "is_isometric") && CheckJsonParameter(tilemapJson, "is_isometric", nlohmann::detail::value_t::boolean))
	{
		newTilemap.SetIsometric(tilemapJson["is_isometric"].get<bool>());
		newTilemapInfo.isIsometric = tilemapJson["is_isometric"].get<bool>();
	}
	
	if (CheckJsonExists(tilemapJson, "layer") && CheckJsonParameter(tilemapJson, "layer", nlohmann::detail::value_t::number_integer))
	{
		newTilemap.SetLayer(tilemapJson["layer"].get<int>());
		newTilemapInfo.layer = tilemapJson["layer"].get<int>();
	}

	Vec2f tileScale = Vec2f();
	if (CheckJsonExists(tilemapJson, "tile_scale") && CheckJsonParameter(tilemapJson, "tile_scale", nlohmann::detail::value_t::array))
	{
		tileScale.x = tilemapJson["tile_scale"][0].get<unsigned>();
		tileScale.y = tilemapJson["tile_scale"][1].get<unsigned>();
		newTilemap.SetTileScale(tileScale);
		newTilemapInfo.tileScale = tileScale;
	}

	Vec2f mapSize = Vec2f();
	if (CheckJsonExists(tilemapJson, "map_size") && CheckJsonParameter(tilemapJson, "map_size", nlohmann::detail::value_t::array))
	{
		mapSize.x = tilemapJson["map_size"][0].get<unsigned>();
		mapSize.y = tilemapJson["map_size"][1].get<unsigned>();
		newTilemap.SetSize(mapSize);
		newTilemapInfo.size = mapSize;
	}

	if(CheckJsonExists(tilemapJson, "map") && CheckJsonParameter(tilemapJson, "map", nlohmann::detail::value_t::array))
	{
		InitializeMap(entity, tilemapJson["map"]);
	}
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
	auto & tilemap = m_Components[entity - 1];
	EmptyMap(entity);

	EntityManager* entityManager = m_Engine.GetEntityManager();
	TileManager* tileManager = m_TilemapSystem->GetTileManager();
	

	Vec2f basePos = m_Transform2dManager->GetComponentPtr(entity)->Position;
	Vec2f tileScale = tilemap.GetTileScale();
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
	//Assigning positions
	for (unsigned indexX = 0; indexX < map.size(); indexX++)
	{
		for (unsigned indexY = 0; indexY < map[indexX].size(); indexY++)
		{
			Entity newEntity = entityManager->CreateEntity(INVALID_ENTITY);
			tileManager->AddComponent(newEntity, map[indexX][indexY].get<int>());
			tilemap.AddTile(Vec2f(indexX, indexY), newEntity);

			const Vec2f newPos = basePos + xPos * indexX + yPos * indexY;

			m_Transform2dManager->GetComponentPtr(newEntity)->Position = newPos;
		}
	}

	if(map.size() > 0)
		tilemap.SetSize(Vec2f(map.size(), map[0].size()));
}

void TilemapManager::EmptyMap(Entity entity)
{
	auto & tilemap = m_Components[entity - 1];
	Vec2f mapSize = tilemap.GetSize();
	std::vector<std::vector<Entity>>& map = tilemap.GetTiles();

	EntityManager* entityManager = m_Engine.GetEntityManager();

	int sizeX = map.size();

	for (int i = 0; i < sizeX; i++)
	{
		int sizeY = map[i].size();
		for (int j = 0; j < sizeY; j++)
		{
			if(map[i][j] != INVALID_ENTITY)
			{	
				entityManager->DestroyEntity(map[i][j]);
				map[i][j] = INVALID_ENTITY;
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