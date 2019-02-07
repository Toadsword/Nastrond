/*
MIT License

Copyright (c) 2018 SAE Institute Switzerland AG

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

----------------------------
Author : Duncan Bourquard (bourquardduncan@gmail.com)
Date : February 2019
Project : TilemapCreator for SFGE
*/
#include <tilemap_imgui_manager.h>

#include <engine/engine.h>
#include <tilemap_creator.h>

#include <imgui.h>
#include <imgui-SFML.h>

#include <input/input.h>

namespace sfge::tools
{
	void TilemapImguiManager::Init(TilemapCreator* engine)
	{
		m_TilemapCreator = engine;
		m_EntityManager = engine->GetEngine().GetEntityManager();
		m_IsInit = true;
	}

	void TilemapImguiManager::Update(float dt)
	{
		if (!m_IsInit)
			return;

		InputManager* inputManager = m_TilemapCreator->GetEngine().GetInputManager();

		//Keyboard
		KeyboardManager& keyboardManager = inputManager->GetKeyboardManager();
		
		bool isControlHeld = keyboardManager.IsKeyHeld(sf::Keyboard::LControl) || keyboardManager.IsKeyHeld(sf::Keyboard::RControl);
		if(isControlHeld && keyboardManager.IsKeyDown(sf::Keyboard::S))
		{
			//m_SaveResult = m_AnimCreator->GetAnimationManager()->ExportToJson(m_AnimCreator->GetTextureManager()->GetAllTextures());
			//m_OpenModalSave = m_SaveResult != SAVE_SUCCESS;
		}
	}

