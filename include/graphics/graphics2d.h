/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

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
*/

#ifndef SFGE_GRAPHICS_H
#define SFGE_GRAPHICS_H

#include <engine/system.h>
#include <graphics/shape2d.h>
#include <graphics/texture.h>
#include <graphics/sprite2d.h>
#include <graphics/animation2d.h>
#include <graphics/camera.h>
#include <graphics/tilemap.h>

namespace sfge
{
	const int WINDOW_TOP_HEIGTH_PIXEL = 38;
	const int WINDOW_SIDES_WIDTH_PIXEL = 8;

	const int WINDOW_DEFAULT_HEIGTH = 720;
	const int WINDOW_DEFAULT_WITDH = 1080;
/**
* \brief The Graphics Manager
*/
class Graphics2dManager : public System
{
public:
	using System::System;

	/**
		* \brief Initialize the Graphics Manager
		*/
	void Init() override;

	/**
		* \brief Update the Graphics Manager and prepare for the rendering
		* \param dt Delta time since last frame
		*/
	void Update(float dt) override;

	void Display();
	/**
	* \brief Destroy the window and other
	*/
	void Destroy() override;

	void Draw() override;

	void Clear() override;
	
	void Collect() override;


	void DrawLine(sf::Vector2f from, sf::Vector2f to, sf::Color color=sf::Color::Red);

	/**
	* \brief Getter of the window created in GraphicsManager
	* \return The SFML window
	*/
	sf::RenderWindow* GetWindow();
	sf::Vector2f GetSizeWindow();
	sf::Vector2f GetPositionWindow();

	void OnChangeScreenMode();

	AnimationManager* GetAnimationManager();
	ShapeManager* GetShapeManager();
	SpriteManager* GetSpriteManager();
	TextureManager* GetTextureManager();
	CameraManager* GetCameraManager();
	TilemapSystem* GetTilemapSystem();

protected:
	bool m_Windowless = false;
	/**
	* \brief Write to log the OpenGL version
	*/
	void CheckVersion() const;
	TextureManager m_TextureManager{m_Engine};
	SpriteManager m_SpriteManager{m_Engine};
	AnimationManager m_AnimationManager{ m_Engine };
	ShapeManager m_ShapeManager{m_Engine};
	CameraManager m_CameraManager{ m_Engine };
	TilemapSystem m_TilemapSystem{ m_Engine };
	std::unique_ptr<sf::RenderWindow> m_Window;
};

}
#endif
