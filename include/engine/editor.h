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

#ifndef SFGE_EDITOR_H
#define SFGE_EDITOR_H

#include <list>

#include <engine/module.h>
#include <engine/entity.h>
#include <physics/body2d.h>

namespace sfge
{

    class Shape;
    

//Editor components
namespace editor
{
    
struct EntityInfo;
struct ComponentInfo;
    
class IComponentInspector
{
public:
    virtual void DrawOnEditor() = 0;
};
struct NamableEditorComponent
{
	std::string name = "";
};

struct PathEditorComponent
{
	std::string path = "";
};


struct SceneInfo : NamableEditorComponent, PathEditorComponent
{
};


}

class Editor : public Module
{
public:
	Editor(Engine& engine);
	/**
	* \brief Initialize the SceneManager, get the Configuration from Engine and save the Scene lists from it
	*/
	void Init() override;
	/**
	* \brief Update the SceneManager, mostly updating the GameObjects of the current Scene and doing the transition when needed
	* \param dt Delta time since last frame
	*/
	void Update(sf::Time dt) override;
	void ProcessEvent(sf::Event& event) const;
	void Draw();
	/**
	* \brief Finalize and delete everything created in the SceneManager
	*/
	void Destroy() override;

	void Collect() override;
	void Clear() override;

	void SetCurrentScene(std::unique_ptr<editor::SceneInfo> sceneInfo);
protected:
	std::weak_ptr<sf::RenderWindow> m_Window;
	std::weak_ptr<GraphicsManager> m_GraphicsManager;
	std::weak_ptr<SceneManager> m_SceneManager;
	std::weak_ptr<EntityManager> m_EntityManager;
	std::weak_ptr<Transform2dManager> m_TransformManager;
	std::weak_ptr<Body2dManager> m_BodyManager;

	std::unique_ptr<editor::SceneInfo> m_CurrentScene = nullptr;
	bool m_IsImguiInit = false;
};

}

#endif
