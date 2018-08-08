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
//Dependencies
#include <SFML/Graphics.hpp>
//Engine
#include <engine/component.h>
#include <engine/transform.h>
#include <engine/editor.h>
#include <graphics/texture.h>

namespace sfge
{
class GraphicsManager;
/**
* \brief Sprite component used in the GameObject
*/
class Sprite:
	public LayerComponent, public TransformRequiredComponent, public Offsetable
{
public:

	Sprite();
	Sprite(Transform2d* transform, sf::Vector2f offset);

	void Init();
	
	void Draw(sf::RenderWindow& window);


	void SetTexture(sf::Texture* newTexture);



	
protected:
	sf::Sprite sprite;
};


namespace editor
{
struct SpriteInfo : ComponentInfo
{
	void DrawOnInspector() override;
	Sprite* sprite = nullptr;
	std::string texturePath = "";
	TextureId textureId = INVALID_TEXTURE;
};
}

/**
* \brief Sprite manager caching all the sprites and rendering them at the end of the frame
*/
class SpriteManager : public ComponentManager<Sprite, editor::SpriteInfo>, 
	public LayerComponentManager<Sprite>, public Module
{
public:
	SpriteManager() = default;
	void Init() override;
	void Update(sf::Time dt) override;
	void Draw(sf::RenderWindow& window);

	void Reset();
	void Collect() override;

	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;

protected:
	std::weak_ptr<GraphicsManager> m_GraphicsManagerPtr;
	std::weak_ptr<Transform2dManager> m_TransformManagerPtr;
	std::weak_ptr<EntityManager> m_EntityManagerPtr;
};




}
#endif // !SFGE_SPRITE
