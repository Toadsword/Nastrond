#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <vector>
#include "SFML/Graphics.hpp"

struct TextureInfos
{
	short id;
	sf::Texture texture;
	std::string path;
	sf::Vector2i position;
	sf::Vector2i size;
};

class TextureManager
{
public:
	TextureManager::TextureManager(){};

	bool LoadTexture(std::string path);
	bool LoadTexture(std::string path, int numRow, int numCol, int rowWidth, int colWidth);
	std::vector<TextureInfos> GetAllTextures();
	TextureInfos GetTextureFromId(short id);

private:
	std::vector<TextureInfos> m_textures;
};

#endif // ifndef TEXTURE_MANAGER_H