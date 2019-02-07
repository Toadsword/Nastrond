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
Date : February 2019
Project : TilemapCreator for SFGE
*/

#ifndef TILEMAP_IMGUI_MANAGER_H
#define TILEMAP_IMGUI_MANAGER_H
#include "engine/entity.h"

namespace sfge::tools
{
/**
 * Prototype
 */
class TilemapCreator;

/**
 * Constants
 */
const int TILEMAP_WINDOW_WIDTH = 400;
const int TILEMAP_WINDOW_HEIGHT = 600;
const int TILEMAP_MAX_FRAMERATE = 60;
const char TILEMAP_WINDOW_NAME[] = "TilemapCreator";

/**
 * \brief TilemapImguiManager : Deals with all the graphics of the tool.
 * Display the window, sort all the elements using ImGui and deals with the User Interface.
 */
class TilemapImguiManager {

public:
	/**
	 * \brief Constructor of the Tilemap Imgui Manager.
	 */
	TilemapImguiManager() = default;

	/**
	 * \brief Initializer of the Tilemap Imgui Manager. This function must be called before running, at it creates all the graphics needs.
	 * \return The pointer to the created windows.
	 */
	void Init(TilemapCreator* engine);
	/**
	 * \brief Updates the graphic state every frames.
	 * \param dt Time passed between the last frame and the current one.
	 */
	void Update(float dt);
	/**
	 * \brief Displays the graphics of the tool of the current frame.
	 */
	void Draw();

	void DisplayMenuWindow();
	void DrawMainWindow();
	void DrawTilemapInformations();

private:
	/**
	 * \brief Initialized state of the graphics.
	 */
	bool m_IsInit = false;

	unsigned m_SelectedTilemap = 0;

	/* -------------------------------------- Others ----------------------------------- */
	/**
	 * \brief Reference to the Engine.
	 */
	TilemapCreator* m_TilemapCreator = nullptr;
	/**
	 * \brief Reference to the Entity Manager.
	 */
	EntityManager* m_EntityManager = nullptr;
};
}
#endif // ifndef GRAPHICS_MANAGER_H