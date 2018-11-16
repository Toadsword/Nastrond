#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <SFML\Graphics.hpp>

#include <Utilities.h>

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

	void OpenModalSave();
	void OpenModalAddText();

	sf::RenderWindow* GetWindow();

private:
	bool m_isInit = false;
	// TextureSelected
	short m_selectedTexture = -1;

	// Modal Add Texture 
	bool m_openModalAddText = false;
	char m_inputNameNewFile[128] = "";
	int m_inputNumCols = 0;
	int m_inputNumRows = 0;
	int m_inputSizeX = 0;
	int m_inputSizeY = 0;

	// Modal Save 
	bool m_openModalSave = false; 
	LogSaveError m_saveResult;

	Engine* m_engine = nullptr;
	sf::RenderWindow* m_window = nullptr;
};

#endif // ifndef GRAPHICS_MANAGER_H