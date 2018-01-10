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

//STL includes
#include <experimental/filesystem>
//SFGE includes
#include <engine/game_object.h>
#include <engine/scene.h>
#include <engine/log.h>

// for convenience
namespace fs = std::experimental::filesystem;


namespace sfge
{


void SceneManager::Init()
{
	
}



void SceneManager::Update(sf::Time dt)
{
	if(m_CurrentScene != nullptr)
	{
		m_CurrentScene->Update(dt);
	}
}

std::shared_ptr<Scene> SceneManager::LoadScene(std::string sceneName)
{
	{
		std::ostringstream oss;
		oss << "Loading scene from: " << sceneName;
		Log::GetInstance()->Msg(oss.str());
	}
	auto sceneJsonPtr = LoadJson(sceneName);
	
	if(sceneJsonPtr != nullptr)
	{
		return LoadScene(*sceneJsonPtr);
	}
	return nullptr;
	
}

std::shared_ptr<Scene> SceneManager::LoadScene(json& sceneJson)
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	if (CheckJsonParameter(sceneJson, "name", json::value_t::string))
	{
		scene->name = sceneJson["name"].get<std::string>();
	}
	else
	{
		scene->name = "NewScene";
	}
	{
		std::ostringstream oss;
		oss << "Loading scene: " << scene->name;
		Log::GetInstance()->Msg(oss.str());
	}
	if (CheckJsonParameter(sceneJson, "game_objects", json::value_t::array))
	{
		for (json gameObjectJson : sceneJson["game_objects"])
		{
			std::shared_ptr<GameObject> gameObject = GameObject::LoadGameObject(gameObjectJson);
			if (gameObject != nullptr)
			{
				scene->m_GameObjects.push_back(gameObject);
			}
		}
	}
	else
	{
		std::ostringstream oss;
		oss << "No GameObjects in " << scene->name;
		Log::GetInstance()->Error(oss.str());
	}
	m_Scenes.push_back(scene);
	return scene;
}

void SceneManager::SetCurrentScene(std::string sceneName)
{
	for (auto scene : m_Scenes)
	{
		if (scene->name == sceneName)
		{
			SetCurrentScene(scene);
			break;
		}
	}
}

void SceneManager::SetCurrentScene(std::shared_ptr<Scene> scene)
{
	m_CurrentScene = scene;
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return m_CurrentScene;
}



void SceneManager::Destroy()
{
}


void Scene::Update(sf::Time dt)
{
	for(auto gameObject : m_GameObjects)
	{
		gameObject->Update(dt);
	}
}

Scene::~Scene()
{
	while(!m_GameObjects.empty())
	{
		m_GameObjects.pop_front();
	}
}
std::list<std::shared_ptr<GameObject>>& Scene::GetGameObjects()
{
	return m_GameObjects;
}
}