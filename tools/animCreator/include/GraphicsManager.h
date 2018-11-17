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
	void DisplayGeneInformationsWindow();
	void DisplayFrameInformationsWindow();

	void OpenModalSave();
	void OpenModalAddText();

	sf::RenderWindow* GetWindow();

private:
	bool m_isInit = false;
	
	// TextureSelected
	short m_selectedTextureId = -1;

	// Preview options
	int m_currentFrame = 0;
	bool m_doPlayAnimation = false;

	// Modal Add Texture 
	bool m_openModalAddText = false;
	char m_inputNameNewFile[128] = "";
	int m_inputNumCols = 0;
	int m_inputNumRows = 0;
	int m_inputSizeX = 0;
	int m_inputSizeY = 0;

	// Modal Save 
	bool m_openModalSave = false; 
	LogSaveError m_saveResult = SUCCESS;

	Engine* m_engine = nullptr;
	sf::RenderWindow* m_window = nullptr;
};

#endif // ifndef GRAPHICS_MANAGER_H