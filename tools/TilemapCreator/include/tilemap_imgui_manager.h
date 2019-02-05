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


#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <SFML/Graphics.hpp>

#include <tilemap_creator.h>
#include <engine/tilemap.h>

namespace sfge::tools
{
/**
 * Constants
 */
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;
const int MAX_FRAMERATE = 60;
const char WINDOW_NAME[] = "TilemapCreator";

const float TIME_TO_DOUBLE_CLICK = 0.5;
const float TIME_BEFORE_REFRESH_LOADED_TEXTURE_STATUS = 5;

/**
 * Prototype
 */
class ToolEngine;
class TilemapCreator;

/**
 * \brief GraphicsManager : Deals with all the graphics of the application.
 * Display the window, sort all the elements using ImGui and deals with the User Interface.
 */
class TilemapImguiManager {

public:

	/**
	 * \brief Constructor of the Graphics Manager.
	 */
	TilemapImguiManager() {}

	/**
	 * \brief Initializer of the Graphics Manager. This function must be called before running, at it creates all the graphics needs.
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

	/**
	 * \brief Display the menu of the application.
	 */
	void DisplayMenuWindow();
	/**
	 * \brief Display the file section of the application.
	 */
	void DisplayFileWindow();
	/**
	 * \brief Display the animation preview section of the application.
	 * \param dt Time passed between the last frame and the current one.
	 */
	void DisplayPreviewWindow();
	/**
	 * \brief Display the information section of the animation and of the current Frame of the application.
	 */
	void DisplayAnimationInformationsWindow();

	/**
	 * \brief Display the modal used to save the animation.
	 */
	void OpenModalSave();
	/**
	 * \brief Display the modal used to confirm the reset of the animation to the application.
	 */
	void OpenModalConfirmNew();

	/**
	 * \brief Getter of the graphic window of the application.
	 * \return The graphic windows used of the application.
	 */
	sf::RenderWindow* GetWindow();

private:
	/**
	 * \brief Initialized state of the graphics.
	 */
	bool m_IsInit = false;

	/* ------------------------------------ Input control ---------------------------------------- */
	/**
	 * \brief Stored time since the user last clicked.
	 */
	float m_TimeSinceLastClick = 0;
	/**
	 * \brief Stored input if the user double clicked last frame.
	 */
	bool m_DoubleClicked = false;

	/* -------------------------------------- Modal Save  ----------------------------------- */
	/**
	 * \brief true if we display the modal to confirm the save the animation, false otherwise.
	 */
	bool m_OpenModalSave = false;
	/**
	 * \brief Result of the saved animation. Used to allow the user to confirm the possible replacement of the animation.
	 */
	//LogSaveError m_SaveResult = SAVE_SUCCESS;

	/* -------------------------------------- Modal new animation  ----------------------------------- */
	/**
	 * \brief true if we display the modal to confirm the reset of the animation, false otherwise.
	 */
	bool m_OpenModalConfirmNew = false;

	/* -------------------------------------- Others ----------------------------------- */
	/**
	 * \brief Reference to the Engine.
	 */
	TilemapCreator* m_TilemapCreator = nullptr;
};
}
#endif // ifndef GRAPHICS_MANAGER_H