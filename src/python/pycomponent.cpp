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
#include <physics/physics.h>
#include <graphics/graphics.h>
namespace sfge
{
Component::Component(PythonEngine* pythonEngine, Entity entity)
{
	m_PythonEngine = pythonEngine;
	m_Entity = entity;
}
void PyComponent::Init()
{
	Log::GetInstance()->Msg("Init PyComponent from C++");
	try
	{
        
        py::gil_scoped_release release;
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
        py::gil_scoped_release release;
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
        py::gil_scoped_release release;
		PYBIND11_OVERLOAD_NAME(
			void,
			PyComponent,
			"fixed_update",
			FixedUpdate,
			fixedDeltaTime
		);
	}

	py::object Component::GetComponent(ComponentType componentType) const
	{
		if (m_PythonEngine)
		{
			switch (componentType)
			{
			case ComponentType::TRANSFORM:
				if (auto transformManager = m_PythonEngine->GetEngine().GetTransform2dManager().lock())
				{
					auto& transform = transformManager->GetComponent(m_Entity);
					return py::cast(transform, py::return_value_policy::reference);
				}
				break;
			case ComponentType::BODY2D:
				if(auto physicsManager = m_PythonEngine->GetEngine().GetPhysicsManager().lock())
				{
					auto& body = physicsManager->GetBodyManager().GetComponent(m_Entity);
					return py::cast(body, py::return_value_policy::reference);
				}
				break;
			case ComponentType::COLLIDER:
				break;
			case ComponentType::SHAPE:
				if(auto graphicsManager = m_PythonEngine->GetEngine().GetGraphicsManager().lock())
				{
					auto shape = graphicsManager->GetShapeManager().GetComponent(m_Entity);
					return py::cast(shape, py::return_value_policy::reference);
				}
				break;
			case ComponentType::SPRITE:
				if (auto graphicsManager = m_PythonEngine->GetEngine().GetGraphicsManager().lock())
				{
					auto sprite = graphicsManager->GetSpriteManager().GetComponent(m_Entity);
					return py::cast(sprite, py::return_value_policy::reference);
				}
				break;
			case ComponentType::SOUND:
				break;
			default:
				return py::none();
			}
		}
		return py::none();
	}

	py::object Component::GetPyComponent(py::object type)
	{
		return m_PythonEngine->GetPyComponentFromType(type, m_Entity);
	}

	
void PyComponent::OnCollisionEnter(ColliderData * collider)
	{
	try
	{
        py::gil_scoped_release release;
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
        py::gil_scoped_release release;
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
        py::gil_scoped_release release;
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
        py::gil_scoped_release release;
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
}
