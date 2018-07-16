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
	class Collider;

	/**
 * \brief Python abstraction of Component
 */
class PyComponent : LayerComponent
{
public:
	PyComponent(Entity entity);
	~PyComponent() = default;

	void Init();
	void Update(float dt);
	void FixedUpdate(float fixedDeltaTime);
	/*
	void OnCollisionEnter(Collider* collider) const;
	void OnTriggerEnter(Collider * collider);
	void OnCollisionExit(Collider* collider);
	void OnTriggerExit(Collider * collider);
	*/

	py::object GetComponent(ComponentType componentType);

	Entity GetEntity();

	unsigned int GetInstanceId() const;
	void SetInstanceId(unsigned int instanceId = 0U);

private:
	unsigned int m_InstanceId = 0U;
	Entity m_Entity = 0U;
};

}


#endif 
