/*
MIT License

Copyright (c) 2018 SAE Institute Switzerland AG

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

----------------------------
Author : Duncan Bourquard (bourquardduncan@gmail.com)
Date : November 2018
Project : AnimationTool for SFGE
*/


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