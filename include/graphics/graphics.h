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
#include <engine/engine.h>

namespace sfge
{
class TextureManager;

/**
* \brief The Graphics Manager
*/
class GraphicsManager : public Module
{
public:
	GraphicsManager(bool enable, bool windowless);
	/**
		* \brief Initialize the Graphics Manager
		*/
	void Init() override;

	/**
		* \brief Update the Graphics Manager and prepare for the rendering
		* \param dt Delta time since last frame
		*/
	void Update(sf::Time dt) override;

	void Display();
	/**
	* \brief Destroy the window and other
	*/
	void Destroy() override;

	/**
	* \brief Getter of the window created in GraphicsManager
	* \return The SFML window
	*/
	std::shared_ptr<sf::RenderWindow> GetWindow();

	std::shared_ptr<SpriteManager> GetSpriteManager();
	std::shared_ptr<TextureManager> GetTextureManager();

protected:
	bool m_Windowless = false;
	/**
	* \brief Write to log the OpenGL version
	*/
	void CheckVersion();
	std::shared_ptr<TextureManager> m_TextureManager = nullptr;
	std::shared_ptr<SpriteManager> m_SpriteManager = nullptr;
	std::shared_ptr<sf::RenderWindow> m_Window = nullptr;
};

}
#endif