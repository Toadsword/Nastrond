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

#include <memory>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


#include <engine/engine.h>
#include <engine/scene.h>
#include <graphics/graphics.h>
#include <input/input.h>
#include <python/python_engine.h>
#include <engine/config.h>
#include <audio/audio.h>
#include <engine/editor.h>
#include <graphics/sprite.h>
#include <physics/physics.h>
#include <engine/log.h>


namespace sfge
{


void Engine::Init(bool windowless, bool editor)
{
	m_Config = std::move(Configuration::LoadConfig());
	if (m_Config == nullptr)
	{
		Log::GetInstance()->Error("[Error] Game Engine is null");
	}
	else
	{
		Log::GetInstance()->Msg("Game Engine Configuration Successfull");
	}

	m_GraphicsManager = std::make_unique<GraphicsManager>(*this, true, windowless);
	m_AudioManager = std::make_unique<AudioManager>(*this, true);
	m_SceneManager = std::make_unique<SceneManager>(*this, true);
	m_InputManager = std::make_unique<InputManager>(*this, true);
	m_PythonManager = std::make_unique<PythonManager>(*this, true);
	m_PhysicsManager = std::make_unique<PhysicsManager>(*this, true);
	m_Editor = std::make_unique<Editor>(*this, editor);
	m_EntityManager = std::make_unique<EntityManager>();

	m_GraphicsManager->Init();
	m_AudioManager->Init();
	m_SceneManager->Init();
	m_InputManager->Init();
	m_PythonManager->Init();
	m_PhysicsManager->Init();
	m_Editor->Init();

	m_Window = m_GraphicsManager->GetWindow();
	running = true;


}

void Engine::Start()
{
	sf::Clock updateClock;
	sf::Clock fixedUpdateClock;
	while (running && m_Window != nullptr)
	{
		const sf::Time dt = updateClock.restart();
		sf::Event event{};
		while (m_Window != nullptr && m_Window->pollEvent(event))
		{
			m_Editor->ProcessEvent(event);
			if (event.type == sf::Event::Closed)
			{
				running = false;
				m_Window->close();
			}
			
		}
		if (!running)
		{
			continue;
		}
		
		m_InputManager->Update(dt);
		if (fixedUpdateClock.getElapsedTime().asSeconds() < m_Config->fixedDeltaTime)
		{
			m_PhysicsManager->Update(dt);
			fixedUpdateClock.restart();
			m_PythonManager->FixedUpdate();
			m_SceneManager->FixedUpdate();
		}
		m_PythonManager->Update(dt);

		m_SceneManager->Update(dt);

		m_Editor->Update(dt);
		if (m_SceneManager->IsSwitching())
		{
			Collect();
		}
		m_GraphicsManager->Update(dt);
		m_Editor->Draw();
		m_GraphicsManager->Display();
	}
	Destroy();
}

void Engine::Destroy()
{
	m_GraphicsManager->Destroy();
	m_AudioManager->Destroy();
	m_SceneManager->Destroy();
	m_InputManager->Destroy();
	m_PythonManager->Destroy();
	m_Editor->Destroy();
	m_PhysicsManager->Destroy();
	m_Window = nullptr;
}

void Engine::Reset()
{
	m_GraphicsManager->Reset();
	m_AudioManager->Reset();
	m_SceneManager->Reset();
	m_InputManager->Reset();
	m_PythonManager->Reset();
	m_Editor->Reset();
	m_PhysicsManager->Reset();
}

void Engine::Collect()
{
	m_GraphicsManager->Collect();
	m_AudioManager->Collect();
	m_SceneManager->Collect();
	m_InputManager->Collect();
	m_PythonManager->Collect();
	m_Editor->Collect();
	m_PhysicsManager->Collect();
}


	std::shared_ptr<Configuration> Engine::GetConfig()
{
	return m_Config;
}

GraphicsManager & Engine::GetGraphicsManager() const
{
	return *m_GraphicsManager;
}

AudioManager & Engine::GetAudioManager() const
{
	return *m_AudioManager;
}

SceneManager & Engine::GetSceneManager() const
{
	return *m_SceneManager;
}

InputManager & Engine::GetInputManager() const
{
	return *m_InputManager;
}

PythonManager & Engine::GetPythonManager() const
{
	return *m_PythonManager;
}

PhysicsManager & Engine::GetPhysicsManager() const
{
	return *m_PhysicsManager;
}


Module::Module(Engine& engine, bool enable=true) :
		m_Engine(engine)
{
	m_Enable = enable;

}

void Module::SetEnable(bool enable)
{
}

bool Module::GetEnable()
{
	return false;
}


}
