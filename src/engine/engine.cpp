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
#include <engine/entity.h>
#include <engine/globals.h>
#include <engine/scene.h>
#include <graphics/graphics2d.h>
#include <input/input.h>
#include <python/python_engine.h>
#include <engine/config.h>
#include <audio/audio.h>
#include <engine/editor.h>
#include <engine/transform2d.h>
#include <physics/physics2d.h>
#include <engine/log.h>


namespace sfge
{

/*Engine::Engine()
{
	instance = this;
	m_EntityManager = std::make_shared<EntityManager>();
	m_Transform2dManager = std::make_shared<Transform2dManager>();
	m_GraphicsManager = std::make_shared<Graphics2dManager>();
	m_AudioManager = std::make_shared<AudioManager>();
	m_SceneManager = std::make_shared<SceneManager>();
	m_InputManager = std::make_shared<InputManager>();
	m_PythonEngine = std::make_shared<PythonEngine>();
	m_PhysicsManager = std::make_shared<Physics2dManager>();
	m_Editor = std::make_shared<Editor>();
}*/

void Engine::Init(std::string configFilename)
{
	const auto configJsonPtr = LoadJson(configFilename);
	if (configJsonPtr)
		Init(*configJsonPtr);
}

void Engine::Init(json& configJson)
{
	m_Config = Configuration::LoadConfig(configJson);
	InitModules();
}

void Engine::Init(std::unique_ptr<Configuration> config)
{
	m_Config = std::move(config);
	InitModules();
}

void Engine::InitModules()
{
	if (m_Config == nullptr)
	{
		Log::GetInstance()->Error("[Error] Game Engine Configuration");
	}
	else
	{
		Log::GetInstance()->Msg("Game Engine Configuration Successfull");
	}
	m_ThreadPool.resize(THREAD_NMB);



	m_EntityManager.Init();
	m_GraphicsManager.Init();
	m_AudioManager.Init();
	m_SceneManager.Init();
	m_InputManager.Init();
	m_PythonEngine.Init();
	m_PhysicsManager.Init();
	m_Editor.Init();

	m_Window = m_GraphicsManager.GetWindow();
	running = true;
}

void Engine::Start()
{
	sf::Clock globalClock;
	sf::Clock updateClock;
	sf::Time previousFixedUpdateTime = globalClock.getElapsedTime();
	sf::Time deltaFixedUpdateTime = sf::Time();
	while (running && m_Window != nullptr)
	{
		const sf::Time dt = updateClock.restart();
		sf::Event event{};
		while (m_Window != nullptr && 
			m_Window->pollEvent(event))
		{
			m_Editor.ProcessEvent(event);
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
		
		m_InputManager.Update(dt);
		sf::Time fixedUpdateTime = globalClock.getElapsedTime() + deltaFixedUpdateTime - previousFixedUpdateTime;
		if (fixedUpdateTime.asSeconds() > m_Config->fixedDeltaTime)
		{
			m_PhysicsManager.FixedUpdate();
			deltaFixedUpdateTime = fixedUpdateTime - sf::seconds(m_Config->fixedDeltaTime);
			previousFixedUpdateTime = globalClock.getElapsedTime();
			m_PythonEngine.FixedUpdate();
			m_SceneManager.FixedUpdate();
		}
		m_PythonEngine.Update(dt);

		m_SceneManager.Update(dt);

		m_Editor.Update(dt);

		m_GraphicsManager.Update(dt);
		m_Editor.Draw();
		m_GraphicsManager.Display();
	}
	Destroy();
}

void Engine::Destroy() 
{
	m_GraphicsManager.Destroy();
	m_AudioManager.Destroy();
	m_SceneManager.Destroy();
	m_InputManager.Destroy();
	m_Editor.Destroy();
	m_PhysicsManager.Destroy();

	m_PythonEngine.Destroy();
}

void Engine::Clear() 
{
	m_GraphicsManager.Clear();
	m_AudioManager.Clear();
	m_SceneManager.Clear();
	m_InputManager.Clear();
	m_PythonEngine.Clear();
	m_Editor.Clear();
	m_PhysicsManager.Clear();
}

void Engine::Collect() 
{
	m_GraphicsManager.Collect();
	m_AudioManager.Collect();
	m_SceneManager.Collect();
	m_InputManager.Collect();
	m_PythonEngine.Collect();
	m_Editor.Collect();
	m_PhysicsManager.Collect();
}


std::weak_ptr<Configuration> Engine::GetConfig() const
{
	return std::weak_ptr<Configuration>(m_Config);
}

Graphics2dManager& Engine::GetGraphicsManager() 
{
	return (m_GraphicsManager);
}

AudioManager& Engine::GetAudioManager() 
{
	return (m_AudioManager);
}

SceneManager& Engine::GetSceneManager() 
{
	return (m_SceneManager);
}

InputManager& Engine::GetInputManager() 
{
	return (m_InputManager);
}

PythonEngine& Engine::GetPythonEngine() 
{
	return (m_PythonEngine);
}

Physics2dManager& Engine::GetPhysicsManager() 
{
	return (m_PhysicsManager);
}

EntityManager& Engine::GetEntityManager() 
{
	return (m_EntityManager);
}

Transform2dManager& Engine::GetTransform2dManager() 
{
	return (m_TransformManager);
}

Editor& Engine::GetEditor() 
{
	return (m_Editor);
}

ctpl::thread_pool & Engine::GetThreadPool()
{
	return m_ThreadPool;
}

}
