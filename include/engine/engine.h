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

#include <SFML/Graphics.hpp>

#include <utility/json_utility.h>
#include <utility/singleton.h>

//modules
#include <audio/audio.h>
#include <input/input.h>
#include <engine/editor.h>
#include <engine/transform2d.h>
#include <engine/entity.h>
#include <engine/scene.h>
#include <python/python_engine.h>
#include <graphics/graphics2d.h>
#include <physics/physics2d.h>

namespace sfge
{

/**
* Prototypes declarations
*/
struct Configuration;

enum class EngineModule
{
	GRAPHICS_MANAGER,
	AUDIO_MANAGER,
	INPUT_MANAGER,
	PYTHON_MANAGER,
	SCENE_MANAGER,
	EDITOR,
	PHYSICS_MANAGER,
	LENGTH
};

/**
* \brief The main Engine class to centralise the frame process and the references
*/
class Engine : public Singleton<Engine>
{
public:
	Engine();
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
	void Destroy() const;
	void Clear() const;
	/**
	* \brief Reload is used after loading a new scene
	*/
	void Collect() const;

	~Engine() = default;
	/**
	* \brief A getter of the Configuration
	* \return The Configuration struct got by the Engine
	*/
	std::weak_ptr<Configuration> GetConfig() const;


	AudioManager& GetAudioManager() const;
	SceneManager& GetSceneManager() const;
	InputManager& GetInputManager() const;
	PythonEngine& GetPythonEngine() const;

    EntityManager& GetEntityManager() const;
    Editor& GetEditor() const;

    Transform2dManager& GetTransform2dManager() const;
	Physics2dManager& GetPhysicsManager() const;
  	Graphics2dManager& GetGraphicsManager() const;

	ctpl::thread_pool& GetThreadPool();
	bool running = false;
protected:
	void InitModules();
	ctpl::thread_pool m_ThreadPool;
	std::shared_ptr<sf::RenderWindow> m_Window;
	std::shared_ptr<Configuration> m_Config = nullptr;


	//module
	Graphics2dManager m_GraphicsManager;
	AudioManager m_AudioManager;
	SceneManager m_SceneManager;
	InputManager m_InputManager;
	PythonEngine m_PythonEngine;
	Physics2dManager m_PhysicsManager;
	Editor m_Editor;
	EntityManager m_EntityManager;
	Transform2dManager m_TransformManager;

};


}

#endif // !SFGE_ENGINE_H
