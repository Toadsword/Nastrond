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
#include <python/python_engine.h>
#include <utility/python_utility.h>
//STL
#include <engine/component.h>

namespace sfge
{

struct ColliderData;

class Component : LayerComponent
{
public:
    Component(Engine& engine, Entity entity);
    ~Component() = default;
    
    virtual void Init() {};
    virtual void Update(float dt) {};
    virtual void FixedUpdate(float fixedDeltaTime) {};
    
    virtual void OnCollisionEnter(ColliderData* collider) {};
    virtual void OnTriggerEnter(ColliderData * collider) {};
    virtual  void OnCollisionExit(ColliderData* collider) {};
    virtual void OnTriggerExit(ColliderData * collider) {};
    
    
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
class PyComponent : public Component
{
public:
    using Component::Component; 
    void Init() override;
    void Update(float dt)  override;
    void FixedUpdate(float fixedDeltaTime) override;
    
    void OnCollisionEnter(ColliderData* collider)  override;
    void OnTriggerEnter(ColliderData * collider) override;
    void OnCollisionExit(ColliderData* collider) override;
    void OnTriggerExit(ColliderData * collider) override;
};


}




#endif 
