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

#include <map>
#include <pybind11/functional.h>

#include <engine/module.h>
#include <utility/python_utility.h>
#include <engine/component.h>

namespace sfge
{
class PyComponent;
struct ColliderData;

typedef unsigned ModuleId;
#define INVALID_MODULE 0U
typedef unsigned InstanceId;
#define INVALID_INSTANCE 0U

/**
* \brief Manage the python interpreter
*/
class PythonEngine : public Module, public LayerComponentManager<PyComponent*>
{
public:
	PythonEngine(Engine& engine);
	/**
	* \brief Initialize the python interpreter
	*/
	void Init() override;
	/**
	* \brief Update the python interpreter, called only in play mode
	* \param dt The delta time since last frame
	*/
	void Update(sf::Time dt) override;

	/**
	* \brief Finalize the python interpreter
	*/
	void Destroy() override;

	void Collect() override;
	void Clear() override;

	ModuleId LoadPyModule(std::string& moduleFilename);

	InstanceId LoadPyComponent(ModuleId moduleId, Entity entity);

	/**
	 * \brief Get a python component object
	 * \param instanceId InstanceId necessary to get back the PyComponent and update it later
	 * \return pyComponent PyComponent pointer that interacts with the python script
	 */
	PyComponent* GetPyComponent(InstanceId instanceId);

	/*
	*/
	void OnTriggerEnter(Entity entity, ColliderData* colliderData);
	void OnTriggerExit(Entity entity, ColliderData* colliderData);
	void OnCollisionEnter(Entity entity, ColliderData* colliderData);
	void OnCollisionExit(Entity entity, ColliderData* colliderData);
private:
	/**
	 * \brief Load all the python scripts at initialization or reset
	 */
	void LoadScripts(std::string dirname = "scripts/");

	std::map<std::string, ModuleId> m_PythonModuleIdMap;
	std::map<ModuleId, std::string> m_PyComponentClassNameMap;
	std::map<ModuleId, std::string> m_PyModuleNameMap;
	Id m_IncrementalModuleId = 1U;

	std::map<Id, py::object> m_PythonInstanceMap;
	Id m_IncrementalInstanceId = 1U;
	std::vector<PyComponent*> m_PyComponents{};

};

}
#endif /* SFGE_PYENGINE_H */
