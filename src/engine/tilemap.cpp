#include <engine\tilemap.h>
#include <graphics/texture.h>
#include <engine/tile_asset.h>

#include <imgui.h>
namespace sfge
{

void Tilemap::Init()
{
}

void Tilemap::Update()
{
}

void editor::TilemapInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Tile");
	ImGui::InputInt("Layer", (int*)layer);
	ImGui::InputFloat("Size%x", &size.x);
	ImGui::SameLine();
	ImGui::InputFloat("Size%y", &size.y);
}

void TilemapManager::Init()
{
	m_Transform2dManager = m_Engine.GetTransform2dManager();
}

void TilemapManager::Update(float dt)
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
}

void TilemapManager::DestroyComponent(Entity entity)
{
	(void)entity;
}

void TilemapManager::OnResize(size_t new_size)
{
	m_Components.resize(new_size);
	m_ComponentsInfo.resize(new_size);
}

void TilemapSystem::Init()
{
}

void TilemapSystem::Update(float dt)
{
	m_TilemapManager.Update(dt);
}

void TilemapSystem::Destroy()
{
	Clear();
	Collect();
}

void TilemapSystem::Clear()
{
	m_TilemapManager.Clear();
	m_TileManager.Clear();
	m_TileTypeManager.Clear();
}

void TilemapSystem::Collect()
{
	m_TilemapManager.Collect();
	m_TileManager.Collect();
	m_TileTypeManager.Collect();
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