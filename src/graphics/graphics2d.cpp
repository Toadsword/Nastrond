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

#include <sstream>

#include <graphics/graphics2d.h>
#include <engine/engine.h>
#include <utility/log.h>
#include <engine/config.h>

//Dependencies includes
#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>


namespace sfge
{

void Graphics2dManager::Init()
{
	if (const auto configPtr = m_Engine.GetConfig())
	{
		m_Windowless = configPtr->windowLess;
		if (!m_Windowless)
		{
			m_Window = std::make_unique<sf::RenderWindow>(
				sf::VideoMode(configPtr->screenResolution.x, configPtr->screenResolution.y),
				"SFGE 0.1");
			if (configPtr->maxFramerate)
			{
				m_Window->setFramerateLimit(configPtr->maxFramerate);
				CheckVersion();
			}
		}
	}
	else
	{
		Log::GetInstance()->Error("[Error] Config is null from Graphics Manager");
		
	}
	m_TextureManager.Init();
	m_ShapeManager.Init();
	m_SpriteManager.Init();
	m_AnimationManager.Init();
	m_ButtonManager.Init();
	m_TextManager.Init();
	m_ImageManager.Init();
}

void Graphics2dManager::Update(float dt)
{
	if (!m_Windowless)
	{
		rmt_ScopedCPUSample(Graphics2dUpdate,0)
		m_Window->clear();

		m_SpriteManager.Update(dt);
		m_AnimationManager.Update(dt);
		m_ShapeManager.Update(dt);
		m_ButtonManager.Update(dt);
		m_TextManager.Update(dt);
		m_ImageManager.Update(dt);
	}
}

void Graphics2dManager::Draw()
{
	if(!m_Windowless)
	{
		m_SpriteManager.DrawSprites(*m_Window);
		m_AnimationManager.DrawAnimations(*m_Window);
		m_ShapeManager.DrawShapes(*m_Window);
		m_ButtonManager.DrawButtons(*m_Window);
		m_TextManager.DrawTexts(*m_Window);
		m_ImageManager.DrawImages(*m_Window);
	}
}

void Graphics2dManager::Display()
{

	rmt_ScopedCPUSample(Graphics2dDisplay,0)
	if (!m_Windowless)
	{
		m_Window->display();
	}
}

void Graphics2dManager::DrawLine(sf::Vector2f from, sf::Vector2f to, sf::Color color)
{
	sf::Vertex vertices[2] =
	{
	    sf::Vertex(from, color),
	    sf::Vertex(to, color)
	};

	m_Window->draw(vertices, 2, sf::Lines);
}

sf::RenderWindow* Graphics2dManager::GetWindow()
{
	return m_Window.get();
}

SpriteManager* Graphics2dManager::GetSpriteManager()
{
	return &m_SpriteManager;
}

AnimationManager* Graphics2dManager::GetAnimationManager()
{
	return &m_AnimationManager;
}

TextureManager* Graphics2dManager::GetTextureManager()
{
	return &m_TextureManager;
}

ShapeManager* Graphics2dManager::GetShapeManager()
{
	return &m_ShapeManager;
}

ButtonManager* Graphics2dManager::GetButtonManager()
{
	return &m_ButtonManager;
}

TextManager* Graphics2dManager::GetTextManager()
{
	return &m_TextManager;
}

ImageManager* Graphics2dManager::GetImageManager()
{
	return &m_ImageManager;
}

void Graphics2dManager::CheckVersion() const
{
	sf::ContextSettings settings = m_Window->getSettings();
	std::stringstream log_message;
	log_message << "OpenGL version: " << settings.majorVersion << "." << settings.minorVersion << std::endl;

	Log::GetInstance()->Msg(log_message.str());
}

void checkVersion()
{

}


void Graphics2dManager::Destroy()
{
	Clear();
	Collect();

	m_Window = nullptr;
	ImGui::SFML::Shutdown();
}

void Graphics2dManager::Clear()
{
	m_TextureManager.Clear();
	m_SpriteManager.Reset();
}

void Graphics2dManager::Collect()
{

	m_TextureManager.Collect();
	m_SpriteManager.Collect();
}

}
