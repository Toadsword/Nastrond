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

#include <iostream>

#include <tool_engine.h>
#include <graphics_manager.h>
#include <utilities.h>
#include "anim_creator.h"

namespace sfge::tools
{
void GraphicsManager::Init(AnimCreator* engine)
{
	m_AnimCreator = engine;
	m_isInit = true;
}

void GraphicsManager::Update(int dt)
{
	if (!m_isInit)
		return;

	/*
	sf::Event event;
	while (m_Window->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		switch (event.type)
		{
		case sf::Event::Closed:
			m_AnimCreator->ExitApplication();
			return;

			//Shortcuts
		case sf::Event::KeyPressed:
			if (event.key.control && event.key.code == sf::Keyboard::Key::S)
			{
				m_saveResult = ExportToJson(m_AnimCreator->GetAnimationManager(), m_AnimCreator->GetTextureManager()->GetAllTextures());
				m_openModalSave = m_saveResult != SAVE_SUCCESS;
			}
			if (event.key.control && event.key.code == sf::Keyboard::Key::O)
			{
				m_openModalAddTexture = true;
			}
			if (event.key.control && event.key.code == sf::Keyboard::Key::P)
				m_doPlayAnimation = !m_doPlayAnimation;
			if (event.key.control && event.key.code == sf::Keyboard::Key::L)
				m_AnimCreator->GetAnimationManager()->SetIsLooped(!m_AnimCreator->GetAnimationManager()->GetIsLooped());
			if (event.key.control && event.key.code == sf::Keyboard::Key::Add)
				m_AnimCreator->GetAnimationManager()->AddOrInsertKey();
			if (event.key.control && event.key.code == sf::Keyboard::Key::Subtract)
				m_AnimCreator->GetAnimationManager()->RemoveKey();
			break;
		case sf::Event::MouseButtonPressed:
			if (m_timeSinceLastClick < TIME_TO_DOUBLE_CLICK)
				m_doubleClicked = true;
			else
				m_doubleClicked = false;
			m_timeSinceLastClick = 0;
			break;
		default:
			break;
		}
	}
	*/

	//ImGui::ShowDemoWindow();

}

void GraphicsManager::Draw()
{
	if (!m_isInit)
		return;

	DisplayAnimationInformationsWindow();
	DisplayFileWindow();
	DisplayPreviewWindow(15.0f);
	DisplayMenuWindow();
}

void GraphicsManager::DisplayMenuWindow()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(900.0f, 0.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("MenuWindow", NULL, window_flags))
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
					m_openModalAddTexture = true;
				}
				if (ImGui::MenuItem("Save current..", "Ctrl+S"))
				{
					m_saveResult = ExportToJson(m_AnimCreator->GetAnimationManager(), m_AnimCreator->GetTextureManager()->GetAllTextures());
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
	ImGui::End();

	if (m_openModalSave) OpenModalSave();
	if (m_openModalConfirmNew) OpenModalConfirmNew();
}

void GraphicsManager::DisplayFileWindow()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(450.0f, 580.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("FileWindow", NULL, window_flags))
	{
		ImGui::Columns(2);
		if (ImGui::Button("Add a texture"))
			m_openModalAddTexture = true;

		ImGui::NextColumn();
		ImGui::Text("All Sprites");

		ImGui::NextColumn();
		auto* loadedTextures = m_AnimCreator->GetTextureManager()->GetAllTextures();
		if (loadedTextures->empty())
		{
			ImGui::Columns(1);
			ImGui::Text("There are currently no sprites in here...");
			ImGui::Text("Begin by adding some sprites ! (Button above)");
			ImGui::Columns(2);
		}
		for (auto* texture : *loadedTextures)
		{
			if (ImGui::Button((std::to_string(texture->id) + " : " + texture->fileName).c_str(), ImVec2(125, 0)))
			{
				//std::cout << "Texture " << texture->id << " selected.\n";
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
				animManager->AddOrInsertKey(animManager->GetHighestKeyNum() + 1, texture->id);
			}
			if (ImGui::IsItemHovered())
			{
				ImVec2 m = ImGui::GetIO().MousePos;

				ImGui::SetNextWindowPos(ImVec2(m.x + 10, m.y + 10));
				ImGui::Begin("2", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
				ImGui::Text("Add a frame with this texture.");
				ImGui::End();
			}
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

			m_AnimCreator->GetTextureManager()->DisplayTexture(texture->id, m_SelectedTextureId == texture->id);
			ImGui::NextColumn();
		}
	}
	ImGui::End();

	if (m_openModalAddTexture) OpenModalAddTexture();
}

