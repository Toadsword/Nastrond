#include <GraphicsManager.h>
#include <SFML/Graphics.hpp>
#include <Engine.h>

#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>

sf::RenderWindow* GraphicsManager::Init(Engine* engine)
{
	m_engine = engine;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);
	window->setFramerateLimit(MAX_FRAMERATE);
	
	m_window = window;
	ImGui::SFML::Init(*m_window, true);

	isInit = true;

	return window;
}

void GraphicsManager::Update(int dt)
{
	if (!isInit)
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
}

void GraphicsManager::DisplayFileWindow()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(450.0f, 280.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("FileWindow", NULL, window_flags))
	{
		if (ImGui::Selectable("Coucou Je suis un seletable"))
			std::cout << "Je suis select \n";
	}
	ImGui::End();
}

void GraphicsManager::DisplayPreviewWindow()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

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

	ImGui::SetNextWindowPos(ImVec2(0.0f, 300.0f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(900, 300.0f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("EditorWindow", NULL, window_flags))
	{
		if (ImGui::Selectable("Coucou Je suis un seletable"))
			std::cout << "Je suis select \n";
	}
	ImGui::End();
}

sf::RenderWindow* GraphicsManager::GetWindow()
{
	return m_window;
}
