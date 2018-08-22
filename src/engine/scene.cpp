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

//SFGE includes
#include <engine/scene.h>
#include <engine/log.h>
#include <utility/json_utility.h>
#include <engine/editor.h>
#include <engine/config.h>
#include <utility/file_utility.h>
#include <engine/entity.h>
#include <graphics/graphics2d.h>
#include <python/python_engine.h>
#include <physics/physics2d.h>
#include <engine/engine.h>

// for convenience


namespace sfge
{
SceneManager::SceneManager(Engine& engine):
	System(engine),
	m_EntityManager(m_Engine.GetEntityManager())
{
}

void SceneManager::Init()
{
	
	if(auto config = m_Engine.GetConfig().lock())
	{
		SearchScenes(config->dataDirname);
	}
	else
	{
		Log::GetInstance()->Error("No config in SceneManager Init");
	}
}

void SceneManager::SearchScenes(std::string& dataDirname)
{
	std::function<void(std::string)> SearchAllScenes;
	SearchAllScenes = [&SearchAllScenes, this](std::string entry)
	{
		
		if (IsRegularFile(entry))
		{
			const auto folderLastIndex = entry.find_last_of('/');
			const std::string::size_type filenameExtensionIndex = entry.find_last_of('.');
			const std::string extension = entry.substr(filenameExtensionIndex);
			if(extension == ".scene")
			{
				const auto sceneJsonPtr = LoadJson(entry);
				if(sceneJsonPtr && CheckJsonExists(*sceneJsonPtr, "name"))
				{
					m_ScenePathMap.insert(std::pair<std::string, std::string>((*sceneJsonPtr)["name"],entry));
				}
			}
		}

		if (IsDirectory(entry))
		{
			{
				std::ostringstream oss;
				oss << "Opening folder: " << entry << "\n";
				Log::GetInstance()->Msg(oss.str());
			}
			IterateDirectory(entry, SearchAllScenes);
		}
	};
	IterateDirectory(dataDirname, SearchAllScenes);
}


void SceneManager::LoadSceneFromPath(const std::string& scenePath) const
{
	{
		std::ostringstream oss;
		oss << "Loading scene from: " << scenePath;
		Log::GetInstance()->Msg(oss.str());
	}
	const auto sceneJsonPtr = LoadJson(scenePath);
	
	if(sceneJsonPtr != nullptr)
	{
		auto sceneInfo = std::make_unique<editor::SceneInfo>();
		sceneInfo->path = scenePath;
		LoadSceneFromJson(*sceneJsonPtr, std::move(sceneInfo));
	}
	else
	{
		Log::GetInstance()->Error("Invalid JSON format for scene");
	}

	
}



void SceneManager::LoadSceneFromJson(json& sceneJson, std::unique_ptr<editor::SceneInfo> sceneInfo) const
{
	m_Engine.Clear();
	if(!sceneInfo)
		sceneInfo = std::make_unique<editor::SceneInfo>();
	if (CheckJsonParameter(sceneJson, "name", json::value_t::string))
	{
		sceneInfo->name = sceneJson["name"].get<std::string>();
	}
	else
	{
		sceneInfo->name = "NewScene";
	}
	{
		std::ostringstream oss;
		oss << "Loading scene: " << sceneInfo->name;
		Log::GetInstance()->Msg(oss.str());
	}
	if (CheckJsonParameter(sceneJson, "entities", json::value_t::array))
	{
		for(auto& entityJson : sceneJson["entities"])
		{
			Entity entity = INVALID_ENTITY;
			entity = m_EntityManager.CreateEntity();
			if(CheckJsonExists(entityJson, "name"))
			{
				m_EntityManager.GetEntityInfo(entity).name = entityJson["name"].get<std::string>();
			}
			else
			{
				std::ostringstream oss;
				oss << "Entity " << entity;
				m_EntityManager.GetEntityInfo(entity).name = oss.str();
			}
			if (entity != INVALID_ENTITY && 
				CheckJsonExists(entityJson, "components"))
			{
				for (auto& componentJson : entityJson["components"])
				{
					if (CheckJsonExists(componentJson, "type"))
					{
						const ComponentType componentType = componentJson["type"];
						switch (componentType)
						{
						case ComponentType::TRANSFORM2D:
						{
							auto& transform2dManager = m_Engine.GetTransform2dManager();

							transform2dManager.CreateComponent(componentJson, entity);
							m_EntityManager.AddComponentType(entity, ComponentType::TRANSFORM2D);
							break;
						}
						case ComponentType::SHAPE2D:
						{
							auto& graphicsManager = m_Engine.GetGraphicsManager();
							auto& shapeManager = graphicsManager.GetShapeManager();
							shapeManager.CreateComponent(componentJson, entity);
							m_EntityManager.AddComponentType(entity, ComponentType::SHAPE2D);
							break;
						}
						case ComponentType::BODY2D:
						{
							auto& physicsManager = m_Engine.GetPhysicsManager();
							physicsManager.GetBodyManager().CreateComponent(componentJson, entity);
							m_EntityManager.AddComponentType(entity, ComponentType::BODY2D);

							break;
						}
						case ComponentType::SPRITE2D:
						{
							auto& graphicsManager = m_Engine.GetGraphicsManager();
							auto& spriteManager = graphicsManager.GetSpriteManager();
							spriteManager.CreateComponent(componentJson, entity);
							m_EntityManager.AddComponentType(entity, ComponentType::SPRITE2D);
							break;
						}
						case ComponentType::COLLIDER2D:
						{
							auto& physicsManager = m_Engine.GetPhysicsManager();
							physicsManager.GetColliderManager().CreateComponent(componentJson, entity);
							m_EntityManager.AddComponentType(entity, ComponentType::COLLIDER2D);
							break; 
						}
						case ComponentType::PYCOMPONENT:
							{
								auto& pythonEngine = m_Engine.GetPythonEngine();
								if (CheckJsonExists(componentJson, "script_path"))
								{
									std::string path = componentJson["script_path"];
									const ModuleId moduleId = pythonEngine.LoadPyModule(path);
									if(moduleId != INVALID_MODULE)
										const InstanceId instanceId = pythonEngine.LoadPyClass(moduleId, entity);
								}
								break;
							}
						default:
							break;
						}
					}
					else
					{
						std::ostringstream oss;
						oss << "[Error] No type specified for component with json content: " << componentJson;
						Log::GetInstance()->Error(oss.str());
					}
				}
			}
			else
			{
				std::ostringstream oss;
				oss << "[Error] No components attached in the JSON entity: " << entity << "with json content: " << entityJson;
				Log::GetInstance()->Error(oss.str());
			}
		}
	}
	else
	{
		std::ostringstream oss;
		oss << "No Entities in " << sceneInfo->name;
		Log::GetInstance()->Error(oss.str());
	}

	//TODO remove previous scene

	m_Engine.Collect();

	auto& editor = m_Engine.GetEditor();
	editor.SetCurrentScene(std::move(sceneInfo));
	
	auto& pythonEngine = m_Engine.GetPythonEngine();
	pythonEngine.InitPyComponent();
	
	
}

std::list<std::string> SceneManager::GetAllScenes()
{
	std::list<std::string> scenes;
	std::for_each(m_ScenePathMap.begin(),m_ScenePathMap.end(), [&](const std::pair<const std::string, std::string>& ref) {
		scenes.push_back(ref.first);
	});
	return scenes;
}


void SceneManager::LoadSceneFromName(const std::string& sceneName)
{
	if (m_ScenePathMap.find(sceneName) != m_ScenePathMap.end())
	{

		sf::Clock loadingClock;
		m_Engine.Clear();
		LoadSceneFromPath(m_ScenePathMap[sceneName]);
		{
			sf::Time loadingTime = loadingClock.getElapsedTime();
			std::ostringstream oss;
			oss << "Scene Loading Time: " << loadingTime.asSeconds();
			Log::GetInstance()->Msg(oss.str());
		}
	}
	else
	{
		std::ostringstream oss;
		oss << "[ERROR] No scene is named: " << sceneName <<"\n";
		oss << "Here are the list of scenes:\n";
		for(auto& sceneNamePair : m_ScenePathMap)
		{
			oss << "- " << sceneNamePair.first << "\n";
		}
		Log::GetInstance()->Error(oss.str());
	}
}



}
