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
#include <engine/globals.h>

#include <utility/log.h>
#include <engine/systems_container.h>



namespace sfge
{

Engine::Engine()
{
	m_SystemsContainer = std::make_unique<SystemsContainer>(*this);

	rmt_CreateGlobalInstance(&rmt);
}
Engine::~Engine()
{
	m_SystemsContainer = nullptr;
}

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
		Log::GetInstance()->Error("[Error] Game tool_engine Configuration");
	}
	else
	{
		Log::GetInstance()->Msg("Game tool_engine Configuration Successfull");
	}
    {
        std::ostringstream oss;
        oss << "Number of cores on machine: "<<std::thread::hardware_concurrency ();
        Log::GetInstance ()->Msg (oss.str ());
    }
    m_ThreadPool.resize(std::thread::hardware_concurrency ()-1);



	m_SystemsContainer->entityManager.Init();
	m_SystemsContainer->transformManager.Init();
    m_SystemsContainer->graphics2dManager.Init();
    m_SystemsContainer->audioManager.Init();
    m_SystemsContainer->sceneManager.Init();
    m_SystemsContainer->inputManager.Init();
    m_SystemsContainer->pythonEngine.Init();
    m_SystemsContainer->physicsManager.Init();
    m_SystemsContainer->editor.Init();

	m_Window = m_SystemsContainer->graphics2dManager.GetWindow();
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

	rmt_BindOpenGL();
	while (running && m_Window != nullptr)
	{

		rmt_ScopedOpenGLSample(SFGE_Frame_GL);
		rmt_ScopedCPUSample(SFGE_Frame,0)

		bool isFixedUpdateFrame = false;
		sf::Event event{};
		while (m_Window != nullptr && 
			m_Window->pollEvent(event))
		{
            m_SystemsContainer->editor.ProcessEvent(event);
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

        m_SystemsContainer->inputManager.Update(dt.asSeconds());
		auto fixedUpdateTime = globalClock.getElapsedTime() - previousFixedUpdateTime;
		if (fixedUpdateTime.asSeconds() > m_Config->fixedDeltaTime)
		{
			fixedUpdateClock.restart ();
			m_SystemsContainer->physicsManager.FixedUpdate();
			previousFixedUpdateTime = globalClock.getElapsedTime();
			m_SystemsContainer->pythonEngine.FixedUpdate();
            m_SystemsContainer->sceneManager.FixedUpdate();
			deltaFixedUpdateTime = fixedUpdateClock.getElapsedTime ();
			m_FrameData.frameFixedUpdate = deltaFixedUpdateTime;
			isFixedUpdateFrame = true;
		}
        m_SystemsContainer->pythonEngine.Update(dt.asSeconds());

        m_SystemsContainer->sceneManager.Update(dt.asSeconds());
		
        m_SystemsContainer->editor.Update(dt.asSeconds());
		graphicsUpdateClock.restart ();
        m_SystemsContainer->transformManager.Update(dt.asSeconds());
		
        m_SystemsContainer->graphics2dManager.Update(dt.asSeconds());

		m_SystemsContainer->sceneManager.Draw();

		m_SystemsContainer->graphics2dManager.Draw();

        m_SystemsContainer->pythonEngine.Draw();
        m_SystemsContainer->editor.Draw();
        m_SystemsContainer->graphics2dManager.Display();

		const sf::Time graphicsDt = graphicsUpdateClock.getElapsedTime ();
		dt = updateClock.restart();
		if(isFixedUpdateFrame)
		{
			m_FrameData.graphicsTime = graphicsDt;
			m_FrameData.frameTotalTime = dt;
		}
	}

	rmt_UnbindOpenGL();
	Destroy();
}

void Engine::Destroy() 
{

	m_SystemsContainer->pythonEngine.Destroy();
	m_SystemsContainer->entityManager.Destroy();
	m_SystemsContainer->graphics2dManager.Destroy();
	m_SystemsContainer->audioManager.Destroy();
	m_SystemsContainer->sceneManager.Destroy();
	m_SystemsContainer->inputManager.Destroy();
	m_SystemsContainer->editor.Destroy();
	m_SystemsContainer->physicsManager.Destroy();
	rmt_DestroyGlobalInstance(rmt);

}

void Engine::Clear() 
{
	m_SystemsContainer->entityManager.Clear();
	m_SystemsContainer->graphics2dManager.Clear();
	m_SystemsContainer->audioManager.Clear();
	m_SystemsContainer->sceneManager.Clear();
	m_SystemsContainer->inputManager.Clear();
	m_SystemsContainer->pythonEngine.Clear();
	m_SystemsContainer->editor.Clear();
	m_SystemsContainer->physicsManager.Clear();
}

void Engine::Collect() 
{

	m_SystemsContainer->entityManager.Collect();
	m_SystemsContainer->graphics2dManager.Collect();
	m_SystemsContainer->audioManager.Collect();
	m_SystemsContainer->sceneManager.Collect();
	m_SystemsContainer->inputManager.Collect();
	m_SystemsContainer->pythonEngine.Collect();
	m_SystemsContainer->editor.Collect();
	m_SystemsContainer->physicsManager.Collect();
}


Configuration * Engine::GetConfig() const
{
	return m_Config.get();
}

Graphics2dManager* Engine::GetGraphics2dManager()
{
	return m_SystemsContainer?&m_SystemsContainer->graphics2dManager:nullptr;
}

AudioManager* Engine::GetAudioManager()
{
	return m_SystemsContainer ? &m_SystemsContainer->audioManager : nullptr;
}

SceneManager* Engine::GetSceneManager() 
{
	return m_SystemsContainer ? &m_SystemsContainer->sceneManager : nullptr;
}

InputManager* Engine::GetInputManager() 
{
	return m_SystemsContainer ? &m_SystemsContainer->inputManager : nullptr;
}

PythonEngine* Engine::GetPythonEngine() 
{
	return m_SystemsContainer ? &m_SystemsContainer->pythonEngine : nullptr;
}

Physics2dManager* Engine::GetPhysicsManager() 
{
	return m_SystemsContainer ? &m_SystemsContainer->physicsManager : nullptr;
}

EntityManager* Engine::GetEntityManager() 
{
	return m_SystemsContainer ? &m_SystemsContainer->entityManager : nullptr;
}

Transform2dManager* Engine::GetTransform2dManager() 
{
	return m_SystemsContainer ? &m_SystemsContainer->transformManager : nullptr;
}

Editor* Engine::GetEditor() 
{
	return m_SystemsContainer ? &m_SystemsContainer->editor : nullptr;
}

TilemapManager* Engine::GetTilemapManager()
{
	return m_SystemsContainer ? &m_SystemsContainer->tilemapManager : nullptr;
}

ctpl::thread_pool & Engine::GetThreadPool()
{
	return m_ThreadPool;
}

ProfilerFrameData& Engine::GetProfilerFrameData()
{
    return m_FrameData;
}

}
