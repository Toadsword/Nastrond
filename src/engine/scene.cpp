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
// for convenience


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

std::shared_ptr<Scene> SceneManager::LoadSceneFromName(std::string sceneName)
{
	{
		std::ostringstream oss;
		oss << "Loading scene from: " << sceneName;
		Log::GetInstance()->Msg(oss.str());
	}
	auto sceneJsonPtr = LoadJson(sceneName);
	
	if(sceneJsonPtr != nullptr)
	{
		return nullptr;//LoadSceneFromJson(*sceneJsonPtr);
	}
	return nullptr;
	
}
/*
std::shared_ptr<Scene> SceneManager::LoadSceneFromJson(json& sceneJson)
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>(this);
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
		
	}
	else
	{
		std::ostringstream oss;
		oss << "No GameObjects in " << scene->name;
		Log::GetInstance()->Error(oss.str());
	}
	//m_Scenes.push_back(scene);
	return scene;
}*/

void SceneManager::SetCurrentScene(std::string sceneName)
{
	auto loadedScene = LoadSceneFromName(sceneName);
	if(loadedScene != nullptr)
	{
		SetCurrentScene(loadedScene);
	}
	else
	{
		std::ostringstream oss;
		oss <<"Error while loading scene: "<<sceneName;
		Log::GetInstance()->Error(oss.str());
	}
}

void SceneManager::SetCurrentScene(std::shared_ptr<Scene> scene)
{
	m_CurrentScene = scene;
}

void SceneManager::Clear()
{
	m_PreviousScene = m_CurrentScene;
}

void SceneManager::Collect()
{
	
	m_PreviousScene = nullptr;
	m_Switching = false;
}

bool SceneManager::IsSwitching()
{
	return m_Switching;
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return m_CurrentScene;
}



void SceneManager::Destroy()
{
	Clear();
	m_CurrentScene = nullptr;
	Collect();
}

void SceneManager::LoadScene(std::string sceneName)
{
	sf::Clock loadingClock;
	m_Engine.Clear();
	SetCurrentScene(LoadSceneFromName(sceneName));
	m_Switching = true;
	{
		sf::Time loadingTime = loadingClock.getElapsedTime();
		std::ostringstream oss;
		oss << "Scene Loading Time: " << loadingTime.asSeconds();
		Log::GetInstance()->Msg(oss.str());
	}
}


Scene::Scene(SceneManager * sceneManager) 
{
	m_SceneManager = sceneManager;
}

void Scene::Update(sf::Time dt)
{
	
}

Scene::~Scene()
{
	
}
}
