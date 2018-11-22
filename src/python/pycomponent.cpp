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

#include <sstream>

#include <python/pycomponent.h>
#include <python/python_engine.h>
#include <utility/log.h>
#include <physics/physics2d.h>
#include <graphics/graphics2d.h>
#include <engine/engine.h>
#include <audio/audio.h>
#include <utility/python_utility.h>
#include <imgui.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace sfge
{


void editor::PyComponentInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("PyComponent");


	ImGui::LabelText("Name", "%s", name.c_str());
	ImGui::LabelText("Path", "%s", path.c_str());
	if (pyComponent != nullptr)
	{
		//TODO check all variables from the cpp
		std::ostringstream oss;
		const auto pyCompObj = py::cast(pyComponent);
		py::dict pyCompAttrDict = pyCompObj.attr("__dict__");
		for(auto& elem : pyCompAttrDict)
		{
			std::string key = py::str(elem.first);
			std::string value = py::str(elem.second).cast<std::string>().c_str();

			ImGui::LabelText("%s", key.c_str(), "%s",  value.c_str());
		}
		Log::GetInstance()->Msg(oss.str());
	}
}

Component::Component(Engine& engine, Entity entity) :
	m_Engine(engine)
{
	m_Entity = entity;
}
void PyComponent::Init()
{
	//Log::GetInstance()->Msg("Init PyComponent from C++");
	try
	{
        
        //py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			Component,
			"init",
			Init,

		);
	}
	catch(std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PyComponent Init\n"<<e.what();
		Log::GetInstance()->Error(oss.str());
	}
}

void PyComponent::Update(float dt)
{
	
	try
	{
        //py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			Component,
			"update",
			Update,
			dt
		);
	}
	catch(std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PyComponent Update\n"<<e.what();
		Log::GetInstance()->Error(oss.str());
	}
}

	void PyComponent::FixedUpdate(float fixedDeltaTime)
	{
		try
		{

			//py::gil_scoped_release release;
			PYBIND11_OVERLOAD_NAME(
				void,
				Component,
				"fixed_update",
				FixedUpdate,
				fixedDeltaTime
			);
		}
		catch (std::runtime_error& e)
		{
			std::stringstream oss;
			oss << "Python error on PyComponent FixedUpdate\n" << e.what();
			Log::GetInstance()->Error(oss.str());
		}
	}

	py::object Component::GetComponent(ComponentType componentType) const
	{
		
		switch (componentType)
		{
		case ComponentType::TRANSFORM2D:
			
			{
				auto* transformManager = m_Engine.GetTransform2dManager();
				auto& transform = transformManager->GetComponentRef(m_Entity);
				return py::cast(transform, py::return_value_policy::reference);
			}
		case ComponentType::BODY2D:
			{
				auto* physicsManager = m_Engine.GetPhysicsManager();
				auto& body = physicsManager->GetBodyManager()->GetComponentRef(m_Entity);
				return py::cast(body, py::return_value_policy::reference);
			}
		case ComponentType::COLLIDER2D:
			break;
		case ComponentType::SHAPE2D:
			{
				auto* graphicsManager = m_Engine.GetGraphics2dManager();
				const auto shape = graphicsManager->GetShapeManager()->GetComponentPtr (m_Entity);
				return py::cast(shape, py::return_value_policy::reference);
			}
		case ComponentType::SPRITE2D:
			{
				auto* graphicsManager = m_Engine.GetGraphics2dManager();
				auto& sprite = graphicsManager->GetSpriteManager()->GetComponentRef(m_Entity);
				return py::cast(sprite, py::return_value_policy::reference);
			}
		case ComponentType::SOUND:
			{
				auto* soundManager = m_Engine.GetAudioManager()->GetSoundManager();
				auto* sound = soundManager->GetComponentPtr(m_Entity);
				return py::cast(sound, py::return_value_policy::reference);
			}
		default:
			return py::none();
		}
		return py::none();
	}

	py::object Component::GetPyComponent(py::object type)
	{
		return m_Engine.GetPythonEngine()->GetPyComponentManager().GetPyComponentFromType(type, m_Entity);
	}

	
    void PyComponent::OnCollisionEnter(ColliderData * collider)
	{
	try
	{
        //py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			Component,
			"on_collision_enter",
			OnCollisionEnter,
			collider
			);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PyComponent OnCollisionEnter\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}
