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

#ifndef SFGE_SPRITE_H
#define SFGE_SPRITE_H

//STL
#include <string>
#include <queue>
//Dependencies
#include <SFML/Graphics.hpp>
//Engine
#include <engine/component.h>
#include <engine/transform.h>

namespace sfge
{
class GraphicsManager;
/**
* \brief Sprite component used in the GameObject
*/
class Sprite:
	LayerComponent
{
public:
	void Init();
	
	void Draw(sf::RenderWindow& window);


	void SetTexture(sf::Texture* newTexture);

	void SetTextureId(unsigned int textureId);


	
protected:
	sf::Vector2f offset = sf::Vector2f();
	std::string filename;
	unsigned int m_TextureId = 0U;
	sf::Sprite sprite;
};

/**
* \brief Sprite manager caching all the sprites and rendering them at the end of the frame
*/
class SpriteManager : ComponentManager<Sprite>, LayerComponentManager<Sprite>
{
public:
	SpriteManager(GraphicsManager& graphicsManager);
	void Update();
	void Draw(sf::RenderWindow& window);

	void Reset();
	void Collect();

	bool CreateComponent() override;
	bool DestroyComponent() override;

protected:
	GraphicsManager& m_GraphicsManager;
	std::weak_ptr<Transform2dManager> m_TransformManager;
};




}
#endif // !SFGE_SPRITE
