#ifndef ENGINE_H
#define ENGINE_H
#include "SFML/Graphics/RenderWindow.hpp"

#include "AnimationManager.h"
#include "TextureManager.h"
#include "GraphicsManager.h"

class Engine
{
public:
	Engine::Engine() {};

	void Init();
	void Start();
	void Update();
	void Stop();

	GraphicsManager* GetGraphicsManager();
	TextureManager* GetTextureManager();
	AnimationManager* GetAnimationManager();

private:
	GraphicsManager* m_graphicsManager;
	TextureManager* m_textureManager;
	AnimationManager* m_animationManager;
	sf::RenderWindow* m_window = nullptr;

	sf::Clock m_clock;
	bool isRunning = false;
};

#endif // ifndef ENGINE_H