	void TilemapImguiManager::Draw()
	{
		if (!m_IsInit)
			return;

		//ImGui::ShowDemoWindow();
		
		ImGui::SetNextWindowPos(ImVec2(100.0f, 100.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(TILEMAP_WINDOW_WIDTH, TILEMAP_WINDOW_HEIGHT), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(TILEMAP_WINDOW_NAME, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
		{
			DisplayMenuWindow();
			DrawMainWindow();
		}
		ImGui::End();
	}

	void TilemapImguiManager::DisplayMenuWindow()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					Entity newEntity = m_EntityManager->CreateEntity(INVALID_ENTITY);
					m_TilemapCreator->GetTilemapManager()->AddComponent(newEntity);
				}
				if (ImGui::MenuItem("Add new Tiletype"))
				{
					//m_OpenAddTexture = true;
				}
				if (ImGui::MenuItem("Add new Tilemap"))
				{
					m_FlagDisplayNewTilemap = !m_FlagDisplayNewTilemap;
				}
				if (ImGui::MenuItem("Save current..", "Ctrl+S"))
				{
					//m_SaveResult = m_AnimCreator->GetAnimationManager()->ExportToJson(m_AnimCreator->GetTextureManager()->GetAllTextures());
					//m_OpenModalSave = m_SaveResult != SAVE_SUCCESS;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void TilemapImguiManager::DrawMainWindow()
	{

		if(m_FlagDisplayNewTilemap)
		{
			//Bouton permettant d'ajouter une tilemap à la liste.
			ImGui::Text("New Tilemap");

			ImGui::InputInt2("Size : ", m_SizeNewTilemap);

			if (m_SizeNewTilemap[0] < 0)
				m_SizeNewTilemap[0] = 1;
			if (m_SizeNewTilemap[1] < 0)
				m_SizeNewTilemap[1] = 1;
				
			if(ImGui::Button("Add a tilemap"))
			{
				Entity newTilemapEntity = m_TilemapCreator->GetEngine().GetEntityManager()->CreateEntity(INVALID_ENTITY);
				m_TilemapCreator->GetTilemapManager()->AddComponent(newTilemapEntity);

				std::vector<std::vector<TileTypeId>> newTiletypeIds = std::vector<std::vector<TileTypeId>>{
					static_cast<unsigned>(m_SizeNewTilemap[0]),
					std::vector<TileTypeId>(static_cast<unsigned>(m_SizeNewTilemap[1]))
				};

				for (unsigned indexX = 0; indexX < newTiletypeIds.size(); indexX++)
				{
					for (unsigned indexY = 0; indexY < newTiletypeIds[indexX].size(); indexY++)
					{
						newTiletypeIds[indexX][indexY] = INVALID_TILE_TYPE;
					}
				}

				m_TilemapCreator->GetTilemapManager()->InitializeMap(newTilemapEntity, newTiletypeIds);
				m_FlagDisplayNewTilemap = false;
			}
			ImGui::Separator();
		}

		auto components = m_TilemapCreator->GetTilemapManager()->GetComponents();
		ImGui::Text("List of Tilemap");
		for (auto i = 0u; i < components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::TILEMAP))
			{
				bool selectedOne = m_SelectedTilemap == i + 1;
				ImGui::Selectable(m_EntityManager->GetEntityInfo(i + 1).name.c_str(), &selectedOne);
				if (selectedOne)
					m_SelectedTilemap = i + 1;
			}
		}
		ImGui::Separator();

		if (m_SelectedTilemap != INVALID_ENTITY)
			DrawTilemapInformations();
	}

	void TilemapImguiManager::DrawTilemapInformations()
	{
		Tilemap* tilemap = m_TilemapCreator->GetTilemapManager()->GetComponentPtr(m_SelectedTilemap);
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("TilemapInfo"))
		{
			{
				bool isIso = tilemap->GetIsometric();
				ImGui::Checkbox("Is Isometric", &isIso);

				if (isIso != tilemap->GetIsometric())
				{
					tilemap->SetIsometric(isIso);
					m_TilemapCreator->GetTilemapManager()->SetupTilePosition(m_SelectedTilemap);
				}
			}
			{
				Vec2f currentSize = tilemap->GetSize();
				int aSize[2] = { currentSize.x , currentSize.y };
				ImGui::InputInt2("Tilemap Size", aSize);

				if ((aSize[0] != currentSize.x || aSize[1] != currentSize.y) && aSize[0] > 0 && aSize[1] > 0)
				{
					std::vector<std::vector<TileTypeId>> oldTileTypes = tilemap->GetTileTypes();
					const Vec2f oldSize = tilemap->GetSize();

					std::vector<std::vector<TileTypeId>> newTileTypeIds = std::vector<std::vector<TileTypeId>>{
						static_cast<unsigned>(aSize[0]),
						std::vector<TileTypeId>(static_cast<unsigned>(aSize[1]))
					};
					for (unsigned indexX = 0; indexX < aSize[0]; indexX++)
					{
						for (unsigned indexY = 0; indexY < aSize[1]; indexY++)
						{
							if (indexX < oldSize.x && indexY < oldSize.y)
								newTileTypeIds[indexX][indexY] = oldTileTypes[indexX][indexY];
							else
								newTileTypeIds[indexX][indexY] = INVALID_TILE_TYPE;
						}
					}
					m_TilemapCreator->GetTilemapManager()->InitializeMap(m_SelectedTilemap, newTileTypeIds);
				}
			}
			{
				Vec2f currentScale = tilemap->GetTileScale();
				int aScale[2] = { currentScale.x , currentScale.y };
				ImGui::InputInt2("Tile scale", aScale);

				if (aScale[0] != currentScale.x || aScale[1] != currentScale.y)
				{
					tilemap->SetTileScale(Vec2f(aScale[0], aScale[1]));
					m_TilemapCreator->GetTilemapManager()->SetupTilePosition(m_SelectedTilemap);
				}
			}
			{
				int aLayer = tilemap->GetLayer();
				ImGui::InputInt("Layer", &aLayer);
				if (aLayer != tilemap->GetLayer())
					tilemap->SetLayer(aLayer);
			}
		}
	}
}
