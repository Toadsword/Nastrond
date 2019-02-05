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

#include <engine/tile.h>
#include <engine/tilemap.h>
#include <engine/tile_asset.h>
#include <graphics/texture.h>
#include <graphics/graphics2d.h>

namespace sfge 
{

Tile::Tile()
{

}

void Tile::Update()
{
}

void Tile::SetParentTilemap(Entity parent)
{
	m_ParentTilemapEntity = parent;
}

Entity Tile::GetParentTilemap()
{
	return m_ParentTilemapEntity;
}

void Tile::SetLayer(int newLayer)
{
	if (newLayer > 0)
		m_Layer = newLayer;
}

int Tile::GetLayer()
{
	return m_Layer;
}

void Tile::SetType(TileTypeId newType)
{
	m_Type = newType;
}

TileTypeId Tile::GetType()
{
	return m_Type;
}

void editor::TileInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Tile");
	int layer = tile->GetLayer();
	ImGui::InputInt("Layer", &layer);
	int type = tile->GetType();
	ImGui::InputInt("Type", &type);
}

void TileManager::Init()
{
	SingleComponentManager::Init();
	m_Transform2dManager = m_Engine.GetTransform2dManager();
	m_TilemapManager = m_Engine.GetTilemapSystem()->GetTilemapManager();
}

void TileManager::Update(float dt)
{

}

void TileManager::Collect()
{
}

Tile * TileManager::AddComponent(Entity entity)
{
	auto& tile = GetComponentRef(entity);
	auto& tileInfo = GetComponentInfo(entity);

	if (!m_Engine.GetEntityManager()->HasComponent(entity, ComponentType::TRANSFORM2D))
		m_Engine.GetTransform2dManager()->AddComponent(entity);

	tileInfo.tile = &tile;

	m_EntityManager->AddComponentType(entity, ComponentType::TILE);
	return &tile;
}

Tile * TileManager::AddComponent(Entity entity, TileTypeId tileType)
{
	auto* tile = AddComponent(entity);
	auto& tileInfo = GetComponentInfo(entity);
	
	tileInfo.tile = tile;

	tile->SetType(tileType);
	m_Engine.GetTilemapSystem()->GetTileTypeManager()->SetTileTexture(entity, tileType);

	return tile;
}

void TileManager::CreateComponent(Entity entity, TileTypeId tileType)
{
	auto & newTile = m_Components[entity - 1];
	auto & newTileInfo = m_ComponentsInfo[entity - 1];

	m_Engine.GetTilemapSystem()->GetTileTypeManager()->SetTileTexture(tileType, entity);
}

void TileManager::CreateComponent(json& componentJson, Entity entity)
{
}

void TileManager::DestroyComponent(Entity entity)
{
	(void)entity;
}

void TileManager::OnResize(size_t new_size)
{
	m_Components.resize(new_size);
	m_ComponentsInfo.resize(new_size);
}
}