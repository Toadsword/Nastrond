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


struct ColliderData;

using ModuleId = unsigned;
const ModuleId INVALID_MODULE = 0U;

using InstanceId = unsigned;
const InstanceId INVALID_INSTANCE = 0U;

/**
* \brief Manage the python interpreter
*/
class PythonEngine :
		public System
{
public:
	using System::System;
	/**
	* \brief Initialize the python interpreter
	*/
	void Init() override;


	void InitScriptsInstances();
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

	ModuleId LoadPyModule(std::string moduleFilename);


	PyComponentManager& GetPyComponentManager(){ return m_PyComponentManager; }
	PySystemManager& GetPySystemManager(){ return m_PySystemManager; }

	void ExecutePythonCommand(std::string pythonCommand);

	const std::string& GetClassNameFrom(ModuleId moduleId);
	const std::string& GetModuleNameFrom(ModuleId moduleId);
	const std::string& GetModulePathFrom(ModuleId moduleId);
	const py::object & GetModuleObjFrom(ModuleId moduleId);

	/*
	*/
    void SpreadClasses();
private:
	/**
	 * \brief Load all the python scripts at initialization or reset
	 */
	void LoadScripts(std::string dirname = "scripts/");


	std::vector<std::string> m_PythonModulePaths = std::vector<std::string>( INIT_ENTITY_NMB * MULTIPLE_COMPONENTS_MULTIPLIER );
	std::vector<std::string> m_PyClassNames = std::vector<std::string>( INIT_ENTITY_NMB * MULTIPLE_COMPONENTS_MULTIPLIER );
	std::vector<std::string> m_PyModuleNames = std::vector<std::string>( INIT_ENTITY_NMB * MULTIPLE_COMPONENTS_MULTIPLIER );
	std::vector<py::object> m_PyModuleObjs = std::vector<py::object>( INIT_ENTITY_NMB * MULTIPLE_COMPONENTS_MULTIPLIER );

	ModuleId m_IncrementalModuleId = 1U;

	PyComponentManager m_PyComponentManager{m_Engine};
	PySystemManager m_PySystemManager{m_Engine};

};

}
#endif /* SFGE_PYENGINE_H */
