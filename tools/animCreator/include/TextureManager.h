#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <vector>
#include "SFML/Graphics/Texture.hpp"

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
private:
	std::vector<TextureInfos> textures;
public:
	bool LoadTexture(std::string path);
	bool LoadTexture(std::string path, int numRow, int numCol, int rowWidth, int colWidth);
	std::vector<TextureInfos> GetAllTextures();
	TextureInfos GetTextureFromId(short Id);
};

#endif // ifndef TEXTURE_MANAGER_H