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

#ifndef SFGE_GIZMO_H_
#define SFGE_GIZMO_H_

#include <editor/editor.h>
#include <engine/component.h>
//Externals
#include <SFML/Graphics.hpp>

namespace sfge
{
	class Transform2d;

enum class GizmoType
{
	NONE,
	CIRCLE,
	RECTANGLE,
	LINE,
};

class Gizmo : public Offsetable
{
public:
  	Gizmo();
	Gizmo(Transform2d* transform, sf::Vector2f offset);
  	Gizmo ( Gizmo && ) = default; //move constructor
  	Gizmo ( const Gizmo & ) = delete; //delete copy constructor
    ~Gizmo();
	void Draw(sf::RenderWindow& window) const;
	void Update(float dt, Transform2d* transform) const;

protected:
	std::unique_ptr<Gizmo> m_Gizmo = nullptr;
};

namespace editor
{

struct GizmoInfo : ComponentInfo
{
	void DrawOnInspector() override;
	Gizmo* gizmoPtr;
};

}

class GizmoManager : public MultipleComponentManager<Gizmo, editor::GizmoInfo, ComponentType::GIZMO2D>
{
public:
	using MultipleComponentManager::MultipleComponentManager; 

	GizmoManager(Engine &Engine);
	~GizmoManager();

	void Init() override;
	
	void Update(float dt) override;
	void Clear() override;
	void DrawGizmos(sf::RenderWindow &window);
	void Collect() override;

	Gizmo* AddComponent(Entity entity) override;
	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;

	void OnResize(size_t new_size) override;
	int GetFreeComponentIndex() override;
	Gizmo* GetComponentPtr(Entity entitiy) override;
protected:
	
	Transform2dManager* m_Transform2dManager;
};



}
#endif /* SRC_GRAPHICS_GIZMO_H_ */
