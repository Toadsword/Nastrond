#include <GraphicsManager.h>
#include <SFML/Graphics.hpp>
#include <Engine.h>

void GraphicsManager::Init(Engine & engine)
{
	m_engine = engine;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);
}

void GraphicsManager::Update()
{
	DisplayEditorWindow();
	DisplayFileWindow();
	DisplayMenuWindow();
	DisplayPreviewWindow();
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


sf::RenderWindow& GraphicsManager::GetWindow()
{
	return m_window;
}

void GraphicsManager::SetEngine(Engine & engine)
{
	m_engine = engine;
}
