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

// for convenience


namespace sfge
{


void SceneManager::Init()
{
	
}



void SceneManager::Update(sf::Time dt)
{
	
}

void SceneManager::LoadSceneFromPath(const std::string& scenePath)
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
	
}

void SceneManager::LoadSceneFromJson(json& sceneJson, std::unique_ptr<editor::SceneInfo> sceneInfo) const
{
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
		for(auto& componentJson : sceneJson["entities"])
		{
			if(CheckJsonExists(componentJson, "type"))
			{
				const ComponentType componentType = componentJson["type"];
				switch(componentType)
				{
				case TRANSFORM:
					break;
				case SHAPE:
					break;
				case BODY2D:
					break;
				case SPRITE:
					break;
				case COLLIDER:
					break;
				default: 
					break;
				}
			}
		}
	}
	else
	{
		std::ostringstream oss;
		oss << "No Entities in " << sceneInfo->name;
		Log::GetInstance()->Error(oss.str());
	}
	//m_Scenes.push_back(scene);
	
}




void SceneManager::LoadScene(std::string sceneName)
{
	sf::Clock loadingClock;
	m_Engine.Clear();
	LoadSceneFromPath(sceneName);
	{
		sf::Time loadingTime = loadingClock.getElapsedTime();
		std::ostringstream oss;
		oss << "Scene Loading Time: " << loadingTime.asSeconds();
		Log::GetInstance()->Msg(oss.str());
	}
}



}
