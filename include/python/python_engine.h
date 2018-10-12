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

#ifndef SFGE_PYENGINE_H
#define SFGE_PYENGINE_H

#include <list>
#include <list>

#include <engine/system.h>
#include <utility/python_utility.h>
#include <engine/component.h>
#include <python/pycomponent.h>
#include <python/pysystem.h>

namespace sfge
{
namespace editor
{
struct PyComponentInfo;
}
class PyComponent;
struct ColliderData;

using ModuleId = unsigned;
const ModuleId INVALID_MODULE = 0U;
using InstanceId = unsigned;
const InstanceId INVALID_INSTANCE = 0U;

/**
* \brief Manage the python interpreter
*/
class PythonEngine : public System, public LayerComponentManager<PyComponent*>, public ResizeObserver
{
public:
	using System::System;
	/**
	* \brief Initialize the python interpreter
	*/
	void Init() override;


	void InitPyComponent();
	/**
	* \brief Update the python interpreter, called only in play mode
	* \param dt The delta time since last frame
	*/
	void Update(float dt) override;
	void FixedUpdate() override;
	void Draw() override;
	/**
	* \brief Finalize the python interpreter
	*/
	void Destroy() override;

	void Collect() override;
	void Clear() override;

	ModuleId LoadPyModule(std::string& moduleFilename);

	InstanceId LoadPyComponent(ModuleId moduleId, Entity entity);
	InstanceId LoadPySystem(ModuleId moduleId);

	void LoadCppExtensionSystem(std::string moduleName);

	std::list<editor::PyComponentInfo> GetPyComponentsInfoFromEntity(Entity entity);
	/**
	 * \brief Get a python component object
	 * \param instanceId InstanceId necessary to get back the PyComponent and update it later
	 * \return pyComponent PyComponent pointer that interacts with the python script
	 */
	PyComponent* GetPyComponentFromInstanceId(InstanceId instanceId);
	PySystem* GetPySystemFromInstanceId(InstanceId instanceId);
	py::object GetPyComponentFromType(py::object type, Entity entity);

	void OnResize(size_t new_size) override;
	/*
	*/
	void OnTriggerEnter(Entity entity, ColliderData* colliderData);
	void OnTriggerExit(Entity entity, ColliderData* colliderData);
	void OnCollisionEnter(Entity entity, ColliderData* colliderData);
	void OnCollisionExit(Entity entity, ColliderData* colliderData);


    void SpreadClasses();
private:
	/**
	 * \brief Load all the python scripts at initialization or reset
	 */
	void LoadScripts(std::string dirname = "scripts/");


	std::vector<std::string> m_PythonModulePaths{ INIT_ENTITY_NMB * 4 };
	std::vector<std::string> m_PyClassNames{ INIT_ENTITY_NMB * 4 };
	std::vector<std::string> m_PyModuleNames{ INIT_ENTITY_NMB * 4 };
	std::vector<py::object> m_PyModuleObjs{INIT_ENTITY_NMB*4};

	ModuleId m_IncrementalModuleId = 1U;

	std::vector<py::object> m_PythonInstances{ INIT_ENTITY_NMB * 4 };
	InstanceId m_IncrementalInstanceId = 1U;
	std::vector<PyComponent*> m_PyComponents{ INIT_ENTITY_NMB * 4 };
	std::vector<PySystem*> m_PySystems{ INIT_ENTITY_NMB * 4 };
	std::vector<editor::PyComponentInfo> m_PyComponentsInfo{ INIT_ENTITY_NMB * 4 };
};

}
#endif /* SFGE_PYENGINE_H */
