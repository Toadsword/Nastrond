#include <TextureManager.h>

#include <SFML/Graphics.hpp>
#include <iostream>

void TextureManager::Init()
{
	m_lastId = 0;
}

bool TextureManager::LoadTexture(std::string path, int sizeX, int sizeY)
{
	return LoadTexture(path, sizeX, sizeY, 0, 0);
}

bool TextureManager::LoadTexture(std::string path, int rowWidth, int colWidth, int numRow, int numCol)
{
	sf::Texture newTexture;
	if (newTexture.loadFromFile(path))
	{
		for (int i = 0; i <= numRow; i++)
		{
			for (int j = 0; j <= numCol; j++)
			{
				TextureInfos* newTextureInfo = new TextureInfos();
				newTextureInfo->texture = newTexture;
				newTextureInfo->path = path;
				newTextureInfo->position = sf::Vector2i(i*rowWidth, j*colWidth);
				newTextureInfo->size = sf::Vector2i(rowWidth, colWidth);;
				newTextureInfo->id = m_lastId;
				m_lastId++;
				m_textures.push_back(newTextureInfo);
			}
		}		
		return true;
	}
	return false;
}

std::vector<TextureInfos*> TextureManager::GetAllTextures()
{
	return m_textures;
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

