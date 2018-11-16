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

#include <GraphicsManager.h>
#include <TextureManager.h>
#include <AnimationManager.h>

#include <Engine.h>

void Engine::Init()
{
	//We don't want to initialize an already running Engine
	if (isRunning)
		return;

	//Create every managers.
	m_graphicsManager = new GraphicsManager();
	m_window = m_graphicsManager->Init(this);

	m_animationManager = new AnimationManager();
	m_animationManager->Init();
	
	m_textureManager = new TextureManager();
	m_textureManager->Init();

	isRunning = true;
}

void Engine::Start()
{
	sf::Clock clock;

	while(isRunning && m_window != nullptr)
	{
		int dt = clock.getElapsedTime().asMilliseconds();

		Update(dt);

		clock.restart();
	}
	StopEngine();
}

void Engine::Update(int dt)
{
	m_graphicsManager->Update(dt);
}

void Engine::ExitApplication()
{
	isRunning = false;
}

void Engine::StopEngine()
{
	m_graphicsManager->Stop();
	m_window = nullptr;

	delete(m_animationManager);
	delete(m_graphicsManager);
	delete(m_textureManager);
}

GraphicsManager* Engine::GetGraphicsManager()
{
	return m_graphicsManager;
}
TextureManager* Engine::GetTextureManager()
{
	return m_textureManager;
}
AnimationManager* Engine::GetAnimationManager()
{
	return m_animationManager;
}