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

#ifndef SFGE_ENGINE_H
#define SFGE_ENGINE_H



#include <memory>
#include <string>
#include <ctpl_stl.h>


#include <utility/json_utility.h>
#include <graphics/graphics2d.h>
#include <audio/audio.h>
#include <engine/scene.h>
#include <input/input.h>
#include <python/python_engine.h>
#include <physics/physics2d.h>
#include <engine/entity.h>
#include <engine/transform2d.h>
#include <editor/editor.h>
#include <editor/profiler.h>
namespace sf
{
class sf::RenderWindow;
}
namespace sfge
{

/**
* Prototypes declarations
*/

struct Configuration;

/**
* \brief The main Engine class to centralise the frame process and the references
*/
class Engine
{
public:
	Engine() = default;
	/**
	* \brief Initialize all the modules of the Game Engine, reading the config file too
	*/
	void Init(std::string configFilename="data/config.json");
	void Init(json& configJson);
	void Init(std::unique_ptr<Configuration> config);
	/**
	* \brief Starting the Game Engine after the Init()
	*/
	void Start();

	/**
	 * \brief Destroy all the modules
	 */
	void Destroy();
	void Clear();
	/**
	* \brief Reload is used after loading a new scene
	*/
	void Collect();

	~Engine() = default;
	/**
	* \brief A getter of the Configuration
	* \return The Configuration struct got by the Engine
	*/
	std::weak_ptr<Configuration> GetConfig() const;

	Graphics2dManager& GetGraphics2dManager();
	AudioManager& GetAudioManager();
	SceneManager& GetSceneManager();
	InputManager& GetInputManager();
	PythonEngine& GetPythonEngine();
	Physics2dManager& GetPhysicsManager();
	EntityManager& GetEntityManager();
	Transform2dManager& GetTransform2dManager();
	Editor& GetEditor();

	ctpl::thread_pool& GetThreadPool();
	ProfilerFrameData& GetProfilerFrameData();
	bool running = false;
protected:
	void InitModules();
	ctpl::thread_pool m_ThreadPool;
	std::shared_ptr<sf::RenderWindow> m_Window;
	std::shared_ptr<Configuration> m_Config = nullptr;


	//module
	Graphics2dManager m_Graphics2dManager {*this};
	AudioManager m_AudioManager {*this};
	SceneManager m_SceneManager {*this};
	InputManager m_InputManager {*this};
	PythonEngine m_PythonEngine {*this};
	Physics2dManager m_PhysicsManager {*this};
	Editor m_Editor {*this};
	EntityManager m_EntityManager {*this};
	Transform2dManager m_TransformManager {*this};

  	ProfilerFrameData m_FrameData;

};


}

#endif // !SFGE_ENGINE_H
