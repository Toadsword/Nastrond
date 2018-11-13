#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <SFML\Graphics.hpp>

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;
const int MAX_FRAMERATE = 60;
const std::string WINDOW_NAME = "AnimCreator";

class Engine;
class GraphicsManager{
public:

	GraphicsManager::GraphicsManager() {}

	sf::RenderWindow* Init(Engine* engine);
	void Update(int dt);
	void Stop();

	void DisplayMenuWindow();
	void DisplayFileWindow();
	void DisplayPreviewWindow();
	void DisplayEditorWindow();

	sf::RenderWindow* GetWindow();
	void SetEngine(Engine* engine);

private:
	bool isInit = false;
	Engine* m_engine = nullptr;
	sf::RenderWindow* m_window = nullptr;
};

#endif // ifndef GRAPHICS_MANAGER_H