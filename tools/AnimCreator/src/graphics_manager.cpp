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
Date : November 2018
Project : AnimationTool for SFGE
*/

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <engine/engine.h>
#include <input/input.h>

#include <graphics_manager.h>
#include <anim_creator.h>
#include <iostream>

namespace sfge::tools
{
	void GraphicsManager::Init(AnimCreator* engine)
	{
		m_AnimCreator = engine;
		m_isInit = true;
	}

	void GraphicsManager::Update(float dt)
	{
		if (!m_isInit)
			return;

		InputManager* inputManager = m_AnimCreator->GetEngine().GetInputManager();

		//Keyboard
		KeyboardManager& keyboardManager = inputManager->GetKeyboardManager();
		
		bool isControlHeld = keyboardManager.IsKeyHeld(sf::Keyboard::LControl) || keyboardManager.IsKeyHeld(sf::Keyboard::RControl);
		if(isControlHeld && keyboardManager.IsKeyDown(sf::Keyboard::S))
		{
			m_saveResult = m_AnimCreator->GetAnimationManager()->ExportToJson(m_AnimCreator->GetTextureManager()->GetAllTextures());
			m_openModalSave = m_saveResult != SAVE_SUCCESS;
		}
		if(isControlHeld && keyboardManager.IsKeyDown(sf::Keyboard::P))
			m_doPlayAnimation = !m_doPlayAnimation;
		if (isControlHeld && keyboardManager.IsKeyDown(sf::Keyboard::L))
			m_AnimCreator->GetAnimationManager()->SetIsLooped(!m_AnimCreator->GetAnimationManager()->GetIsLooped());
		if (isControlHeld && keyboardManager.IsKeyDown(sf::Keyboard::Add))
			m_AnimCreator->GetAnimationManager()->AddOrInsertKey();
		if (isControlHeld && keyboardManager.IsKeyDown(sf::Keyboard::Subtract))
			m_AnimCreator->GetAnimationManager()->RemoveKey();

		//Mouse
		MouseManager& mouseManager = inputManager->GetMouseManager();
		m_timeSinceLastClick += dt;
		if(mouseManager.IsButtonDown(sf::Mouse::Left))
		{
			if (m_timeSinceLastClick < TIME_TO_DOUBLE_CLICK)
				m_doubleClicked = true;
			else
				m_doubleClicked = false;
			m_timeSinceLastClick = 0;			
		}

		//Animations
		auto animManager = m_AnimCreator->GetAnimationManager();
		if (m_doPlayAnimation)
		{
			m_elapsedTimeSinceNewFrame += dt;
			if (m_elapsedTimeSinceNewFrame >= animManager->GetSpeed())
			{
				m_elapsedTimeSinceNewFrame = 0;
				m_currentFrame++;
				if (m_currentFrame > animManager->GetHighestKeyNum())
				{
					m_currentFrame = 0;
					if (!animManager->GetIsLooped())
						m_doPlayAnimation = false;
				}
			}
		}
		if(m_fileImportResult != LOAD_NONE)
		{
			m_lastTimeTextureLoaded += dt;
			if (m_lastTimeTextureLoaded >= TIME_BEFORE_REFRESH_LOADED_TEXTURE_STATUS)
			{
				m_fileImportResult = LOAD_NONE;
				m_lastTimeTextureLoaded = 0;
			}
		}

		//ImGui::ShowDemoWindow();
	}

