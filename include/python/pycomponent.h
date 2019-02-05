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

#ifndef SFGE_PYCOMPONENT_H_
#define SFGE_PYCOMPONENT_H_


#include <memory>

#include <engine/entity.h>
#include <utility/python_utility.h>
//STL
#include <engine/component.h>

namespace sfge
{
class PyBehavior;

using InstanceId = unsigned;
using ModuleId = unsigned;
namespace editor
{
struct PyComponentInfo : public ComponentInfo, public PathEditorComponent
{
  void DrawOnInspector() override;

  PyBehavior* pyComponent = nullptr;
};
}
struct ColliderData;

class Behavior : public LayerComponent
{
public:
    Behavior(Engine& engine, Entity entity);
    virtual ~Behavior() = default;
    
    virtual void Init() {};
    virtual void Update(float dt) {(void) dt;};
    virtual void FixedUpdate(float fixedDeltaTime) {(void)fixedDeltaTime;};
    
    virtual void OnCollisionEnter(ColliderData* collider) {(void) collider;};
    virtual void OnTriggerEnter(ColliderData * collider) { (void) collider;};
    virtual  void OnCollisionExit(ColliderData* collider) {(void) collider;};
    virtual void OnTriggerExit(ColliderData * collider) {(void) collider;};
    
    
    py::object GetComponent(ComponentType componentType) const;
	py::object GetPyComponent(py::object type);


    Entity GetEntity();
    
    unsigned int GetInstanceId() const;
    void SetInstanceId(unsigned int instanceId = 0U);
protected:
    unsigned int m_InstanceId = 0U;
    Entity m_Entity = 0U;
	Engine& m_Engine;
};
    
	/**
 * \brief Python abstraction of Component
 */
class PyBehavior : public Behavior
{
public:
    using Behavior::Behavior;
    void Init() override;
    void Update(float dt)  override;
    void FixedUpdate(float fixedDeltaTime) override;
    
    void OnCollisionEnter(ColliderData* collider)  override;
    void OnTriggerEnter(ColliderData * collider) override;
    void OnCollisionExit(ColliderData* collider) override;
    void OnTriggerExit(ColliderData * collider) override;
};

class PyComponentManager: public MultipleComponentManager<PyBehavior*, editor::PyComponentInfo, ComponentType::PYCOMPONENT>
{
public:

	using MultipleComponentManager::MultipleComponentManager;
	~PyComponentManager(){};
	void Init() override;
	void Update(float dt) override;
	void FixedUpdate() override;

	void CreateComponent(json& componentJson, Entity entity) override;
	virtual PyBehavior** AddComponent(Entity entity) override;
	virtual void DestroyComponent(Entity entity) override;
	virtual PyBehavior** GetComponentPtr(Entity entity) override;

	void OnTriggerEnter(Entity entity, ColliderData* colliderData);
	void OnTriggerExit(Entity entity, ColliderData* colliderData);
	void OnCollisionEnter(Entity entity, ColliderData* colliderData);
	void OnCollisionExit(Entity entity, ColliderData* colliderData);

	void OnResize(size_t newSize) override;

	void RemovePyComponentsFrom(Entity entity);

	void Destroy() override;
	/**
	 * \brief Get a python component object
	 * \param instanceId InstanceId necessary to get back the PyComponent and update it later
	 * \return pyComponent PyComponent pointer that interacts with the python script
	 */
	PyBehavior* GetPyComponentFromInstanceId(InstanceId instanceId);

	py::object GetPyComponentFromType(py::object type, Entity entity);

	InstanceId LoadPyComponent(ModuleId moduleId, Entity entity);

	void InitPyComponents();
protected:
	int GetFreeComponentIndex() override;
	std::vector<py::object> m_PythonInstances = std::vector<py::object>( INIT_ENTITY_NMB * MULTIPLE_COMPONENTS_MULTIPLIER );

	std::list<editor::PyComponentInfo> GetPyComponentsInfoFromEntity(Entity entity);

	InstanceId m_IncrementalInstanceId = 1U;

	PythonEngine* m_PythonEngine = nullptr;
};


}




#endif 
