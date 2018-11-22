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


#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>

#include <AnimationManager.h>
#include <TextureManager.h>
#include <GraphicsManager.h>

/**
 * \brief Engine : Core of the application, manages all the managers.
 * Creates, Destroys managers and deals with the global function of the application.
 */
class Engine
{
public:
	/**
	 * \brief Constructor of the Engine.
	 */
	Engine() {};

	/**
	 * \brief Initializer of the Engine. This function must be called before running, at it creates every Managers the tool need to function.
	 */
	void Init();
	/**
	 * \brief Start the Engine. Once started, it will repeatedly call Update().
	 */
	void Start();
	/**
	 * \brief Updates the Engine state every frames.
	 * \param dt Time passed between the last frame and the current one.
	 */
	void Update(int dt);
	/**
	 * \brief Called when the application is meant to be closed. Waits until the end of the current frame before closing.
	 */
	void ExitApplication();

	/**
	 * \brief Getter of the Graphics Manager.
	 * \return The Graphics Manager.
	 */
	GraphicsManager* GetGraphicsManager();
	/**
	 * \brief Getter of the Texture Manager.
	 * \return The Texture Manager.
	 */
	TextureManager* GetTextureManager();
	/**
	 * \brief Getter of the Animation Manager.
	 * \return The Animation Manager.
	 */
	AnimationManager* GetAnimationManager();

private:
	/**
	 * \brief Stops the Engine, destroys all the instanced objects and managers.
	 */
	void StopEngine();
	/**
	 * \brief Pointer to the Graphics Manager of the Engine.
	 */
	GraphicsManager* m_graphicsManager;
	/**
	 * \brief Pointer to the Texture Manager of the Engine.
	 */
	TextureManager* m_textureManager;
	/**
	 * \brief Pointer to the Animation Manager of the Engine.
	 */
	AnimationManager* m_animationManager;
	/**
	 * \brief Pointer to the graphics windows of the Engine.
	 */
	sf::RenderWindow* m_window = nullptr;

	/**
	 * \brief Current running state. Is set to "true" once the engine is initialized.
	 */
	bool isRunning = false;
};

#endif // ifndef ENGINE_H