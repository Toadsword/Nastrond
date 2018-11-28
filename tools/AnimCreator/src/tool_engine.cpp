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

#include <graphics_manager.h>
#include <texture_manager.h>
#include <animation_manager.h>

#include <tool_engine.h>
#include <iostream>

void ToolEngine::Init()
{
	//We don't want to initialize an already running tool_engine
	if (isRunning)
		return;

	//Create every managers.
	m_Window = m_GraphicsManager.Init(this);

	m_AnimationManager.Init();

	m_TextureManager.Init();

	isRunning = true;
}

void ToolEngine::Start()
{
	sf::Clock clock;

	while(isRunning && m_Window != nullptr)
	{
		int dt = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		Update(dt);
	}
	StopEngine();
}

void ToolEngine::Update(int dt)
{
	m_GraphicsManager.Update(dt);
}

void ToolEngine::ExitApplication()
{
	isRunning = false;
}

void ToolEngine::StopEngine()
{
	m_GraphicsManager.Stop();
	m_Window = nullptr;
}

GraphicsManager* ToolEngine::GetGraphicsManager()
{
	return &m_GraphicsManager;
}
TextureManager* ToolEngine::GetTextureManager()
{
	return &m_TextureManager;
}
AnimationManager* ToolEngine::GetAnimationManager()
{
	return &m_AnimationManager;
}