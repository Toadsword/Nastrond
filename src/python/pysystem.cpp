//
// Created by efarhan on 21.08.18.
//

#include <sstream>

#include <python/pysystem.h>
#include <python/python_engine.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <utility/python_utility.h>

#include <utility/log.h>

namespace sfge
{

void PySystem::Init()
{
	try
	{
		//py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			System,
			"init",
			Init,

			);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PySystem Init\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}

void PySystem::Update(float dt)
{

	try
	{

		//py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			System,
			"update",
			Update,
			dt
			);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PySystem Update\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}

void PySystem::FixedUpdate()
{
	try
	{

		//py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			System,
			"fixed_update",
			FixedUpdate,
			);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PySystem FixedUpdate\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}

void PySystem::Draw()
{
	try
	{

		//py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			System,
			"draw",
			Draw,
			);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PySystem Draw\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}

void PySystemManager::Init()
{
	System::Init();
	m_PythonEngine = m_Engine.GetPythonEngine();
}

InstanceId PySystemManager::LoadPySystem(ModuleId moduleId)
{
	std::string className = m_PythonEngine->GetClassNameFrom(moduleId);
	try
	{
		auto globals = py::globals();
		auto moduleName = m_PythonEngine->GetModuleNameFrom(moduleId);
		auto moduleObj = py::module(m_PythonEngine->GetModuleObjFrom(moduleId));

		const auto pyInstanceId = m_IncrementalInstanceId;
		//Load PySystem
		m_PythonInstances[pyInstanceId] =
			moduleObj.attr(className.c_str())(m_Engine);
		const auto pySystem = GetPySystemFromInstanceId(pyInstanceId);
		if (pySystem != nullptr)
		{
			m_PySystems.push_back(pySystem);

			/* TODO editor info on system
			 *
			 *auto pyInfo = editor::PyComponentInfo();
			pyInfo.name = className;
			pyInfo.path = m_PythonModulePaths[moduleId];
			pyInfo.pyComponent = pySystem;
			m_PyComponentsInfo.push_back(pyInfo);
			*/
		}
		else
		{
			std::ostringstream oss;
			oss << "[Python Error] Could not load the PyComponent* out of the instance";
			Log::GetInstance()->Error(oss.str());
			return INVALID_INSTANCE;
		}

		m_IncrementalInstanceId++;
		return pyInstanceId;
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "[PYTHON ERROR] trying to instantiate class: " << className << "\n" << e.what() << "\n" << py::str(py::globals());
		Log::GetInstance()->Error(oss.str());
	}

	return INVALID_INSTANCE;
}


void PySystemManager::LoadCppExtensionSystem(std::string systemClassName)
{
	const auto pyInstanceId = m_IncrementalInstanceId;
	try
	{
		py::module sfge = py::module::import("SFGE");
		m_PythonInstances[pyInstanceId] = sfge.attr(systemClassName.c_str())(m_Engine);
		const auto pySystem = GetPySystemFromInstanceId(pyInstanceId);
		if (pySystem != nullptr)
		{
			m_PySystems.push_back(pySystem);
		}
		m_IncrementalInstanceId++;
	}
	catch(std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "[PYTHON ERROR] trying to instantiate System from C++: " << systemClassName << "\n" << e.what() << "\n";
		Log::GetInstance()->Error(oss.str());
	}
}


PySystem* PySystemManager::GetPySystemFromInstanceId(InstanceId instanceId)
{
	if (instanceId > m_IncrementalInstanceId)
	{
		std::ostringstream oss;
		oss << "[Python Error] Could not find instance Id: " << instanceId << " in the pythonInstanceMap";
		Log::GetInstance()->Error(oss.str());

		return nullptr;
	}
	return m_PythonInstances[instanceId].cast<PySystem*>();
}
void PySystemManager::InitPySystems()
{
	for (auto pySystem : m_PySystems)
	{
		if (pySystem != nullptr)
			pySystem->Init();
	}
}
void PySystemManager::Update(float dt)
{
	System::Update(dt);


	rmt_ScopedCPUSample(PySystemUpdate,0);

	for (auto pySystem : m_PySystems)
	{
		if (pySystem != nullptr)
			pySystem->Update(dt);
	}
}
void PySystemManager::FixedUpdate()
{
	System::FixedUpdate();

	rmt_ScopedCPUSample(PySystemFixedUpdate,0);

	for (auto pySystem : m_PySystems)
	{
		if (pySystem != nullptr)
			pySystem->FixedUpdate();
	}
}
void PySystemManager::Destroy()
{
	System::Destroy();
	m_PySystems.clear();
	m_PythonInstances.clear();
}
void PySystemManager::Draw()
{
	rmt_ScopedCPUSample(PySystemDraw,0);
	for(auto pySystem : m_PySystems)
	{
		if(pySystem != nullptr)
			pySystem->Draw();
	}
}
}
