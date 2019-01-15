#include <engine\tilemap.h>
#include <graphics/texture.h>
#include <engine/tile_asset.h>

#include <imgui.h>

void sfge::Tilemap::Init()
{
}

void sfge::Tilemap::Update()
{
}

void sfge::editor::TilemapInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Tile");
	ImGui::InputInt("Layer", (int*)layer);
	ImGui::InputFloat("Size%x", &size.x);
	ImGui::SameLine();
	ImGui::InputFloat("Size%y", &size.y);
}

void sfge::TilemapManager::Init()
{
	m_Transform2dManager = m_Engine.GetTransform2dManager();
}

void sfge::TilemapManager::Update(float dt)
{
}

void sfge::TilemapManager::Collect()
{
}

sfge::Tilemap * sfge::TilemapManager::AddComponent(Entity entity)
{
	auto& tilemap = GetComponentRef(entity);
	auto& tilemapInfo = GetComponentInfo(entity);

	//tilemap.SetTransform(m_Transform2dManager->GetComponentPtr(entity));
	tilemapInfo.tilemap = &tilemap;

	m_EntityManager->AddComponentType(entity, ComponentType::TILEMAP);
	return &tilemap;
}

void sfge::TilemapManager::CreateComponent(json & componentJson, Entity entity)
{
}

void sfge::TilemapManager::DestroyComponent(Entity entity)
{
	(void) entity;
}

void sfge::TilemapManager::OnResize(size_t new_size)
{
	m_Components.resize(new_size);
	m_ComponentsInfo.resize(new_size);
}

void sfge::TilemapSystem::Init()
{
}

void sfge::TilemapSystem::Update(float dt)
{
	m_TilemapManager.Update(dt);
}

void sfge::TilemapSystem::Destroy()
{
	Clear();
	Collect();
}

void sfge::TilemapSystem::Clear()
{
	m_TilemapManager.Clear();
	m_TileManager.Clear();
	m_TileTypeManager.Clear();
}

void sfge::TilemapSystem::Collect()
{
	m_TilemapManager.Collect();
	m_TileManager.Collect();
	m_TileTypeManager.Collect();
}

sfge::TilemapManager * sfge::TilemapSystem::GetTilemapManager()
{
	return &m_TilemapManager;
}

sfge::TileManager * sfge::TilemapSystem::GetTileManager()
{
	return &m_TileManager;
}

sfge::TileTypeManager * sfge::TilemapSystem::GetTileTypeManager()
{
	return &m_TileTypeManager;
}
