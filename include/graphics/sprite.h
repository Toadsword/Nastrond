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
#include <map>
#include <string>
//Engine
#include <engine/component.h>
#include <engine/engine.h>
//Dependencies
#include <SFML/Graphics.hpp>

namespace sfge
{
class GraphicsManager;
/**
* \brief Sprite component used in the GameObject
*/
class Sprite : public Component, Offsetable
{
public:
	using Component::Component;
	void Init() override;
	/**
	* \brief Update the Component
	* \param dt Delta time since last frame
	*/
	void Update(float dt) override;
	
	void Draw(sf::RenderWindow& window);


	static Sprite* LoadSprite(Engine& engine, json& componentJson, GameObject* gameObject);


	void SetTexture(sf::Texture* newTexture);

	void SetTextureId(unsigned int textureId);

	void SetLayer(int layer);

	static bool SpriteLayerComp(Sprite* s1, Sprite* s2);
protected:
	sf::Vector2f offset = sf::Vector2f();
	std::string filename;
	unsigned int m_TextureId{};
	int layer{};
	sf::Sprite sprite;
};

/**
* \brief Sprite manager caching all the sprites and rendering them at the end of the frame
*/
class SpriteManager 
{
public:
	SpriteManager(GraphicsManager& graphicsManager);
	void Update(sf::Time dt);
	void Draw(sf::RenderWindow& window);
	void LoadSprite(json& componentJson, Sprite* sprite);

	void Reset();
	void Collect();
protected:
	std::list<Sprite*> m_Sprites;
	GraphicsManager& m_GraphicsManager;
};




}
#endif // !SFGE_SPRITE