void PyComponent::OnTriggerEnter(ColliderData * collider)
{
	try
	{
        //py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			Component,
			"on_trigger_enter",
			OnTriggerEnter,
			collider
		);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PyComponent OnTriggerEnter\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}
void PyComponent::OnCollisionExit(ColliderData * collider)
{
	try
	{
        //py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			Component,
			"on_collision_exit",
			OnCollisionExit,
			collider
		);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PyComponent OnCollisionExit\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}
void PyComponent::OnTriggerExit(ColliderData * collider)
{
	try
	{
        //py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			Component,
			"on_trigger_exit",
			OnTriggerExit,
			collider
		);
	}
	catch (std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "Python error on PyComponent OnTriggerExit\n" << e.what();
		Log::GetInstance()->Error(oss.str());
	}
}



Entity Component::GetEntity()
{
	return m_Entity;
}
unsigned int Component::GetInstanceId() const
{
	return m_InstanceId;
}

void Component::SetInstanceId(unsigned int instanceId)
{
	m_InstanceId = instanceId;
}


InstanceId PyComponentManager::LoadPyComponent(ModuleId moduleId, Entity entity)
{
	std::string className = m_PythonEngine->GetClassNameFrom(moduleId);
	try
	{
		auto globals = py::globals ();
		auto moduleName = m_PythonEngine->GetModuleNameFrom(moduleId);
		auto moduleObj = py::module(m_PythonEngine->GetModuleObjFrom(moduleId));


		const auto pyInstanceId = m_IncrementalInstanceId;

		int index = GetFreeComponentIndex();
		if(index != -1)
		{
			//Load PyComponent
			m_PythonInstances[pyInstanceId] =
				moduleObj.attr(className.c_str())(m_Engine, entity);

			const auto pyComponent = GetPyComponentFromInstanceId(pyInstanceId);
			if (pyComponent != nullptr)
			{

				m_Components[index] = pyComponent;
				auto& pyInfo = m_ComponentsInfo[index];
				pyInfo.name = className;
				pyInfo.path = m_PythonEngine->GetModulePathFrom(moduleId);
				pyInfo.pyComponent = pyComponent;
				pyInfo.SetEntity(entity);
				m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::PYCOMPONENT);
			}
			else
			{
				std::ostringstream oss;
				oss << "[Python Error] Could not load the PyComponent* out of the instance";
				Log::GetInstance()->Error(oss.str());
				return INVALID_INSTANCE;
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "[Python Error] Could not load the PyComponent* no more slot available";
			Log::GetInstance()->Error(oss.str());
			return INVALID_INSTANCE;
		}

		m_IncrementalInstanceId++;
		return pyInstanceId;
	}
	catch(std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "[PYTHON ERROR] trying to instantiate class: " << className << "\n" << e.what()<<"\n"<<py::str(py::globals ());
		Log::GetInstance()->Error(oss.str());
	}

	return INVALID_INSTANCE;

}


std::list<editor::PyComponentInfo> PyComponentManager::GetPyComponentsInfoFromEntity(Entity entity)
{
	std::list<editor::PyComponentInfo> pyComponentInfos;
	for(auto& pyInfo : m_ComponentsInfo)
	{
		if(pyInfo.pyComponent != nullptr && pyInfo.pyComponent->GetEntity() == entity)
		{
			pyComponentInfos.push_back(pyInfo);
		}
	}
	return pyComponentInfos;
}



PyComponent* PyComponentManager::GetPyComponentFromInstanceId(InstanceId instanceId)
{
	if (instanceId > m_IncrementalInstanceId)
	{
		std::ostringstream oss;
		oss << "[Python Error] Could not find instance Id: " << instanceId << " in the pythonInstanceMap";
		Log::GetInstance()->Error(oss.str());

		return nullptr;
	}
	return m_PythonInstances[instanceId].cast<PyComponent*>();
}


py::object PyComponentManager::GetPyComponentFromType(py::object type, Entity entity)
{
	for (PyComponent* pyComponent : m_Components)
	{
		if (pyComponent != nullptr and
			pyComponent->GetEntity() == entity and
			py::cast(pyComponent).get_type().is(type))
		{
			return py::cast(pyComponent);
		}
	}
	return py::none();
}

