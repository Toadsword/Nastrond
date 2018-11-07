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
#include <editor/editor.h>
#include <engine/transform2d.h>
#include <physics/physics2d.h>
#include <engine/log.h>
#include <engine/systems_container.h>


namespace sfge
{

/*Engine::Engine()
{
	instance = this;
	m_EntityManager = std::make_shared<EntityManager>();
	m_Transform2dManager = std::make_shared<Transform2dManager>();
	m_Graphics2dManager = std::make_shared<Graphics2dManager>();
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
    {
        std::ostringstream oss;
        oss << "Number of cores on machine: "<<std::thread::hardware_concurrency ();
        Log::GetInstance ()->Msg (oss.str ());
    }
    m_ThreadPool.resize(std::thread::hardware_concurrency ()-1);



	m_EntityManager.Init();
	m_Graphics2dManager.Init();
	m_AudioManager.Init();
	m_SceneManager.Init();
	m_InputManager.Init();
	m_PythonEngine.Init();
	m_PhysicsManager.Init();
	m_Editor.Init();

	m_Window = m_Graphics2dManager.GetWindow();
	running = true;
}

void Engine::Start()
{
	sf::Clock globalClock;
	sf::Clock updateClock;
	sf::Clock fixedUpdateClock;
	sf::Clock graphicsUpdateClock;
	sf::Time previousFixedUpdateTime = globalClock.getElapsedTime();
	sf::Time deltaFixedUpdateTime = sf::Time();
	sf::Time dt = sf::Time();
	while (running && m_Window != nullptr)
	{
		bool isFixedUpdateFrame = false;
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
		
		m_InputManager.Update(dt.asSeconds());
		sf::Time fixedUpdateTime = globalClock.getElapsedTime() - previousFixedUpdateTime;
		if (fixedUpdateTime.asSeconds() > m_Config->fixedDeltaTime)
		{
			fixedUpdateClock.restart ();
			m_PhysicsManager.FixedUpdate();
			previousFixedUpdateTime = globalClock.getElapsedTime();
			m_PythonEngine.FixedUpdate();
			m_SceneManager.FixedUpdate();
			deltaFixedUpdateTime = fixedUpdateClock.getElapsedTime ();
			m_FrameData.frameFixedUpdate = deltaFixedUpdateTime;
			isFixedUpdateFrame = true;
		}
		m_PythonEngine.Update(dt.asSeconds());

		m_SceneManager.Update(dt.asSeconds());

		m_Editor.Update(dt.asSeconds());
		graphicsUpdateClock.restart ();
		m_TransformManager.Update(dt.asSeconds());
		m_Graphics2dManager.Update(dt.asSeconds());
		m_PythonEngine.Draw();
		m_Editor.Draw();
		m_Graphics2dManager.Display();
		const sf::Time graphicsDt = graphicsUpdateClock.getElapsedTime ();
		dt = updateClock.restart();
		if(isFixedUpdateFrame)
		{
			m_FrameData.graphicsTime = graphicsDt;
			m_FrameData.frameTotalTime = dt;
		}
	}
	Destroy();
}

void Engine::Destroy() 
{
	m_EntityManager.Destroy();
	m_Graphics2dManager.Destroy();
	m_AudioManager.Destroy();
	m_SceneManager.Destroy();
	m_InputManager.Destroy();
	m_Editor.Destroy();
	m_PhysicsManager.Destroy();

	m_PythonEngine.Destroy();
}

void Engine::Clear() 
{
	m_EntityManager.Clear();
	m_Graphics2dManager.Clear();
	m_AudioManager.Clear();
	m_SceneManager.Clear();
	m_InputManager.Clear();
	m_PythonEngine.Clear();
	m_Editor.Clear();
	m_PhysicsManager.Clear();
}

void Engine::Collect() 
{

	m_EntityManager.Collect();
	m_Graphics2dManager.Collect();
	m_AudioManager.Collect();
	m_SceneManager.Collect();
	m_InputManager.Collect();
	m_PythonEngine.Collect();
	m_Editor.Collect();
	m_PhysicsManager.Collect();
}


Configuration * Engine::GetConfig() const
{
	return m_Config.get();
}

Graphics2dManager& Engine::GetGraphics2dManager() 
{
	return (m_Graphics2dManager);
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

ProfilerFrameData& Engine::GetProfilerFrameData()
{
    return m_FrameData;
}
Engine::Engine()
{
	m_SystemsContainer = std::make_unique<SystemsContainer>(*this);
}

}
