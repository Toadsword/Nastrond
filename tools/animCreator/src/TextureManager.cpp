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

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <iostream>

#include <TextureManager.h>

void TextureManager::Init()
{
	m_lastId = 0;

	LoadTexture("D:/Images/Sprites/Pepper_publish.png", 24, 32, 4, 3);
}

bool TextureManager::LoadTexture(std::string path, int sizeX, int sizeY)
{
	return LoadTexture(path, sizeX, sizeY, 0, 0);
}

bool TextureManager::LoadTexture(std::string path, int sizeX, int sizeY, int numRow, int numCol)
{
	sf::Texture newTexture;
	if (newTexture.loadFromFile(path))
	{
		for (int i = 0; i < numCol; i++)
		{
			for (int j = 0; j < numRow; j++)
			{
				TextureInfos* newTextureInfo = new TextureInfos();
				newTextureInfo->texture = newTexture;
				newTextureInfo->path = path;

				//Get the file name.
				std::string fileName = path;
				std::size_t found = fileName.find("/");
				while (found < fileName.length())
				{
					fileName = fileName.substr(found + 1);
					found = fileName.find("/");
				}
				newTextureInfo->fileName = fileName;

				newTextureInfo->position = sf::Vector2i(i*sizeX, j*sizeY);
				newTextureInfo->size = sf::Vector2i(sizeX, sizeY);;
				newTextureInfo->id = m_lastId;
				m_lastId++;
				m_textures.push_back(newTextureInfo);
			}
		}		
		return true;
	}
	return false;
}

std::vector<TextureInfos*>* TextureManager::GetAllTextures()
{
	return &m_textures;
}

TextureInfos* TextureManager::GetTextureFromId(short id)
{
	for (auto element : m_textures)
	{
		if (element->id == id)
			return element;
	}

	return nullptr;
}

void TextureManager::DisplayTexture(short id, bool selected)
{
	auto texture = GetTextureFromId(id);
	if(texture != nullptr)
	{
		sf::FloatRect imgRect(texture->position.x, texture->position.y, texture->size.x, texture->size.y);
		if (selected)
			ImGui::Image(texture->texture, imgRect, sf::Color::White, sf::Color::Blue);
		else
			ImGui::Image(texture->texture, imgRect);
	}
}

