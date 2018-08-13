/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

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
*/

#ifndef SFGE_TEXTURE_H_
#define SFGE_TEXTURE_H_

//STL
#include <map>
#include <string>
#include <memory>


//Externals
#include <SFML/Graphics.hpp>


#include <engine/system.h>

namespace sfge
{
class GraphicsManager;

using TextureId = unsigned;
const TextureId INVALID_TEXTURE = 0U;
/**
* \brief The Texture Manager is the cache of all the textures used for sprites or other objects
*
*/
class TextureManager : public System
{
public:
	TextureManager() = default;
	/**
	 * \brief Load all the textures in the data in Shipping mode
	 */
	void Init() override;

	/**
	* \brief load the texture from the disk or the texture cache
	* \param filename The filename string of the texture
	* \return The strictly positive texture id > 0, if equals 0 then the texture was not loaded
	*/
	unsigned int LoadTexture(std::string filename);
	/**
	* \brief Used after loading the texture in the texture cache to get the pointer to the texture
	* \param text_id The texture id striclty positive
	* \return The pointer to the texture in memory
	*/
	sf::Texture* GetTexture(unsigned int text_id);
	
	void Clear() override;

	void Collect() override;


private:
	void LoadTextures(std::string dataDirname);

	std::map<std::string, TextureId> m_NameIdsMap;
	std::map<TextureId, unsigned int> m_IdsRefCountMap;
	std::map<TextureId, sf::Texture*> m_TexturesMap;
	TextureId m_IncrementId = 0U;

};
}


#endif /* INCLUDE_GRAPHICS_TEXTURE_H_ */
