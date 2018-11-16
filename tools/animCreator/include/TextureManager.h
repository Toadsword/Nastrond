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

private:
	std::vector<TextureInfos*> m_textures;
	short m_lastId = 0;
};

#endif // ifndef TEXTURE_MANAGER_H