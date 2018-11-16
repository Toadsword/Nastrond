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

#include <windows.h>
#include <iostream>

#include <Engine.h>
#include <GraphicsManager.h>
#include <imgui_internal.h>
#include "Utilities.h"

sf::RenderWindow* GraphicsManager::Init(Engine* engine)
{
	m_engine = engine;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);
	window->setFramerateLimit(MAX_FRAMERATE);
	
	m_window = window;
	ImGui::SFML::Init(*m_window, true);

	m_isInit = true;

	return window;
}

void GraphicsManager::Update(int dt)
{
	if (!m_isInit)
		return;

	sf::Event event;
	while (m_window->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		switch (event.type)
		{
		case sf::Event::Closed:
			m_engine->ExitApplication();
			return;
		}
	}

	ImGui::SFML::Update(*m_window, sf::seconds(dt));

	//ImGui::ShowDemoWindow();

	
	DisplayEditorWindow();
	DisplayFileWindow();
	DisplayMenuWindow();
	DisplayPreviewWindow();
	

	m_window->clear();
	ImGui::SFML::Render(*m_window);
	//window.draw(playerSprite);
	m_window->display();
}

void GraphicsManager::Stop()
{
	delete(m_window);
	ImGui::Shutdown();
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
					std::cout << "New select \n";
				}
				if (ImGui::MenuItem("Open new file", "Ctrl+O")) 
				{
					std::cout << "Open select \n";
				}
				if (ImGui::MenuItem("Save current..", "Ctrl+S"))
				{
					m_saveResult = Utilities::ExportToJson(m_engine->GetAnimationManager(), m_engine->GetTextureManager()->GetAllTextures());
					m_openModalSave = m_saveResult != SUCCESS;
				}
				if (ImGui::MenuItem("Quit", "Alt+F4"))
				{
					m_engine->ExitApplication();			
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
	ImGui::End();

	if (m_openModalSave) OpenModalSave();
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
	ImGui::SetNextWindowSize(ImVec2(450.0f, 280.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("FileWindow", NULL, window_flags))
	{
		if(ImGui::Button(" + "))
		{
			std::cout << "Ajouter une texture.\n";
			m_openModalAddText = true;
		}

		ImGui::Columns(2);
		auto* loadedTextures = m_engine->GetTextureManager()->GetAllTextures();

		for(auto* texture : *loadedTextures)
		{
			if (ImGui::Button((std::to_string(texture->id) + " : " + texture->fileName).c_str()))
			{
				std::cout << "Texture " << texture->id << " selected.\n";
				m_selectedTexture = texture->id;
			}
			sf::FloatRect imgRect(texture->position.x, texture->position.y, texture->size.x, texture->size.y);
			if(m_selectedTexture == texture-> id)
				ImGui::Image(texture->texture, imgRect, sf::Color::White, sf::Color::Blue);
			else
				ImGui::Image(texture->texture, imgRect);
			ImGui::NextColumn();
		}
	}
	ImGui::End();

	if (m_openModalAddText) OpenModalAddText();
}

void GraphicsManager::DisplayPreviewWindow()
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
		if (ImGui::Selectable("Coucou Je suis un seletable"))
			std::cout << "Je suis select \n";
	}
	ImGui::End();
}

void GraphicsManager::DisplayEditorWindow()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 300.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(900, 300.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("EditorWindow", NULL, window_flags))
	{
		if (ImGui::Selectable("Coucou Je suis un seletable"))
			std::cout << "Je suis select \n";
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
		if (m_saveResult == FAILURE)
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 0.8), "Couldn't save animation");
			if (ImGui::Button("Oh ok"))
			{
				m_saveResult = SUCCESS;
				m_openModalSave = false;
			}
		}
		if(m_saveResult == DO_REPLACE)
		{
			ImGui::TextColored(ImVec4(1, 0, 0, 0.8), "An animation already exists with this name... Do you want to replace it?");
			ImGui::Columns(4, "yes_or_no", false);
			ImGui::NextColumn();
			if(ImGui::Button("Yes"))
			{
				m_saveResult = Utilities::ExportToJson(m_engine->GetAnimationManager(), m_engine->GetTextureManager()->GetAllTextures(), true);
				m_saveResult = SUCCESS;
				m_openModalSave = false;
			}
			ImGui::NextColumn();
			if (ImGui::Button("No"))
			{
				m_saveResult = SUCCESS;
				m_openModalSave = false;
			}
		}
	}
	ImGui::End();
}

void GraphicsManager::OpenModalAddText()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_Modal;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowPos(ImVec2(100.0f, 250.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600.0f, 130.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Load new sprite...", &m_openModalAddText, window_flags))
	{
		//TODO : FILE SYSTEM
		ImGui::InputText("FilePath (from C:)", m_inputNameNewFile, IM_ARRAYSIZE(m_inputNameNewFile));
		ImGui::Columns(2);
		ImGui::InputInt("Size X", &m_inputSizeX);
		ImGui::NextColumn();
		ImGui::InputInt("Size Y", &m_inputSizeY);
		ImGui::NextColumn();
		ImGui::InputInt("Num Rows", &m_inputNumRows);
		ImGui::NextColumn();
		ImGui::InputInt("Num Columns", &m_inputNumCols);

		if(ImGui::Button("Load"))
		{
			bool result = m_engine->GetTextureManager()->LoadTexture(m_inputNameNewFile, m_inputSizeX, m_inputSizeY, m_inputNumRows, m_inputNumCols);
			std::cout << "rows : " << m_inputNumRows << " ;\n" <<
				"cols : " << m_inputNumCols << " ;\n" <<
				"SizeX : " << m_inputSizeX << " ;\n" <<
				"SizeY : " << m_inputSizeY << " ;\n";
			if (result)
				std::cout << "Loaded Textures !\n";
			else
				std::cout << "Failed to load textures..\n";				
		}
	}
	ImGui::End();
}

sf::RenderWindow* GraphicsManager::GetWindow()
{
	return m_window;
}
