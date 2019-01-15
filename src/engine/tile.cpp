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

#include <engine/tile.h>
#include <engine/tile_asset.h>
#include <graphics/texture.h>

#include <imgui.h>

namespace sfge {

Entity Tile::GetParentTilemap()
{
	return m_ParentTilemapEntity;
}

void editor::TileInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Tile");
	ImGui::InputInt("Layer", (int*)layer);
	ImGui::InputInt("Type", (int*)&type);
}

void TileManager::Init()
{
	m_Transform2dManager = m_Engine.GetTransform2dManager();
}

void TileManager::Collect()
{
}

Tile * TileManager::AddComponent(Entity entity)
{
	auto& tile = GetComponentRef(entity);
	auto& tileInfo = GetComponentInfo(entity);

	tile.SetTransform(m_Transform2dManager->GetComponentPtr(entity));
	tileInfo.tile = &tile;

	m_EntityManager->AddComponentType(entity, ComponentType::TILE);
	return &tile;
}

void TileManager::CreateComponent(TileTypeId tileType, Entity entity)
{
	auto & newTile = m_Components[entity - 1];
	auto & newTileInfo = m_ComponentsInfo[entity - 1];

	m_Engine.GetTilemapSystem()->GetTileTypeManager()->SetTileTexture(tileType, entity);
}

void TileManager::CreateComponent(json & componentJson, Entity entity)
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