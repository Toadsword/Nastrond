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

#include "SFML/Graphics.hpp"

#include <vector>

struct TextureInfos
{
	short id;
	sf::Texture texture;
	std::string path;
	std::string fileName;
	sf::Vector2i position;
	sf::Vector2i size;
};

class TextureManager
{
public:
	TextureManager::TextureManager(){};

	void Init();

	bool LoadTexture(std::string path, int sizeX, int sizeY);
	bool LoadTexture(std::string path, int rowWidth, int colWidth, int numRow, int numCol);
	std::vector<TextureInfos*>* GetAllTextures();
	TextureInfos* GetTextureFromId(short id);

	void DisplayTexture(short id, bool selected = false);

private:
	std::vector<TextureInfos*> m_textures;
	short m_lastId = 0;
};

#endif // ifndef TEXTURE_MANAGER_H