	void GraphicsManager::Draw()
	{
		if (!m_isInit)
			return;

		ImGui::SetNextWindowPos(ImVec2(150.0f, 100.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 600.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(WINDOW_NAME, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
		{
			DisplayMenuWindow();
			DisplayFileWindow();
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(750.0f, 100.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(450.0f, 300.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("PreviewWindow", NULL, ImGuiWindowFlags_NoCollapse))
		{
			DisplayPreviewWindow();
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(750.0f, 400.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(450.0f, 300.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("FrameInfoWindow", NULL, ImGuiWindowFlags_NoCollapse))
		{
			DisplayAnimationInformationsWindow();
		}
		ImGui::End();
		if (m_openModalSave) OpenModalSave();
		if (m_openModalConfirmNew) OpenModalConfirmNew();
	}

	void GraphicsManager::DisplayMenuWindow()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					m_openModalConfirmNew = true;
				}
				if (ImGui::MenuItem("Open new sprite", "Ctrl+O"))
				{
					m_openAddTexture = true;
				}
				if (ImGui::MenuItem("Save current..", "Ctrl+S"))
				{
					m_saveResult = m_AnimCreator->GetAnimationManager()->ExportToJson(m_AnimCreator->GetTextureManager()->GetAllTextures());
					m_openModalSave = m_saveResult != SAVE_SUCCESS;
				}
				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{
					//m_AnimCreator->ExitApplication();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void GraphicsManager::DisplayFileWindow()
	{
		// Texture add section
		ImGui::Text("Add a new sprite...");
		ImGui::Separator();

		ImGui::InputText("FilePath (from C:)", m_inputNameNewFile, IM_ARRAYSIZE(m_inputNameNewFile));

		ImGui::Columns(4, "idColAddSprite");
		ImGui::Text("Size X"); ImGui::NextColumn();
		ImGui::InputInt("##Size X", &m_inputSizeX); ImGui::NextColumn();

		ImGui::Text("Size Y"); ImGui::NextColumn();
		ImGui::InputInt("##Size Y", &m_inputSizeY); ImGui::NextColumn();

		ImGui::Text("Num Rows"); ImGui::NextColumn();
		ImGui::InputInt("##Num Rows", &m_inputNumRows); ImGui::NextColumn();

		ImGui::Text("Num Columns"); ImGui::NextColumn();
		ImGui::InputInt("##Num Columns", &m_inputNumCols); ImGui::NextColumn();

		ImGui::Text("Offset x"); ImGui::NextColumn();
		ImGui::InputInt("##Offset x", &m_inputOffsetX); ImGui::NextColumn();

		ImGui::Text("Offset y"); ImGui::NextColumn();
		ImGui::InputInt("##Offset y", &m_inputOffsetY); ImGui::NextColumn();

		ImGui::Columns(1, "idColAddSprite");

		auto textManager = m_AnimCreator->GetTextureManager();
		if (ImGui::Button("Load"))
		{
			m_lastIdBeforeNewTextLoad = m_AnimCreator->GetTextureManager()->GetLastId();
			m_fileImportResult = textManager->LoadTexture(m_inputNameNewFile, m_inputSizeX, m_inputSizeY, m_inputNumRows, m_inputNumCols, m_inputOffsetX, m_inputOffsetY);
			if(m_fileImportResult == LOAD_FAILURE && m_lastIdBeforeNewTextLoad == 0)
				m_lastIdBeforeNewTextLoad = -1;			
		}
		if (m_lastIdBeforeNewTextLoad != -1)
		{
			ImGui::SameLine();
			if (ImGui::Button("Undo"))
			{
				int currentId = m_AnimCreator->GetTextureManager()->GetLastId();
				for (int i = m_lastIdBeforeNewTextLoad; i <= currentId; i++)
					textManager->RemoveTexture(i);

				m_lastIdBeforeNewTextLoad = -1;
				m_lastTimeTextureLoaded = 0;
				m_fileImportResult = LOAD_NONE;
			}
		}

		if (m_fileImportResult == LOAD_SUCCESS)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Textures loaded successfully");
		}
		else if (m_fileImportResult == LOAD_FAILURE)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error while loading file : Does it really exists?");
		}

		ImGui::Separator();

		// All sprites section
		ImGui::Columns(2);
		ImGui::Text("All Sprites");
		ImGui::Separator();

		ImGui::NextColumn();
		ImGui::NextColumn();
		auto* loadedTextures = m_AnimCreator->GetTextureManager()->GetAllTextures();
		if (loadedTextures->empty())
		{
			ImGui::Columns(1);
			ImGui::Text("There are currently no sprites in here...");
			ImGui::Text("Begin by adding some sprites !");
			ImGui::Columns(2);
		}

		for (auto* texture : *loadedTextures)
		{
			if (ImGui::Button((std::to_string(texture->id) + " : " + texture->fileName).c_str(), ImVec2(125, 0)))
			{
				m_SelectedTextureId = texture->id;
				if (m_doubleClicked)
				{
					if (m_currentFrame == -1)
						m_AnimCreator->GetAnimationManager()->AddOrInsertKey(m_AnimCreator->GetAnimationManager()->GetHighestKeyNum(), texture->id);
					else
						m_AnimCreator->GetAnimationManager()->AddOrInsertKey(m_currentFrame, texture->id);
				}
			}
			if (ImGui::IsItemHovered())
			{
				ImVec2 m = ImGui::GetIO().MousePos;

				ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y + 10));
				ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
				ImGui::Text("click : Select texture.");
				ImGui::Text("dblclick : Assign texture.");
				ImGui::Text(("path : " + texture->path).c_str());
				ImGui::End();
			}
			ImGui::SameLine();
			if (ImGui::Button((" + ##TextureId" + std::to_string(texture->id)).c_str(), ImVec2(35, 0)))
			{
				auto animManager = m_AnimCreator->GetAnimationManager();
				if(animManager->GetHighestKeyNum() == 0 && animManager->GetTextureIdFromKeyframe(0) == -1)
					animManager->AddOrInsertKey(animManager->GetHighestKeyNum(), texture->id);
				else
					animManager->AddOrInsertKey(animManager->GetHighestKeyNum() + 1, texture->id);

				m_currentFrame = animManager->GetHighestKeyNum();
			}
			if (ImGui::IsItemHovered())
			{
				ImVec2 m = ImGui::GetIO().MousePos;

				ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y + 10));
				ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
				ImGui::Text("Add a frame with this texture.");
				ImGui::End();
			}

			/*
			ImGui::SameLine();
			if (ImGui::Button((" - ##TextureId" + std::to_string(texture->id)).c_str(), ImVec2(35, 0)))
			{
				auto textManager = m_AnimCreator->GetTextureManager();
				textManager->RemoveTexture(texture->id);
				continue;
			}
			if (ImGui::IsItemHovered())
			{
				ImVec2 m = ImGui::GetIO().MousePos;

				ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y + 10));
				ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
				ImGui::Text("Remove this texture from here.");
				ImGui::End();
			}
			*/

			m_AnimCreator->GetTextureManager()->DisplayTexture(texture->id, m_SelectedTextureId == texture->id);
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
	}

	void GraphicsManager::DisplayPreviewWindow()
	{
		auto animManager = m_AnimCreator->GetAnimationManager();
		ImGui::SliderInt("All Frames", &m_currentFrame, 0, std::max(0, animManager->GetHighestKeyNum()));

		ImGui::Columns(9, "colsEditorBtns", false);
		//Frame at the beginning
		if (ImGui::Button(" <<+ "))
		{
			animManager->AddOrInsertKey();
			m_currentFrame = 0;
		}
		if (ImGui::IsItemHovered())
		{
			ImVec2 m = ImGui::GetIO().MousePos;
			ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y + 10));
			ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("Add new frame at the end.");
			ImGui::End();
		}
		ImGui::NextColumn();
		if (ImGui::Button(" <<- "))
			animManager->RemoveKey(0);

		if (ImGui::IsItemHovered())
		{
			ImVec2 m = ImGui::GetIO().MousePos;
			ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y + 10));
			ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("Remove last frame.");
			ImGui::End();
		}
		ImGui::NextColumn();

		//Frame before/after
		if (ImGui::Button(" <+ "))
		{
			animManager->AddNewKey(m_currentFrame - 1);
			m_currentFrame--;
		}
		if (ImGui::IsItemHovered())
		{
			ImVec2 m = ImGui::GetIO().MousePos;
			ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y + 10));
			ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("Add a frame before the current one.");
			ImGui::End();
		}
		ImGui::NextColumn();

		if (ImGui::Button(" < "))
		{
			animManager->SwapKeyTextures(m_currentFrame, m_currentFrame - 1);
			m_currentFrame--;
			if (m_currentFrame < 0)
				m_currentFrame = 0;
		}
		if (ImGui::IsItemHovered())
		{
			ImVec2 m = ImGui::GetIO().MousePos;
			ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y + 10));
			ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("Move current frame to the left.");
			ImGui::End();
		}
		ImGui::NextColumn();
		if (!m_doPlayAnimation)
		{
			if (ImGui::Button("Play", ImVec2(40, 20)))
			{
				m_doPlayAnimation = true;
				m_elapsedTimeSinceNewFrame = 0;
			}
		}
		else
		{
			if (ImGui::Button("Pause", ImVec2(40, 20)))
				m_doPlayAnimation = false;
		}
		ImGui::NextColumn();

		if (ImGui::Button(" > "))
		{
			animManager->SwapKeyTextures(m_currentFrame, m_currentFrame + 1);
			m_currentFrame++;
			if (m_currentFrame > animManager->GetHighestKeyNum())
				m_currentFrame = animManager->GetHighestKeyNum();
		}
		if (ImGui::IsItemHovered())
		{
			ImVec2 m = ImGui::GetIO().MousePos;
			ImGui::SetNextWindowPos(ImVec2(m.x - 200, m.y + 10));
			ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("Move current frame to the right.");
			ImGui::End();
		}
		ImGui::NextColumn();

		if (ImGui::Button(" +> "))
		{
			animManager->AddNewKey(m_currentFrame + 1);
			m_currentFrame++;
		}
		if (ImGui::IsItemHovered())
		{
			ImVec2 m = ImGui::GetIO().MousePos;
			ImGui::SetNextWindowPos(ImVec2(m.x - 200, m.y + 10));
			ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("Add a frame after the current one.");
			ImGui::End();
		}

		//Frame at the end
		ImGui::NextColumn();
		if (ImGui::Button(" ->> "))
		{
			animManager->RemoveKey();
			if (m_currentFrame > animManager->GetHighestKeyNum())
				m_currentFrame = animManager->GetHighestKeyNum();
		}
		if (ImGui::IsItemHovered())
		{
			ImVec2 m = ImGui::GetIO().MousePos;
			ImGui::SetNextWindowPos(ImVec2(m.x - 200, m.y + 10));
			ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("Remove last frame.");
			ImGui::End();
		}

		ImGui::NextColumn();
		if (ImGui::Button(" +>> "))
		{
			animManager->AddOrInsertKey();
			m_currentFrame = animManager->GetHighestKeyNum();
		}
		if (ImGui::IsItemHovered())
		{
			ImVec2 m = ImGui::GetIO().MousePos;
			ImGui::SetNextWindowPos(ImVec2(m.x - 200, m.y + 10));
			ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("Add new frame at the end.");
			ImGui::End();
		}
		ImGui::Columns(1, "colsEditorBtns", false);

		//Display Image
		auto idCurrentTexture = animManager->GetTextureIdFromKeyframe(m_currentFrame);
		if (idCurrentTexture > -1)
		{
			m_AnimCreator->GetTextureManager()->DisplayTexture(idCurrentTexture);
		}
	}

	void GraphicsManager::DisplayAnimationInformationsWindow()
	{
		auto animManager = m_AnimCreator->GetAnimationManager();

		//***** BUTTONS *****//
		if (ImGui::Button("Assign selected texture") && m_SelectedTextureId != -1)
		{
			animManager->AddOrInsertKey(m_currentFrame, m_SelectedTextureId);
		}
		if (ImGui::IsItemHovered())
		{
			ImVec2 m = ImGui::GetIO().MousePos;

			ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y + 10));
			ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("Currently selected texture : ");
			m_AnimCreator->GetTextureManager()->DisplayTexture(m_SelectedTextureId);
			ImGui::End();
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove current frame"))
		{
			animManager->RemoveKey(m_currentFrame);
			if (m_currentFrame > animManager->GetHighestKeyNum())
				m_currentFrame = animManager->GetHighestKeyNum();
		}

		//***** FILE INFORMATIONS *****//
		if (ImGui::CollapsingHeader("Animation informations"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::Columns(2, "infoCols");
			//Name
			std::string animName = animManager->GetName();
			// Tell me it's ugly, and I'll let you find something better. Write me a message before.
			char nameInChar[32] = "                         ";
			for (int i = 0; i < animName.length(); i++)
				nameInChar[i] = animName[i];
			
			ImGui::Text("Animation name"); ImGui::NextColumn();
			ImGui::InputText("##name", nameInChar, 32);
			animManager->SetName(nameInChar);
			ImGui::NextColumn();

			//Looped
			bool looped = animManager->GetIsLooped();
			ImGui::Text("Looped"); ImGui::NextColumn();
			ImGui::Checkbox("##looped", &looped);
			animManager->SetIsLooped(looped);
			ImGui::NextColumn();

			//Speed
			float speed = animManager->GetSpeed() * 1000;

			ImGui::Text("Speed"); ImGui::NextColumn();
			ImGui::DragFloat("##speed", &speed); ImGui::SameLine();
			ImGui::Text("ms");
			animManager->SetSpeed(speed / 1000);
			ImGui::NextColumn();

			//NumFrames
			int animSize = animManager->GetAnim().size();
			ImGui::Text("Number of frames"); ImGui::NextColumn();
			ImGui::Text(std::to_string(animSize).c_str());
			ImGui::Columns(1, "infoCols");
		}

		short currentTextId = animManager->GetTextureIdFromKeyframe(m_currentFrame);
		TextureInfos* currFrame = m_AnimCreator->GetTextureManager()->GetTextureFromId(currentTextId);
		if (currFrame == nullptr)
			return;
		

		//**** FRAME INFORMATIONS *****//
		if (ImGui::CollapsingHeader("Frame informations"), ImGuiTreeNodeFlags_DefaultOpen) {
			ImGui::Columns(2, "infoCols");
			// Frame informations
			// Id
			ImGui::Text("Id"); ImGui::NextColumn();
			ImGui::Text(std::to_string(currFrame->id).c_str()); ImGui::NextColumn();

			// Filename
			ImGui::Text("File name"); ImGui::NextColumn();
			ImGui::Text(currFrame->fileName.c_str()); ImGui::NextColumn();

			// Path
			ImGui::Text("Texture path"); ImGui::NextColumn();
			ImGui::Text(currFrame->path.c_str()); ImGui::NextColumn();

			// Position
			ImGui::Text("Position"); ImGui::NextColumn();
			ImGui::Text((std::to_string(currFrame->position.x) + ", " + std::to_string(currFrame->position.y)).c_str()); ImGui::NextColumn();

			// Size
			ImGui::Text("Size"); ImGui::NextColumn();
			ImGui::Text((std::to_string(currFrame->size.x) + ", " + std::to_string(currFrame->size.y)).c_str()); ImGui::NextColumn();
		}
	}

	void GraphicsManager::OpenModalSave()
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_Modal;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::SetNextWindowPos(ImVec2(100.0f, 250.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 100.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Save Current...", &m_openModalSave, window_flags))
		{
			if (m_saveResult == SAVE_FAILURE)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.8f), "Couldn't save animation");
				if (ImGui::Button("Oh ok"))
				{
					m_saveResult = SAVE_SUCCESS;
					m_openModalSave = false;
				}
			}
			if (m_saveResult == SAVE_DO_REPLACE)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.8f), "An animation already exists with this name... Do you want to replace it?");
				ImGui::Columns(4, "yes_or_no", false);
				ImGui::NextColumn();
				if (ImGui::Button("Yes"))
				{
					m_saveResult =  m_AnimCreator->GetAnimationManager()->ExportToJson(m_AnimCreator->GetTextureManager()->GetAllTextures(), true);
					m_saveResult = SAVE_SUCCESS;
					m_openModalSave = false;
				}
				ImGui::NextColumn();
				if (ImGui::Button("No"))
				{
					m_saveResult = SAVE_SUCCESS;
					m_openModalSave = false;
				}
			}
		}
		ImGui::End();
	}

	void GraphicsManager::OpenModalConfirmNew()
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_Modal;
		window_flags |= ImGuiWindowFlags_NoCollapse;

		ImGui::SetNextWindowPos(ImVec2(100.0f, 250.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 50.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("New animation...", &m_openModalConfirmNew, window_flags))
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.8f), "Are you sure you want to begin a new animation?");
			ImGui::Columns(4, "yes_or_no", false);
			ImGui::NextColumn();
			if (ImGui::Button("Yes"))
			{
				m_AnimCreator->GetAnimationManager()->Init();
				m_openModalConfirmNew = false;
			}
			ImGui::NextColumn();
			if (ImGui::Button("No"))
			{
				m_openModalConfirmNew = false;
			}
		}
		ImGui::End();
	}	
}
