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
#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <engine/engine.h>
#include <engine/globals.h>

#include <utility/log.h>
#include <utility/file_utility.h>
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
	m_SystemsContainer->rectTransformManager.Init();
    m_SystemsContainer->graphics2dManager.Init();
	m_SystemsContainer->uiManager.Init();
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

		m_SystemsContainer->rectTransformManager.Update(dt.asSeconds());
		m_SystemsContainer->uiManager.Update(dt.asSeconds());

        m_SystemsContainer->graphics2dManager.Update(dt.asSeconds());

		m_SystemsContainer->sceneManager.Draw();

		m_SystemsContainer->uiManager.Draw();
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

void Engine::Save()
{
	json j;
	j["name"] = m_SystemsContainer->editor.GetCurrentSceneName();
	j["entities"] = nlohmann::detail::value_t::array;

	//Gathering datas from the managers
	json transformSave = m_SystemsContainer->transformManager.Save();
	json spriteSave = m_SystemsContainer->graphics2dManager.GetSpriteManager()->Save();
	json pyComponentSave = m_SystemsContainer->pythonEngine.GetPyComponentManager().Save();
	json tilemapSave = m_SystemsContainer->graphics2dManager.GetTilemapSystem()->Save();
	json cameraSave = m_SystemsContainer->graphics2dManager.GetCameraManager()->Save();

	bool hasAtLeastOneComponent = false;
	unsigned entityIndex = 1;
	unsigned componentIndex = 0;

	std::string referencePath = TILEMAP_FOLDER + "coucou.asset";
	//Save of the tileAsset file
	if(CheckJsonExists(tilemapSave, "tiletype"))
	{
		// Copy all the sprites needed for the tiletypes into the data folder
		for(int i = 0; i < tilemapSave["tiletype"].size(); i++)
		{
			std::string filepath = tilemapSave["tiletype"][i]["texturePath"].get<std::string>();

			char slash = '/';
			if (!(filepath.find(slash) < filepath.length()))
				slash = '\\';
			
			std::string newPath = SPRITE_FOLDER + filepath.substr(filepath.find_last_of(slash) + 1);
			if(!FileExists(filepath))
				CopyFile(filepath, newPath);
			
			tilemapSave["tiletype"][i]["texturePath"] = newPath;
		}

		// File write
		std::ofstream myfile;
		myfile.open(referencePath);
		myfile.flush();
		myfile << std::setw(4) << tilemapSave["tiletype"] << std::endl;
		myfile.close();
	}

	//Save of the scene file
	//Loop on all the entities
	for (int i = 1; i < INIT_ENTITY_NMB; i++)
	{
		hasAtLeastOneComponent = false;
		componentIndex = 0;

		//Transform2d
		if (CheckJsonExists(transformSave[i - 1], "position"))
		{
			j["entities"][entityIndex - 1]["components"][componentIndex] = transformSave[i - 1];
			hasAtLeastOneComponent = true;
			componentIndex++;
		}

		//Sprite
		if (CheckJsonExists(spriteSave[i - 1], "path"))
		{
			j["entities"][entityIndex - 1]["components"][componentIndex] = spriteSave[i - 1];
			hasAtLeastOneComponent = true;
			componentIndex++;
		}

		//Pycomponent
		if (CheckJsonExists(pyComponentSave[i][0], "script_path"))
		{
			for(int iPyComp = 0; iPyComp < pyComponentSave[i].size(); iPyComp++)
			{
				j["entities"][entityIndex - 1]["components"][componentIndex] = pyComponentSave[i][iPyComp];
				componentIndex++;
			}
			hasAtLeastOneComponent = true;
		}

		//Tilemap
		if (CheckJsonExists(tilemapSave["tilemap"][i - 1], "map"))
		{
			j["entities"][entityIndex - 1]["components"][componentIndex] = tilemapSave["tilemap"][i - 1];
			j["entities"][entityIndex - 1]["components"][componentIndex]["reference_path"] = referencePath;
			hasAtLeastOneComponent = true;
			componentIndex++;
		}

		//Camera
		if (CheckJsonExists(cameraSave[i - 1], "type"))
		{
			j["entities"][entityIndex - 1]["components"][componentIndex] = cameraSave[i - 1];
			hasAtLeastOneComponent = true;
			componentIndex++;
		}

		//If we stored something, we can add the name of the entity.
		if (hasAtLeastOneComponent)
		{
			j["entities"][entityIndex - 1]["name"] = m_SystemsContainer->entityManager.GetEntityInfo(i).name;
			entityIndex++;
		}
	}

	int indexMakeUnique = 1;
	std::string sceneFilename = SCENE_FOLDER + "saved_scene.scene";
	while(FileExists(sceneFilename))
	{
		sceneFilename = SCENE_FOLDER + "saved_scene" + std::to_string(indexMakeUnique) + ".scene";
		indexMakeUnique++;
	}

	// File write
	std::ofstream myfile;
	myfile.open(sceneFilename);
	myfile.flush();
	myfile << std::setw(4) << j << std::endl;
	myfile.close();
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

RectTransformManager* Engine::GetRectTransformManager()
{
	return m_SystemsContainer ? &m_SystemsContainer->rectTransformManager : nullptr;
}

UIManager* Engine::GetUIManager()
{
	return m_SystemsContainer ? &m_SystemsContainer->uiManager : nullptr;
}

Editor* Engine::GetEditor() 
{
	return m_SystemsContainer ? &m_SystemsContainer->editor : nullptr;
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