void PyComponentManager::OnTriggerEnter(Entity entity, ColliderData * colliderData)
{
	for (auto& pyComponent : m_Components)
	{
		if (pyComponent != nullptr and pyComponent->GetEntity() == entity)
		{
			pyComponent->OnTriggerEnter(colliderData);
		}
	}
}

void PyComponentManager::OnTriggerExit(Entity entity, ColliderData * colliderData)
{
	for (auto& pyComponent : m_Components)
	{
		if (pyComponent != nullptr and pyComponent->GetEntity() == entity)
		{
			pyComponent->OnTriggerExit(colliderData);
		}
	}
}

void PyComponentManager::OnCollisionEnter(Entity entity, ColliderData * colliderData)
{
	for (auto& pyComponent : m_Components)
	{
		if (pyComponent != nullptr and pyComponent->GetEntity() == entity)
		{
			pyComponent->OnCollisionEnter(colliderData);
		}
	}
}

void PyComponentManager::OnCollisionExit(Entity entity, ColliderData * colliderData)
{
	for (auto& pyComponent : m_Components)
	{
		if (pyComponent != nullptr and pyComponent->GetEntity() == entity)
		{
			pyComponent->OnCollisionExit(colliderData);
		}
	}
}


void PyComponentManager::RemovePyComponentsFrom(Entity entity)
{
	for(int i = 0; i < m_Components.size();i++)
	{
		auto& pyComponent = m_Components[i];
		if(pyComponent != nullptr && entity == pyComponent->GetEntity())
		{
			pyComponent = nullptr;
		}
	}
	for(int i = 0; i < m_PythonInstances.size();i++)
	{
		auto& pyInstance = m_PythonInstances[i];
		if(pyInstance.is_none() || pyInstance.ptr() == nullptr ) continue;
		try
		{
			if(pyInstance.cast<PyComponent*>() != nullptr && pyInstance.cast<PyComponent*>()->GetEntity() == entity)
			{
				m_PythonInstances.erase(m_PythonInstances.begin()+i);
				i--;
			}
		}
		catch (py::cast_error& e)
		{

		}
	}
}
int PyComponentManager::GetFreeComponentIndex()
{
	for(int i = 0; i < m_Components.size();i++)
	{
		if (m_Components[i] == nullptr)
		{
			return i;
		}
	}
	return -1;
}
void PyComponentManager::Init()
{
	MultipleComponentManager::Init();
	m_PythonEngine = m_Engine.GetPythonEngine();
}

void PyComponentManager::CreateComponent(json &componentJson, Entity entity)
{
	if (CheckJsonExists(componentJson, "script_path"))
	{
		std::string path = componentJson["script_path"];
		const ModuleId moduleId = m_PythonEngine->LoadPyModule(path);
		auto moduleName = m_PythonEngine->GetModuleNameFrom(moduleId);
		if(moduleId != INVALID_MODULE)
			const InstanceId instanceId = LoadPyComponent(moduleId, entity);
	}
}

void PyComponentManager::DestroyComponent(Entity entity)
{

}

PyComponent **PyComponentManager::AddComponent(Entity entity)
{
	return nullptr;
}

PyComponent **PyComponentManager::GetComponentPtr(Entity entity) {
	return nullptr;
}
void PyComponentManager::InitPyComponents()
{
	for (auto* pyComponent : m_Components)
	{
		if(pyComponent != nullptr)
			pyComponent->Init();
	}

}
void PyComponentManager::Destroy()
{
	System::Destroy();
	m_Components.clear();
	m_ComponentsInfo.clear();
	m_PythonInstances.clear();
}
void PyComponentManager::FixedUpdate()
{
	System::FixedUpdate();
	auto config = m_Engine.GetConfig();
	for(auto* pyComponent:m_Components)
	{
		if(pyComponent != nullptr)
		{
			pyComponent->FixedUpdate(config->fixedDeltaTime);
		}
	}
}
void PyComponentManager::Update(float dt)
{
	System::Update(dt);
	for(auto* pyComponent:m_Components)
	{
		if(pyComponent != nullptr)
		{
			pyComponent->Update(dt);
		}
	}
}
void PyComponentManager::OnResize(size_t newSize)
{
	MultipleComponentManager::OnResize(newSize);
	m_PythonInstances.resize(newSize);
}
}
