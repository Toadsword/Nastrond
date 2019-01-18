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
}

void Tilemap::SetSize(sf::Vector2<unsigned> newSize)
{
	if (newSize.x <= 0 || newSize.y <= 0)
		return;

	m_Size = newSize;
	m_Tiles = std::vector<std::vector<Entity>>{ newSize.x, std::vector<Entity>(newSize.y)};
}

sf::Vector2<unsigned> Tilemap::GetSize()
{
	return m_Size;
}

void Tilemap::SetLayer(short newLayer)
{
	if(newLayer > 0)
		m_Layer = newLayer;
}

short Tilemap::GetLayer()
{
	return m_Layer;
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
	ImGui::Text("Tile");
	ImGui::InputInt("Layer", (int*)layer);
	ImGui::InputInt("Size%x", (int*)size.x);
	ImGui::SameLine();
	ImGui::InputInt("Size%y", (int*)size.y);
}

void TilemapManager::Init()
{
	m_Transform2dManager = m_Engine.GetTransform2dManager();
}

void TilemapManager::Update(float dt)
{
	auto newPosition =
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

	//tilemap.SetTransform(m_Transform2dManager->GetComponentPtr(entity));
	tilemapInfo.tilemap = &tilemap;

	m_EntityManager->AddComponentType(entity, ComponentType::TILEMAP);
	return &tilemap;
}

void TilemapManager::CreateComponent(json & componentJson, Entity entity)
{
	auto & newTilemap = m_Components[entity - 1];
	auto & newTilemapInfo = m_ComponentsInfo[entity - 1];

	if (CheckJsonExists(componentJson, "layer") && CheckJsonParameter(componentJson, "layer", nlohmann::detail::value_t::number_integer))
	{
		newTilemap.SetLayer(componentJson["layer"].get<short>());
		newTilemapInfo.layer = componentJson["layer"].get<short>();
	}

	sf::Vector2<unsigned> mapSize = sf::Vector2<unsigned>();
	if (CheckJsonExists(componentJson, "size") && CheckJsonParameter(componentJson, "size", nlohmann::detail::value_t::array))
	{
		mapSize.x = componentJson["size"][0].get<unsigned>();
		mapSize.y = componentJson["size"][1].get<unsigned>();
		newTilemap.SetSize(mapSize);
		newTilemapInfo.size = mapSize;
	}

	if(CheckJsonExists(componentJson, "map") && CheckJsonParameter(componentJson, "map", nlohmann::detail::value_t::array))
	{
		InitializeMap(entity, componentJson);
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

	sf::Vector2<unsigned> mapSize = tilemap.GetSize();
	EntityManager* entityManager = m_Engine.GetEntityManager();

	for (int i = 0; i < mapSize.x; i++)
	{
		for (int j = 0; j < mapSize.y; j++)
		{
			Entity newEntity = entityManager->CreateEntity(0);
			m_TileManager->AddComponent(newEntity, map[i][j].get<int>());
		}
	}
}

void TilemapManager::EmptyMap(Entity entity)
{
	auto & tilemap = m_Components[entity - 1];
	sf::Vector2<unsigned> mapSize = tilemap.GetSize();
	std::vector<std::vector<Entity>>& map = tilemap.GetTiles();

	EntityManager* entityManager = m_Engine.GetEntityManager();

	for (int i = 0; i <= mapSize.x; i++)
	{
		for (int j = 0; j <= mapSize.y; j++)
		{
			entityManager->DestroyEntity(map[i][j]);
			map[i][j] = INVALID_ENTITY;
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
	m_TilemapManager.Update(dt);
	//m_TileManager.Update(dt);
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