void GraphicsManager::DisplayPreviewWindow(int dt)
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(450.0f, 20.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(450.0f, 280.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("PreviewWindow", NULL, window_flags))
	{
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
		ImGui::SliderInt("All Frames", &m_currentFrame, 0, std::max(0, m_AnimCreator->GetAnimationManager()->GetHighestKeyNum()));

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
	ImGui::End();
}

void GraphicsManager::DisplayAnimationInformationsWindow()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(450.0f, 300.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(450, 300.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("FrameInfoWindow", NULL, window_flags))
	{
		auto animManager = m_AnimCreator->GetAnimationManager();

		//***** BUTTONS *****//
		if (ImGui::Button("Assign selected texture") && m_SelectedTextureId != -1)
		{
			//std::cout << "Assigned texture " << m_selectedTextureId << " to frame " << m_currentFrame << ".\n";
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
		if (ImGui::CollapsingHeader("Animation informations"))
		{
			ImGui::Columns(2, "infoCols");
			//Name
			char* nameInChar = ConvertStringToArrayChar(animManager->GetName(), 32);
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
			int speed = animManager->GetSpeed();

			ImGui::Text("Speed"); ImGui::NextColumn();
			ImGui::DragInt("##speed", &speed); ImGui::SameLine();
			ImGui::Text("ms");
			animManager->SetSpeed(speed);
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
		{
			ImGui::End();
			return;
		}

		//**** FRAME INFORMATIONS *****//
		if (ImGui::CollapsingHeader("Frame informations")) {
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
	ImGui::End();
}

void GraphicsManager::OpenModalSave()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_Modal;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;

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
				m_saveResult = ExportToJson(m_AnimCreator->GetAnimationManager(), m_AnimCreator->GetTextureManager()->GetAllTextures(), true);
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

void GraphicsManager::OpenModalAddTexture()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_Modal;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowPos(ImVec2(100.0f, 250.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600.0f, 150.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Load new sprite...", &m_openModalAddTexture, window_flags))
	{
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

		ImGui::Text("Start position x"); ImGui::NextColumn();
		ImGui::InputInt("##Start position x", &m_inputOffsetX); ImGui::NextColumn();

		ImGui::Text("Start position y"); ImGui::NextColumn();
		ImGui::InputInt("##Start position y", &m_inputOffsetY); ImGui::NextColumn();

		ImGui::Columns(1, "idColAddSprite");

		auto textManager = m_AnimCreator->GetTextureManager();
		if (ImGui::Button("Load"))
		{
			m_lastIdBeforeNewTextLoad = m_AnimCreator->GetTextureManager()->GetLastId();
			m_fileImportResult = textManager->LoadTexture(m_inputNameNewFile, m_inputSizeX, m_inputSizeY, m_inputNumRows, m_inputNumCols, m_inputOffsetX, m_inputOffsetY);
		}
		ImGui::SameLine();
		if (m_fileImportResult != LOAD_NONE)
		{
			if (ImGui::Button("Undo"))
			{
				int currentId = m_AnimCreator->GetTextureManager()->GetLastId();
				for (int i = m_lastIdBeforeNewTextLoad; i <= currentId; i++)
					textManager->RemoveTexture(i);

				m_lastIdBeforeNewTextLoad = currentId;
			}
			ImGui::SameLine();
		}

		if (m_fileImportResult == LOAD_SUCCESS)
		{
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Textures loaded successfully");
		}
		else if (m_fileImportResult == LOAD_FAILURE)
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error while loading file : Does it really exists?");
		}
	}
	ImGui::End();
}

void GraphicsManager::OpenModalConfirmNew()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_Modal;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;

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
