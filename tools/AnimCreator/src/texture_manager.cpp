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

#include <texture_manager.h>
#include <json.hpp>

void TextureManager::Init()
{
	m_lastId = 0;
}

LoadFileResult TextureManager::LoadTexture(std::string path, int sizeX, int sizeY, int numRow, int numCol, int startPosX, int startPosY)
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

				std::string fileName = path;
				char slash = '/';
				if(!(fileName.find(slash) < fileName.length()))
					slash = '\\';

				//Get the file name.
				std::size_t found = fileName.find(slash);
				while (found < fileName.length())
				{
					fileName = fileName.substr(found + 1);
					found = fileName.find(slash);
				}
				newTextureInfo->fileName = fileName;

				newTextureInfo->position = sf::Vector2i(startPosX + i*sizeX, startPosY + j*sizeY);
				newTextureInfo->size = sf::Vector2i(sizeX, sizeY);;
				newTextureInfo->id = m_lastId;
				m_lastId++;
				m_textures.push_back(newTextureInfo);
			}
		}
		return LOAD_SUCCESS;
	}
	return LOAD_FAILURE;
}


std::vector<TextureInfos*>* TextureManager::GetAllTextures()
{
	return &m_textures;
}

bool TextureManager::RemoveTexture(short id)
{
	for (auto iter = m_textures.begin(); iter != m_textures.end(); iter++)
	{
		if ((*iter)->id == id)
		{
			m_textures.erase(iter);
			return true;
		}
	}
	return false;
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

short TextureManager::GetLastId()
{
	return m_lastId;
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

