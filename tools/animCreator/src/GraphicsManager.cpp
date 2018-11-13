#include <GraphicsManager.h>
#include <SFML/Graphics.hpp>
#include <Engine.h>

#include <imgui.h>
#include <imgui-SFML.h>

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
			m_engine->Stop();
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

void GraphicsManager::DisplayEditorWindow()
{

}

void GraphicsManager::DisplayFileWindow()
{

}

void GraphicsManager::DisplayMenuWindow()
{

}

void GraphicsManager::DisplayPreviewWindow()
{

}


sf::RenderWindow* GraphicsManager::GetWindow()
{
	return m_window;
}

void GraphicsManager::SetEngine(Engine* engine)
{
	m_engine = engine;
}
