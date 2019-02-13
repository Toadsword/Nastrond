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

#ifndef SFGE_PYSYSTEM_H
#define SFGE_PYSYSTEM_H

#include <utility/python_utility.h>

#include <engine/system.h>
#include <engine/component.h>

namespace sfge
{

using InstanceId = unsigned;
using ModuleId = unsigned;
class PySystem : public System
{
public:
	using System::System;
	void Init() override;
	void Update(float dt) override;
	void FixedUpdate() override;
	void Draw() override;

};

class PySystemManager : public System
{
public:

	using System::System;
	void Init() override;

	void Destroy() override;

	InstanceId LoadPySystem(ModuleId moduleId);
	InstanceId LoadCppExtensionSystem(std::string systemClassName);
	PySystem* GetPySystemFromInstanceId(InstanceId instanceId);

	PySystem* GetPySystemFromClassName(std::string className);

	template<class T>
	T* GetPySystem(std::string className) {
		for (auto i = 0u; i < m_PySystemNames.size(); i++)
		{
			if (m_PySystemNames[i] == className)
			{
				return m_PythonInstances[i].cast<T*>();
			}
		}
		return nullptr;
	}

protected:
	std::vector<PySystem*> m_PySystems = std::vector<PySystem*>( INIT_ENTITY_NMB * MULTIPLE_COMPONENTS_MULTIPLIER );
	std::vector<std::string> m_PySystemNames = std::vector<std::string>( INIT_ENTITY_NMB * MULTIPLE_COMPONENTS_MULTIPLIER );
	std::vector<py::object> m_PythonInstances = std::vector<py::object>( INIT_ENTITY_NMB * MULTIPLE_COMPONENTS_MULTIPLIER );
	InstanceId m_IncrementalInstanceId = 1U;

	PythonEngine* m_PythonEngine = nullptr;
};
}
#endif