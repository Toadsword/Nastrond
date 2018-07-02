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

#include <python/python_engine.h>
#include <engine/modules.h>
#include <engine/log.h>

#include <python/pycomponent.h>

#include <utility/file_utility.h>
#include <utility/time_utility.h>


#include <pybind11/operators.h>
#include <pybind11/stl.h>



namespace sfge
{

PYBIND11_EMBEDDED_MODULE(SFGE, m)
{


	py::class_<Engine> engine(m, "Engine");

	py::class_<Module> module(m, "Module");

	py::class_<SceneManager> sceneManager(m, "SceneManager");
	sceneManager
		.def("load_scene", &SceneManager::LoadScene);

	py::class_<InputManager> inputManager(m, "InputManager");
	inputManager
		.def_property_readonly("keyboard", &InputManager::GetKeyboardManager, py::return_value_policy::reference);

	py::class_<KeyboardManager> keyboardManager(m, "KeyboardManager");
	keyboardManager
		.def("is_key_held", &KeyboardManager::IsKeyHeld)
		.def("is_key_down", &KeyboardManager::IsKeyDown)
		.def("is_key_up", &KeyboardManager::IsKeyUp);

	py::enum_<sf::Keyboard::Key>(keyboardManager, "Key")
		.value("Space", sf::Keyboard::Space)
		.value("Up", sf::Keyboard::Up)
		.value("Down", sf::Keyboard::Down)
		.value("Left", sf::Keyboard::Left)
		.value("Right", sf::Keyboard::Right)
		.export_values();

	py::class_<Scene> scene(m, "Scene");


	py::class_<PyComponent> component(m, "Component");
	component
		.def(py::init<>(), py::return_value_policy::reference)
		.def("init", &PyComponent::Init)
		.def("update", &PyComponent::Update)
		.def_property_readonly("entity", &PyComponent::GetEntity)
		.def("on_trigger_enter", &PyComponent::OnTriggerEnter)
		.def("on_collision_enter", &PyComponent::OnCollisionEnter)
		.def("on_trigger_exit", &PyComponent::OnTriggerExit)
		.def("on_collision_exit", &PyComponent::OnCollisionExit);

	/*py::enum_<ComponentType>(component, "ComponentType")
		.value("PyComponent", ComponentType::PYCOMPONENT)
		.value("Shape", ComponentType::SHAPE)
		.value("Body", ComponentType::BODY2D)
		.value("Sprite", ComponentType::SPRITE)
		.export_values();*/

	py::class_<Transform2d> transform(m, "Transform");
	/*transform
		.def_property("euler_angle", Transform2d::m_EulerAngle)
		.def("set_euler_angle", &Transform::SetEulerAngle)
		.def("get_position", &Transform::GetPosition)
		.def("set_position", &Transform::SetPosition)
		.def("get_scale", &Transform::GetScale)
		.def("set_scale", &Transform::SetScale);*/
	
	py::class_<Collider> collider(m, "Collider");
	collider
		.def("is_trigger", &Collider::IsTrigger);
	
	py::class_<Body2d> body(m, "Body");
	body
		.def_property("velocity", &Body2d::GetVelocity, &Body2d::SetVelocity)
		.def("add_force", &Body2d::AddForce)
		.def_property_readonly("body_type", &Body2d::GetBodyType)
		.def_property_readonly("mass", &Body2d::GetMass);

	py::enum_<b2BodyType>(body, "BodyType")
		.value("STATIC_BODY", b2_staticBody)
		.value("KINEMATIC_BODY", b2_kinematicBody)
		.value("DYNAMIC_BODY", b2_dynamicBody)
		.export_values();

	py::class_<Sound> sound(m, "Sound");
	sound
		.def("play", &Sound::Play)
		.def("stop", &Sound::Stop);
	
	py::class_<Shape> shape(m, "Shape");
	shape
		.def("set_fill_color", &Shape::SetFillColor);
	//Utility
	py::class_<sf::Color> color(m, "Color");
	color
		.def_readwrite("r", &sf::Color::r)
		.def_readwrite("g", &sf::Color::g)
		.def_readwrite("b", &sf::Color::b)
		.def_readwrite("a", &sf::Color::a)
		.def_readonly_static("Black", &sf::Color::Black)
		.def_readonly_static("White", &sf::Color::White)
		.def_readonly_static("Red", &sf::Color::Red)
		.def_readonly_static("Green", &sf::Color::Green)
		.def_readonly_static("Blue", &sf::Color::Blue)
		.def_readonly_static("Yellow", &sf::Color::Yellow)
		.def_readonly_static("Magenta", &sf::Color::Magenta)
		.def_readonly_static("Cyan", &sf::Color::Cyan)
		.def_readonly_static("Transparent", &sf::Color::Transparent);

	py::class_<Timer> timer(m, "Timer");
	timer
		.def(py::init<float, float>())
		.def("update", &Timer::Update)
		.def("reset", &Timer::Reset)
		.def("get_current", &Timer::GetCurrent)
		.def("get_current_time", &Timer::GetCurrentTime)
		.def("is_over", &Timer::IsOver);

	py::class_<sf::Vector2f> vector2f(m, "Vector2f");
	vector2f
		.def(py::init<float, float>())
		.def(py::init<>())
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def(py::self * float())
		.def_readwrite("x", &sf::Vector2f::x)
		.def_readwrite("y", &sf::Vector2f::y);

	py::class_<b2Vec2> b2vec2(m, "b2Vec2");
	b2vec2
		.def(py::init<>())
		.def(py::init<float, float>())
		.def_readwrite("x", &b2Vec2::x)
		.def_readwrite("y", &b2Vec2::y)
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self *= float());
		
}





