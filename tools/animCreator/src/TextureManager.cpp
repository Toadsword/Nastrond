#include <SFML/Graphics.hpp>

#include <iostream>

#include <TextureManager.h>

void TextureManager::Init()
{
	m_lastId = 0;

	//LoadTexture("D:/Images/Sprites/items.png", 24, 24, 6, 6);
}

bool TextureManager::LoadTexture(std::string path, int sizeX, int sizeY)
{
	return LoadTexture(path, sizeX, sizeY, 0, 0);
}

bool TextureManager::LoadTexture(std::string path, int SizeX, int SizeY, int numRow, int numCol)
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

				newTextureInfo->position = sf::Vector2i(i*SizeX, j*SizeY);
				newTextureInfo->size = sf::Vector2i(SizeX, SizeY);;
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

