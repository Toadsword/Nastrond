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


#include <python/pycomponent.h>
#include <python/python_engine.h>
#include <engine/log.h>
#include <physics/collider.h>
#include <sstream>
namespace sfge
{
PyComponent::PyComponent()
{
}
void PyComponent::Init()
{
	Log::GetInstance()->Msg("Init PyComponent from C++");
	try
	{
		PYBIND11_OVERLOAD_PURE_NAME(
			void,
			PyComponent,
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
	/*{
		std::ostringstream oss;
		oss << "Update PyComponent with dt:  " << dt;
		Log::GetInstance()->Msg(oss.str());
	}*/
	try
	{
		PYBIND11_OVERLOAD_PURE_NAME(
			void,
			PyComponent,
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



void PyComponent::OnCollisionEnter(Collider * collider)
{
	try
	{
		PYBIND11_OVERLOAD_PURE_NAME(
			void,
			PyComponent,
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
void PyComponent::OnTriggerEnter(Collider * collider)
{
	try
	{
		PYBIND11_OVERLOAD_PURE_NAME(
			void,
			PyComponent,
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
void PyComponent::OnCollisionExit(Collider * collider)
{
	try
	{
		PYBIND11_OVERLOAD_PURE_NAME(
			void,
			PyComponent,
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
void PyComponent::OnTriggerExit(Collider * collider)
{
	try
	{
		PYBIND11_OVERLOAD_PURE_NAME(
			void,
			PyComponent,
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
Entity PyComponent::GetEntity()
{
	return Entity();
}
unsigned int PyComponent::GetInstanceId() const
{
	return instanceId;
}

void PyComponent::SetInstanceId(unsigned int instanceId)
{
	this->instanceId = instanceId;
}
}
