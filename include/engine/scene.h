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


#ifndef SFGE_SCENE_H
#define SFGE_SCENE_H

#include <memory>
#include <string>
#include <list>

#include <engine/module.h>
#include <utility/json_utility.h>
#include <engine/transform2d.h>

namespace sfge
{

namespace editor
{
struct SceneInfo;
}

/**
* \brief The Scene Manager do the transition between two scenes, read from the Engine Configuration the scenes build list
*/
class SceneManager : public Module
{
public:
	SceneManager() = default;
	~SceneManager() = default;
	void Init() override;

	void SearchScenes(std::string& dataDirname);
	/**
	* \brief Finalize and delete everything created in the SceneManager
	*/
	void LoadSceneFromName(const std::string& sceneName);
	/**
	* \brief Load a Scene and create all its GameObject
	* \param scenePath the scene path given by the configuration
	* \return the heap Scene that is automatically destroyed when not used
	*/
	void LoadSceneFromPath(const std::string& scenePath) const;
	/**
	* \brief Load a Scene and create all its GameObject
	* \param sceneName the scene path given by the configuration
	* \return the heap Scene that is automatically destroyed when not used
	*/
	void LoadSceneFromJson(json& sceneJson, std::unique_ptr<editor::SceneInfo> sceneInfo = nullptr) const;
	/**
	 * \brief Return a list of all the scenes available in the data folder, pretty useful for python and the editor
	 * \return the list of scenes in the data folder
	 */
	std::list<std::string> GetAllScenes();
private:

	std::weak_ptr<EntityManager> m_EntityManagerPtr;
	std::weak_ptr<Transform2dManager> m_TransformManagerPtr;

	std::map<std::string, std::string> m_ScenePathMap;

};
}

#endif
