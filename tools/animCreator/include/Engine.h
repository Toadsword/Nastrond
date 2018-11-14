#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>

#include <AnimationManager.h>
#include <TextureManager.h>
#include <GraphicsManager.h>

class Engine
{
public:
	Engine::Engine() {};

	void Init();
	void Start();
	void Update(int dt);
	void ExitApplication();

	GraphicsManager* GetGraphicsManager();
	TextureManager* GetTextureManager();
	AnimationManager* GetAnimationManager();

private:
	void StopEngine();

	GraphicsManager* m_graphicsManager;
	TextureManager* m_textureManager;
	AnimationManager* m_animationManager;
	sf::RenderWindow* m_window = nullptr;

	bool isRunning = false;
};

#endif // ifndef ENGINE_H