#ifndef ENGINE_H
#define ENGINE_H

class GraphicsManager;
class TextureManager;
class AnimationManager;

class Engine
{
public:
	void Init();
	void Update();
	void Start();

	GraphicsManager& graphicsManager;
	TextureManager& textureManager;
	AnimationManager& animationManager;
};

#endif // ifndef ENGINE_H