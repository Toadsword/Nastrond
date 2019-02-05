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
#include <tilemap_imgui_manager.h>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <engine/engine.h>
#include <input/input.h>

namespace sfge::tools
{
	void TilemapImguiManager::Init(TilemapCreator* engine)
	{
		m_TilemapCreator = engine;
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

		//ImGui::ShowDemoWindow();
	}

	void TilemapImguiManager::Draw()
	{
		if (!m_IsInit)
			return;

		ImGui::SetNextWindowPos(ImVec2(150.0f, 100.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 600.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(WINDOW_NAME, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
		{
			DisplayMenuWindow();
			//DisplayFileWindow();
		}
		ImGui::End();

		if (m_OpenModalSave) OpenModalSave();
		if (m_OpenModalConfirmNew) OpenModalConfirmNew();
	}

	void TilemapImguiManager::DisplayMenuWindow()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					m_OpenModalConfirmNew = true;
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


	/*
	void TilemapImguiManager::OpenModalSave()
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_Modal;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::SetNextWindowPos(ImVec2(100.0f, 250.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 100.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Save Current...", &m_OpenModalSave, window_flags))
		{
			if (m_SaveResult == SAVE_FAILURE)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.8f), "Couldn't save animation");
				if (ImGui::Button("Oh ok"))
				{
					m_SaveResult = SAVE_SUCCESS;
					m_OpenModalSave = false;
				}
			}
			if (m_SaveResult == SAVE_DO_REPLACE)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.8f), "An animation already exists with this name... Do you want to replace it?");
				ImGui::Columns(4, "yes_or_no", false);
				ImGui::NextColumn();
				if (ImGui::Button("Yes"))
				{
					m_SaveResult =  m_AnimCreator->GetAnimationManager()->ExportToJson(m_AnimCreator->GetTextureManager()->GetAllTextures(), true);
					m_SaveResult = SAVE_SUCCESS;
					m_OpenModalSave = false;
				}
				ImGui::NextColumn();
				if (ImGui::Button("No"))
				{
					m_SaveResult = SAVE_SUCCESS;
					m_OpenModalSave = false;
				}
			}
		}
		ImGui::End();
	}
	*/

	void TilemapImguiManager::OpenModalConfirmNew()
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_Modal;
		window_flags |= ImGuiWindowFlags_NoCollapse;

		ImGui::SetNextWindowPos(ImVec2(100.0f, 250.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(600.0f, 50.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("New animation...", &m_OpenModalConfirmNew, window_flags))
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.8f), "Are you sure you want to begin a new animation?");
			ImGui::Columns(4, "yes_or_no", false);
			ImGui::NextColumn();
			if (ImGui::Button("Yes"))
			{
				m_TilemapCreator->GetEngine().GetTilemapSystem()->Init();
				m_OpenModalConfirmNew = false;
			}
			ImGui::NextColumn();
			if (ImGui::Button("No"))
			{
				m_OpenModalConfirmNew = false;
			}
		}
		ImGui::End();
	}	
}