void PythonManager::Init()
{
	Log::GetInstance()->Msg("Initialise the python embed interpretor");
	py::initialize_interpreter();
	//Adding refecrence to c++ engine modules
	py::module sfgeModule = py::module::import("SFGE");
	sfgeModule.attr("engine")=  py::cast(&m_Engine);
	sfgeModule.attr("scene_manager") = py::cast(m_Engine.GetSceneManager());
	sfgeModule.attr("input_manager") = py::cast(m_Engine.GetInputManager());

}

void PythonManager::Update(sf::Time)
{
}


void PythonManager::Destroy()
{
	pythonInstanceMap.clear();
	pythonModuleObjectMap.clear();
	Log::GetInstance()->Msg("Finalize the python embed interpretor");
	py::finalize_interpreter();
}

void PythonManager::Clear()
{
	pythonInstanceMap.clear();
}
void PythonManager::Collect()
{
}

void PythonManager::LoadScripts(std::string dirname)
{
	using std::placeholders::_1;
	std::function<void(std::string)> checkFunc = std::bind(&PythonManager::CheckEntry, this, _1);
	IterateDirectory(dirname, checkFunc);
}
void PythonManager::CheckEntry(std::string entry)
{
	if(IsRegularFile(entry))
	{
		
	}

	if(IsDirectory(entry))
	{
		LoadScripts(entry);
	}
}
/*
unsigned int PythonManager::LoadPyComponentFile(std::string script_path, GameObject* gameObject)
{
    const auto folderLastIndex = script_path.find_last_of("/");
    std::string filename = script_path.substr(folderLastIndex+1, script_path.size());
	const std::string::size_type filenameExtensionIndex = filename.find_last_of(".");
	std::string module_name = filename.substr(0,filenameExtensionIndex);
	std::string class_name = module2class(module_name);
	if(IsRegularFile(script_path))
	{
		if(pythonModuleIdMap.find(script_path) != pythonModuleIdMap.end())
		{
			const unsigned int scriptId = pythonModuleIdMap[script_path];
			if(scriptId == 0U)
			{
				std::ostringstream oss;
				oss << "Python script: " << script_path << " has id 0";
				Log::GetInstance()->Error(oss.str());
				return 0U;
			}
			{
				{
					std::ostringstream oss;
					oss << "Loaded Python script: " << script_path << " has id: " << scriptId;
					Log::GetInstance()->Msg(oss.str());
				}
				try
				{
					pythonInstanceMap[incrementalInstanceId] = pythonModuleObjectMap[scriptId]
						.attr(class_name.c_str())(gameObject);
					
					incrementalInstanceId++;
					return incrementalInstanceId-1;
				}
				catch(const std::runtime_error& e)
				{
					std::stringstream oss;
					oss << "Python error on script file: "<<script_path<<"\n"<<e.what();
					Log::GetInstance()->Error(oss.str());
				}
			}
		}
		else
		{

			try
			{

				{
					std::ostringstream oss;
					oss << "Loading module: "<<module_name<<" with Component: "<<class_name;
					Log::GetInstance()->Msg(oss.str());
				}
				py::object globals  = py::globals();

				pythonModuleObjectMap[incrementalScriptId] = import(module_name, script_path, globals);
				pythonModuleIdMap[script_path] = incrementalScriptId;
				for (auto& moduleObj : pythonModuleObjectMap)
				{
					moduleObj.second.attr(class_name.c_str()) = pythonModuleObjectMap[incrementalScriptId]
						.attr(class_name.c_str());
				}
				pythonInstanceMap[incrementalInstanceId] = pythonModuleObjectMap[incrementalScriptId].attr(class_name.c_str())(gameObject);


				incrementalScriptId++;
				incrementalInstanceId++;
				return incrementalInstanceId-1;
			}
			catch(const std::runtime_error& e)
			{
				std::stringstream oss;
				oss << "Python error on script file: "<<script_path<<"\n"<<e.what();
				Log::GetInstance()->Error(oss.str());
			}
		}
	}
	else
	{
		std::stringstream oss;
		oss << "Python error on script file: "<<script_path<<" is not a file!\n";
		Log::GetInstance()->Error(oss.str());
	}

	return 0U;
}
*/

PyComponent* PythonManager::GetPyComponent(unsigned int instanceId)
{
	if(pythonInstanceMap.find(instanceId) != pythonInstanceMap.end())
	{
		return pythonInstanceMap[instanceId].cast<PyComponent*>();
	}
	return nullptr;
}



}
