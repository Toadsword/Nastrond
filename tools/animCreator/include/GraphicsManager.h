#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <SFML\Graphics.hpp>

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;
const std::string WINDOW_NAME = "AnimCreator";

class Engine;
class GraphicsManager{

private:
	Engine& m_engine;
	sf::RenderWindow m_window;
public:
	void Init(Engine & engine);
	void Update();

	void DisplayEditorWindow();
	void DisplayFileWindow();
	void DisplayMenuWindow();
	void DisplayPreviewWindow();

	sf::RenderWindow& GetWindow();

	void SetEngine(Engine& engine);
};

#endif // ifndef GRAPHICS_MANAGER_H