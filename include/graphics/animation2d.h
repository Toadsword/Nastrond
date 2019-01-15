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

#ifndef SFGE_ANIMATOR_H
#define SFGE_ANIMATOR_H

//STL
#include <string>
//Dependencies
#include <SFML/Graphics.hpp>
//tool_engine
#include <engine/component.h>
#include <engine/transform2d.h>
#include <editor/editor.h>
#include <graphics/texture.h>

namespace sfge
{

const std::string ANIM_FOLER = "./data/animSaves/";

struct AnimationFrame
{
	short key;
	std::string filename;
	sf::Texture* texture;
	sf::IntRect textureRect;
};

class Graphics2dManager;
/**
* \brief Animation component used in the GameObject
*/
class Animation:
	public LayerComponent, public TransformRequiredComponent, public Offsetable
{
public:

	Animation();
	Animation(Transform2d* transform, sf::Vector2f offset);

	void Init();
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	void SetAnimation(std::vector<AnimationFrame> newFrameList, float newSpeed, bool newIsLooped);

protected:
	std::vector<AnimationFrame> frameList;
	sf::Sprite sprite;
	short currentFrameKey = 0;
	float timeSinceChangedFrame = 0;
	float speed = 0.1f;
	bool isLooped = false;
};


namespace editor
{
struct AnimationInfo : ComponentInfo
{
	void DrawOnInspector() override;
	Animation* animation = nullptr;
	std::string name = "";
	int speed = 0.1f;
	bool isLooped = false;
};
}

/**
* \brief Animation manager caching all the animations and rendering them at the end of the frame
*/
class AnimationManager : public SingleComponentManager<Animation, editor::AnimationInfo, ComponentType::ANIMATION2D>,
	public LayerComponentManager<Animation>
{
public:
	using SingleComponentManager::SingleComponentManager;
	void Init() override;
	void Update(float dt) override;
	void DrawAnimations(sf::RenderWindow &window);

	void Reset();
	void Collect() override;
	Animation* AddComponent(Entity entity) override;
	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;

protected:
	Graphics2dManager* m_GraphicsManager;
	Transform2dManager* m_Transform2dManager;
};

}
#endif // !SFGE_ANIMATION
