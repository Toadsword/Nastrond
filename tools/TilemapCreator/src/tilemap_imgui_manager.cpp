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
#include <graphics/graphics2d.h>
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

		m_TileEditorId = m_EntityManager->CreateEntity(INVALID_ENTITY);
		m_EntityManager->GetEntityInfo(m_TileEditorId).name = "EditorTile";
		m_TileEditor = m_TilemapCreator->GetEngine().GetTilemapSystem()->GetTileManager()->AddComponent(m_TileEditorId);
	}

	void TilemapImguiManager::Update(float dt)
	{
		if (!m_IsInit)
			return;

		InputManager* inputManager = m_TilemapCreator->GetEngine().GetInputManager();

		//Keyboard
		KeyboardManager& keyboardManager = inputManager->GetKeyboardManager();

		const bool isControlHeld = keyboardManager.IsKeyHeld(sf::Keyboard::LControl) || keyboardManager.IsKeyHeld(sf::Keyboard::RControl);
		if (isControlHeld && keyboardManager.IsKeyDown(sf::Keyboard::S))
		{
			m_TilemapCreator->GetEngine().Save();
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
					m_FlagDisplayNewTileType = !m_FlagDisplayNewTileType;
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
		if (m_FlagDisplayNewTilemap)
		{
			//Bouton permettant d'ajouter une tilemap à la liste.
			ImGui::Text("New Tilemap");

			ImGui::InputInt2("Size : ", m_SizeNewTilemap);

			if (m_SizeNewTilemap[0] < 0)
				m_SizeNewTilemap[0] = 1;
			if (m_SizeNewTilemap[1] < 0)
				m_SizeNewTilemap[1] = 1;

			if (ImGui::Button("Add a tilemap"))
			{
				Entity newTilemapEntity = m_TilemapCreator
				                          ->GetEngine().GetEntityManager()->CreateEntity(INVALID_ENTITY);
				m_TilemapCreator->GetTilemapManager()->AddComponent(newTilemapEntity);

#ifdef Opti1
				const Vec2f newTilemapSize = Vec2f(m_SizeNewTilemap[0], m_SizeNewTilemap[1]);
				const std::vector<TileTypeId> newTiletypeIds = std::vector<TileTypeId>(m_SizeNewTilemap[0] * m_SizeNewTilemap[1], INVALID_TILE_TYPE);

				/*for (unsigned indexX = 0; indexX < newTilemapSize.x; indexX++)
				{
					for (unsigned indexY = 0; indexY < newTilemapSize.y; indexY++)
					{
						newTiletypeIds[indexX * m_SizeNewTilemap[0] + indexY] = INVALID_TILE_TYPE;
					}
				}*/

				m_TilemapCreator->GetTilemapManager()->InitializeMap(newTilemapEntity, newTiletypeIds, newTilemapSize);
#else
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
#endif
				m_FlagDisplayNewTilemap = false;
			}
			ImGui::Separator();
		}
		if (m_FlagDisplayNewTileType)
		{
			//Bouton permettant d'ajouter une tilemap à la liste.
			ImGui::Text("New TileType");

			ImGui::InputText("Sprite Location : ", m_filepathNewTileType, sizeof(m_filepathNewTileType));
			ImGui::Separator();

			if (ImGui::Button("Add tiletype"))
			{
				m_TilemapCreator->GetTileTypeManager()->AddNewTileType(m_filepathNewTileType);
				m_FlagDisplayNewTileType = false;
			}
		}

		auto components = m_TilemapCreator->GetTilemapManager()->GetComponents();
		if (ImGui::CollapsingHeader("List of Tilemap"))
		{
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
		}

		if (m_SelectedTilemap != INVALID_ENTITY)
			DrawTilemapInformations();

		ImGui::Separator();

		if (ImGui::CollapsingHeader("List of Tiletypes"))
		{
			std::vector<size_t> tileTypes = m_TilemapCreator->GetTileTypeManager()->GetAllTileTypeIds();
			for (size_t index : tileTypes)
			{
				if (index != INVALID_TILE_TYPE)
				{
					bool selectedOne = m_SelectedTileType == index;
					sf::Texture* texture = m_TilemapCreator->GetEngine().GetGraphics2dManager()->GetTextureManager()->GetTexture(
	                       m_TilemapCreator->GetTileTypeManager()->GetTextureFromTileType(tileTypes[index - 1])
						);
					float h = ImGui::GetTextLineHeightWithSpacing() * 2;
					float ratio = 1.0f * texture->getSize().x / texture->getSize().y;

					ImGui::Dummy(ImVec2(0.0f, h / 2));
					ImGui::Selectable(("Tile type " + std::to_string(index)).c_str(), &selectedOne);
					ImGui::SameLine();
					ImGui::Image(*texture, sf::Vector2f(h * ratio, h));
					if (selectedOne)
						m_SelectedTileType = index;
				}
			}
		}

		/* Displays the tileEditor on the right position on screen */
		if(m_SelectedTileType != INVALID_TILE_TYPE && m_SelectedTilemap != INVALID_TILE_TYPE)
		{
			m_TilemapCreator->GetTileTypeManager()->SetTileTexture(m_TileEditorId, m_SelectedTileType);
			
			Entity tilePosEntity = m_TilemapCreator->GetTilemapManager()->GetTileEntityFromMouse(m_SelectedTilemap);
			if(tilePosEntity != INVALID_ENTITY)
			{
				Transform2dManager* transform2dManager = m_TilemapCreator->GetEngine().GetTransform2dManager();
				transform2dManager->GetComponentPtr(m_TileEditorId)->Position = transform2dManager->GetComponentPtr(tilePosEntity)->Position;
				
				/*** Displays the coordinates of the cursor and which tile it points.

				ImGui::Separator();
				ImGui::Text(m_EntityManager->GetEntityInfo(tilePosEntity).name.c_str());
				int pos[2] = { transform2dManager->GetComponentPtr(tilePosEntity)->Position.x, transform2dManager->GetComponentPtr(tilePosEntity)->Position.y };
				ImGui::InputInt2("pos tile :", pos);

				int posMouse[2] = { m_TilemapCreator->GetEngine().GetInputManager()->GetMouseManager().GetWorldPosition().x, m_TilemapCreator->GetEngine().GetInputManager()->GetMouseManager().GetWorldPosition().y};
				ImGui::InputInt2("pos mouse :", posMouse);
				*/

				if(m_TilemapCreator->GetEngine().GetInputManager()->GetMouseManager().IsButtonDown(sf::Mouse::Left))
				{
					m_TilemapCreator->GetTilemapManager()->GetComponentPtr(m_SelectedTilemap)->SetTileAt(tilePosEntity, m_SelectedTileType);
					m_TilemapCreator->GetTileTypeManager()->SetTileTexture(tilePosEntity, m_SelectedTileType);
				}
			}
		}
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
				Vec2f currentSize = tilemap->GetTilemapSize();
				int aSize[2] = {currentSize.x, currentSize.y};
				ImGui::InputInt2("Tilemap Size", aSize);

				if ((aSize[0] != currentSize.x || aSize[1] != currentSize.y) && aSize[0] > 0 && aSize[1] > 0)
				{
#ifdef Opti1
					std::vector<TileTypeId> oldTileTypes = tilemap->GetTileTypes();
					const Vec2f oldSize = tilemap->GetTilemapSize();

					std::vector<TileTypeId> newTileTypeIds = std::vector<TileTypeId>(aSize[0] * aSize[1], INVALID_TILE_TYPE);

					for (unsigned indexX = 0; indexX < aSize[0]; indexX++)
					{
						for (unsigned indexY = 0; indexY < aSize[1]; indexY++)
						{
							if (indexX < oldSize.x && indexY < oldSize.y)
								newTileTypeIds[indexX * aSize[1] + indexY] = oldTileTypes[indexX * oldSize.y + indexY];
							//else
							//	newTileTypeIds[indexX * aSize[0] + indexY] = INVALID_TILE_TYPE;
						}
					}
					m_TilemapCreator->GetTilemapManager()->InitializeMap(m_SelectedTilemap, newTileTypeIds, Vec2f(aSize[0], aSize[1]));
#else
					std::vector<std::vector<TileTypeId>> oldTileTypes = tilemap->GetTileTypes();
					const Vec2f oldSize = tilemap->GetTilemapSize();

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
#endif
				}
			}
			{
				Vec2f currentSize = tilemap->GetTileSize();
				int aScale[2] = { currentSize.x, currentSize.y};
				ImGui::InputInt2("Tile scale", aScale);

				if (aScale[0] != currentSize.x || aScale[1] != currentSize.y)
				{
					tilemap->SetTileSize(Vec2f(aScale[0], aScale[1]));
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

	void TilemapImguiManager::DrawTileTypeInformations()
	{
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("TiletypeInfo"))
		{
		}
	}
}
