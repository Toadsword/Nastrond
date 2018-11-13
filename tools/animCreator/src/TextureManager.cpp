#include <TextureManager.h>

bool TextureManager::LoadTexture(std::string path)
{
	return true;
}

bool TextureManager::LoadTexture(std::string path, int numRow, int numCol, int rowWidth, int colWidth)
{
	return true;
}

std::vector<TextureInfos> TextureManager::GetAllTextures()
{
	return m_textures;
}

TextureInfos TextureManager::GetTextureFromId(short id)
{
	return TextureInfos();
}

