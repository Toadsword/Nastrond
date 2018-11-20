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

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>

#include <vector>

/**
 * \brief Enum used to reference the state of the last loaded texture in the application.
 */
enum LoadFileResult
{
	LOAD_NONE,
	LOAD_SUCCESS,
	LOAD_FAILURE,
};

/**
 * \brief Struct storing all the informations of a loaded texture.
 */
struct TextureInfos
{
	short id;
	sf::Texture texture;
	std::string path;
	std::string fileName;
	sf::Vector2i position;
	sf::Vector2i size;
};

/**
 * \brief Texture Manager : Store and references all the loaded textures in the application.
 * A texture can be applied to a keyframe in the animation.
 */
class TextureManager
{
public:
	/**
	 * \brief Constructor of the Texture Manager.
	 */
	TextureManager::TextureManager(){};
	
	/**
	 * \brief Initializer of the Texture Manager. This function must be called before running, at it initializes the needed variables.
	 */
	void Init();

	/**
	 * \brief Load new textures to the application. Can be a single image or a spriteSheet.
	 * \param path Path of the file to load.
	 * \param sizeX Size X of a sprite.
	 * \param sizeY Size Y of a sprite.
	 * \param numRow Number of rows of the spriteSheet.
	 * \param numCol Number of columns of the spriteSheet.
	 * \param offsetX Offset X of the spriteSheet.
	 * \param offsetY Offset Y of the spriteSheet.
	 * \return 
	 */
	LoadFileResult LoadTexture(std::string path, int sizeX = 0, int sizeY = 0, int numRow = 0, int numCol = 0, int offsetX = 0, int offsetY = 0);
	/**
	 * \brief Return of all the textures loaded.
	 * \return 
	 */
	std::vector<TextureInfos*>* GetAllTextures();
	/**
	 * \brief Remove the texture specified by the given Id.
	 * \param id Id of the texture to remove.
	 * \return True if the texture is removed, false otherwise.
	 */
	bool RemoveTexture(short id);
	/**
	 * \brief Getter of the TextureInfo from the giver id.
	 * \param id Id of the texture to get.
	 * \return The reference to the TextureInfos if found, nullptr otherwise.
	 */
	TextureInfos* GetTextureFromId(short id);

	/**
	 * \brief Getter of the LastId attributed to a texture.
	 * \return LastId.
	 */
	short GetLastId();

	/**
	 * \brief Display with ImGui the texture.
	 * \param id Id of the texture to display.
	 * \param selected Add a border around the image if true, nothing otherwise.
	 */
	void DisplayTexture(short id, bool selected = false);

private:
	/**
	 * \brief List of loaded textures.
	 */
	std::vector<TextureInfos*> m_textures;
	/**
	 * \brief Last Id attributed to a loaded texture.
	 */
	short m_lastId = 0;
};

#endif // ifndef TEXTURE_MANAGER_H