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
#include <engine/log.h>
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
				auto& sound = soundManager->GetComponentRef(m_Entity);
				return py::cast(sound, py::return_value_policy::reference);
			}
		default:
			return py::none();
		}
		return py::none();
	}

	py::object Component::GetPyComponent(py::object type)
	{
		return m_Engine.GetPythonEngine()->GetPyComponentFromType(type, m_Entity);
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

void editor::PyComponentInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("PyComponent");

	ImGui::LabelText("Name", name.c_str());
	ImGui::LabelText("Path", path.c_str());
	if (pyComponent != nullptr)
	{
		//TODO check all variables from the cpp
		std::ostringstream oss;
		const auto pyCompObj = py::cast(pyComponent);
		py::dict pyCompAttrDict = pyCompObj.attr("__dict__");
		for(auto& elem : pyCompAttrDict)
		{
			std::string key = py::str(elem.first);
			std::string value = py::str(elem.second);
			ImGui::LabelText(key.c_str(), value.c_str());
		}
		Log::GetInstance()->Msg(oss.str());
	}
}
}
