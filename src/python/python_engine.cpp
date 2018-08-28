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

#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <python/python_engine.h>
#include <engine/log.h>
#include <engine/scene.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <input/input.h>
#include <audio/audio.h>
#include <graphics/shape2d.h>
#include <graphics/sprite2d.h>
#include <physics/physics2d.h>
#include <python/pycomponent.h>
#include <python/pysystem.h>

#include <utility/file_utility.h>
#include <utility/time_utility.h>


namespace sfge
{

PYBIND11_EMBEDDED_MODULE(SFGE, m)
{


	py::class_<Engine> engine(m, "Engine");
	engine
		.def_property_readonly("config", [](Engine* engine)
	{
		return engine->GetConfig().lock();
	}, py::return_value_policy::reference);

	py::class_<Configuration, std::unique_ptr<Configuration, py::nodelete>> config(m, "Configuration");
	config
		.def_property_readonly("screen_size", [](Configuration* config) {return sf::Vector2f(config->screenResolution.x, config->screenResolution.y); });
	py::class_<System, PySystem> system(m, "System");
	system
		.def(py::init<Engine&>(), py::return_value_policy::reference)
		.def("init", &System::Init)
		.def("update", &System::Update)
		.def("fixed_update", &System::FixedUpdate);

	py::class_<SceneManager> sceneManager(m, "SceneManager");
	sceneManager
		.def("load_scene", &SceneManager::LoadSceneFromName)
		.def("get_scenes", &SceneManager::GetAllScenes);

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

	py::class_<Transform2dManager> transform2dManager(m , "Transform2dManager");
	transform2dManager
	    .def("create_component", &Transform2dManager::CreateEmptyComponent)
	    .def("get_component", &Transform2dManager::GetComponentRef, py::return_value_policy::reference);

	py::class_<EntityManager> entityManager(m, "EntityManager");
	entityManager
	    .def("create_entity", &EntityManager::CreateEntity)
	    .def("has_component", &EntityManager::HasComponent);

	py::class_<Physics2dManager> physics2dManager(m, "Physics2dManager");
	physics2dManager
	    .def_property_readonly ("body_manager", &Physics2dManager::GetBodyManager, py::return_value_policy::reference)
		.def("pixel2meter", [](float v) {return pixel2meter(v); })
		.def("pixel2meter", [](sf::Vector2f v) {return pixel2meter(v); })
		.def("meter2pixel", [](float v) {return meter2pixel(v); })
		.def("meter2pixel", [](b2Vec2 v) {return meter2pixel(v); });

	py::class_<Body2dManager> body2dManager(m, "Body2dManager");
	body2dManager
	    .def("create_component", &Body2dManager::CreateEmptyComponent)
	    .def("get_component", &Body2dManager::GetComponentRef, py::return_value_policy::reference);


	py::class_<PythonEngine> pythonEngine(m, "PythonEngine");

	py::class_<Component, PyComponent> component(m, "Component");
	component
		.def(py::init<Engine&, Entity>(), py::return_value_policy::reference)
		.def("init", &Component::Init)
		.def("update", &Component::Update)
		.def_property_readonly("entity", &Component::GetEntity)
		.def("get_component", &Component::GetComponent)
		.def("get_component", &Component::GetPyComponent)
		.def("on_trigger_enter", &Component::OnTriggerEnter)
		.def("on_collision_enter", &Component::OnCollisionEnter)
		.def("on_trigger_exit", &Component::OnTriggerExit)
		.def("on_collision_exit", &Component::OnCollisionExit)
	;

	py::enum_<ComponentType>(component, "ComponentType")
		.value("PyComponent", ComponentType::PYCOMPONENT)
		.value("Shape", ComponentType::SHAPE2D)
		.value("Body", ComponentType::BODY2D)
		.value("Sprite", ComponentType::SPRITE2D)
		.value("Sound", ComponentType::SOUND)
		.value("Transform2d", ComponentType::TRANSFORM2D)
		.export_values();

	py::class_<Transform2d> transform(m, "Transform2d");
	transform
		.def_readwrite("euler_angle", &Transform2d::EulerAngle)
		.def_readwrite("position", &Transform2d::Position)
		.def_readwrite("scale", &Transform2d::Scale);

	py::class_<ColliderData> colliderData(m, "ColliderData");
	colliderData
		.def_readonly("body", &ColliderData::body)
		.def_readonly("entity", &ColliderData::entity);
	
	py::class_<Body2d> body2d(m, "Body2d");
	body2d
		.def_property("velocity", &Body2d::GetLinearVelocity, &Body2d::SetLinearVelocity)
		.def("apply_force", &Body2d::ApplyForce)
		.def_property_readonly("body_type", &Body2d::GetType)
		.def_property_readonly("mass", &Body2d::GetMass);

	py::class_<b2Body,std::unique_ptr<b2Body, py::nodelete>> body(m, "Body");
	body
		.def_property("velocity", &b2Body::GetLinearVelocity, &b2Body::SetLinearVelocity)
		.def("apply_force", &b2Body::ApplyForce)
		.def_property_readonly("body_type", &b2Body::GetType)
		.def_property_readonly("mass", &b2Body::GetMass);
		
	py::enum_<b2BodyType>(body, "BodyType")
		.value("STATIC_BODY", b2_staticBody)
		.value("KINEMATIC_BODY", b2_kinematicBody)
		.value("DYNAMIC_BODY", b2_dynamicBody)
		.export_values();
		
	py::class_<Sound> sound(m, "Sound");
	sound
		.def("play", &Sound::Play)
		.def("stop", &Sound::Stop);
	
	py::class_<Shape, std::unique_ptr<Shape, py::nodelete>> shape(m, "Shape");
	shape
		.def("set_fill_color", &Shape::SetFillColor);
	py::class_<Sprite, std::unique_ptr<Sprite, py::nodelete>> sprite(m, "Sprite");
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
		.def("is_over", &Timer::IsOver)
		.def("__repr__", [](const Timer &timer)
	{
		std::ostringstream oss;
		oss << "(" << timer.GetTime() << ", " << timer.GetPeriod() << ")";
		return oss.str();
	});

	py::class_<sf::Vector2f> vector2f(m, "Vector2f");
	vector2f
		.def(py::init<float, float>())
		.def(py::init<>())
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def(py::self * float())
		.def(py::self / float())
		.def_property_readonly("magnitude", [](const sf::Vector2f & vec)
		{
			return sqrtf(vec.x*vec.x + vec.y*vec.y);
		})
		.def_readwrite("x", &sf::Vector2f::x)
		.def_readwrite("y", &sf::Vector2f::y)
		.def("__repr__", [](const sf::Vector2f &vec)
		{
			std::ostringstream oss;
			oss << "(" << vec.x << ", " << vec.y << ")";
			return oss.str();
		});

	py::class_<b2Vec2> b2vec2(m, "b2Vec2");
	b2vec2
		.def(py::init<>())
		.def(py::init<float, float>())
		.def_readwrite("x", &b2Vec2::x)
		.def_readwrite("y", &b2Vec2::y)
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self *= float())
		.def_property_readonly("magnitude", [](const b2Vec2 & vec)
		{
			return sqrtf(vec.x*vec.x + vec.y*vec.y);
		})
		.def("__repr__", [](const b2Vec2 &vec)
		{
			std::ostringstream oss;
			oss << "(" << vec.x << ", " << vec.y << ")";
			return oss.str();
		});
		
}

void PythonEngine::Init()
{

	Log::GetInstance()->Msg("Initialise the python embed interpretor");
	System::Init();

	OnResize(INIT_ENTITY_NMB);

	m_Engine.GetEntityManager().AddObserver(this);
	py::initialize_interpreter();
	//Adding reference to c++ engine modules

	py::module sfgeModule = py::module::import("SFGE");
	try
	{
		sfgeModule.attr("engine") = py::cast(m_Engine, py::return_value_policy::reference);
		sfgeModule.attr("scene_manager") = py::cast(m_Engine.GetSceneManager(), py::return_value_policy::reference);
		sfgeModule.attr("input_manager") = py::cast(m_Engine.GetInputManager(), py::return_value_policy::reference);
		sfgeModule.attr("physics2d_manager") = py::cast(m_Engine.GetPhysicsManager(), py::return_value_policy::reference);
		sfgeModule.attr("transform2d_manager") = py::cast(m_Engine.GetTransform2dManager(), py::return_value_policy::reference);
		sfgeModule.attr("entity_manager") = py::cast(m_Engine.GetEntityManager(), py::return_value_policy::reference);
	}
	catch (py::error_already_set& e)
	{
		std::ostringstream oss;
		oss << "[ERROR] Python already set error: " << e.what();
		Log::GetInstance()->Error(oss.str());
	}
	LoadScripts();
}

void PythonEngine::InitPyComponent()
{
	for (auto pyComponent : m_PyComponents)
	{
		if(pyComponent != nullptr)
			pyComponent->Init();
	}
	for (auto pySystem : m_PySystems)
	{
		if (pySystem != nullptr)
			pySystem->Init();
	}
}


void PythonEngine::Update(float dt)
{
	for (auto pyComponent : m_PyComponents)
	{
		if(pyComponent != nullptr)
			pyComponent->Update(dt);
	}
	for (auto pySystem : m_PySystems)
	{
		if (pySystem != nullptr)
			pySystem->Update(dt);
	}
}

void PythonEngine::FixedUpdate()
{
	const auto config = m_Engine.GetConfig().lock();
	for (auto pyComponent : m_PyComponents)
	{
		if (pyComponent != nullptr)
			pyComponent->FixedUpdate(config->fixedDeltaTime);
	}
	for(auto pySystem : m_PySystems)
	{
		if (pySystem != nullptr)
			pySystem->FixedUpdate();
	}
}


void PythonEngine::Destroy()
{
	System::Destroy();
	Log::GetInstance()->Msg("Finalize the python embed interpretor");
	py::finalize_interpreter();
}

void PythonEngine::Clear()
{
	m_PythonInstances.clear();
	m_PyComponents.clear ();
	OnResize(INIT_ENTITY_NMB);
}

ModuleId PythonEngine::LoadPyModule(std::string& moduleFilename)
{
	const auto folderLastIndex = moduleFilename.find_last_of('/');
	std::string filename = moduleFilename.substr(folderLastIndex + 1, moduleFilename.size());
	const std::string::size_type filenameExtensionIndex = filename.find_last_of('.');
	if(filenameExtensionIndex > moduleFilename.size())
	{
		std::ostringstream oss;
		oss << "Python script: " << moduleFilename << " has not a correct extension";
		Log::GetInstance()->Error(oss.str());
		return INVALID_MODULE;
	}
	std::string moduleName = filename.substr(0, filenameExtensionIndex);
	const std::string extension = filename.substr(filenameExtensionIndex);
	const std::string className = module2class(moduleName);
	if (IsRegularFile(moduleFilename) && extension == ".py")
	{
		ModuleId moduleId = INVALID_MODULE;
		for(ModuleId testedModuleId = 1U; testedModuleId < m_IncrementalModuleId; testedModuleId++)
		{
			if(m_PythonModulePaths[testedModuleId-1] == moduleFilename)
			{
				moduleId = testedModuleId;
			}
		}
		if (moduleId != INVALID_MODULE)
		{
			return moduleId;
		}
		else
		{
			try
			{
				moduleId = m_IncrementalModuleId;
                py::dict globals = py::globals ();
				m_PyModuleObjs[moduleId] = import(moduleName, moduleFilename, globals);
				m_PyModuleNames[moduleId] = moduleName;
				m_PyClassNames[moduleId] = className;

				m_IncrementalModuleId++;
				/*{
					std::ostringstream oss;
					oss << "Loading module: " << moduleName << " with class: " << className << ",\n";
					Log::GetInstance()->Msg(oss.str());
				}*/
			}
			catch (const std::runtime_error& e)
			{
				std::stringstream oss;
				oss << "[PYTHON ERROR] on script file: " << moduleFilename << "\n" << e.what();
				Log::GetInstance()->Error(oss.str());
				return INVALID_MODULE;
			}
			return moduleId;
		}
	}
	return INVALID_MODULE;
}


InstanceId PythonEngine::LoadPyComponent(ModuleId moduleId, Entity entity)
{
	std::string className = m_PyClassNames[moduleId];
	try
	{
		auto globals = py::globals ();
		auto moduleName = m_PyModuleNames[moduleId];
		auto moduleObj = py::module(m_PyModuleObjs[moduleId]);
		

		const auto pyInstanceId = m_IncrementalInstanceId;
		
		//Load PyComponent
		m_PythonInstances[pyInstanceId] =
			moduleObj.attr(className.c_str())(m_Engine, entity);

		const auto pyComponent = GetPyComponentFromInstanceId(pyInstanceId);
		if (pyComponent != nullptr)
		{
			m_PyComponents.push_back(pyComponent);
			auto pyInfo = editor::PyComponentInfo();
			pyInfo.name = className;
			pyInfo.path = m_PythonModulePaths[moduleId];
			pyInfo.pyComponent = pyComponent;
			m_PyComponentsInfo.push_back(pyInfo);
			m_Engine.GetEntityManager().AddComponentType(entity, ComponentType::PYCOMPONENT);
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
	catch(std::runtime_error& e)
	{
		std::stringstream oss;
		oss << "[PYTHON ERROR] trying to instantiate class: " << className << "\n" << e.what()<<"\n"<<py::str(py::globals ());
		Log::GetInstance()->Error(oss.str());
	}
	
	return INVALID_INSTANCE;
	
}

InstanceId PythonEngine::LoadPySystem(ModuleId moduleId)
{
	std::string className = m_PyClassNames[moduleId];
	try
	{
		auto globals = py::globals();
		auto moduleName = m_PyModuleNames[moduleId];
		auto moduleObj = py::module(m_PyModuleObjs[moduleId]);

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

std::list<editor::PyComponentInfo> PythonEngine::GetPyComponentsInfoFromEntity(Entity entity)
{
	std::list<editor::PyComponentInfo> pyComponentInfos;
	for(auto& pyInfo : m_PyComponentsInfo)
	{
		if(pyInfo.pyComponent != nullptr && pyInfo.pyComponent->GetEntity() == entity)
		{
			pyComponentInfos.push_back(pyInfo);
		}
	}
	return pyComponentInfos;
}

void PythonEngine::Collect()
{
}

void PythonEngine::LoadScripts(std::string dirname)
{
	std::function<void(std::string)> LoadAllPyModules;
 	LoadAllPyModules = [&LoadAllPyModules, this](std::string entry)
	{
		if (IsRegularFile(entry))
		{
			
			if(LoadPyModule(entry))
			{
				std::ostringstream oss;
				oss << "Loading script: " << entry << "\n";
				Log::GetInstance()->Msg(oss.str());
			}
		}

		if (IsDirectory(entry))
		{
			IterateDirectory(entry, LoadAllPyModules);
		}
	};
	IterateDirectory(dirname, LoadAllPyModules);
	//Spread the class name in all the scripts
	for(ModuleId moduleId = 1U; moduleId < m_IncrementalModuleId; moduleId++)
	{
		for (ModuleId otherModuleId = 1U; otherModuleId < m_IncrementalModuleId; otherModuleId++)
		{
			if(moduleId == otherModuleId) continue;
			try
			{
			m_PyModuleObjs[moduleId].attr(py::str(m_PyClassNames[otherModuleId])) = 
				m_PyModuleObjs[otherModuleId].attr(py::str(m_PyClassNames[otherModuleId]));
			}
			catch (std::runtime_error& e)
			{
				std::ostringstream oss;
				oss << "[PYTHON ERROR] Could not import class: " << m_PyClassNames[otherModuleId] << " into module: " << moduleId << " with error: " << e.what();
				Log::GetInstance()->Error(oss.str());
			}
		}
		
	}
}


PyComponent* PythonEngine::GetPyComponentFromInstanceId(InstanceId instanceId)
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

PySystem* PythonEngine::GetPySystemFromInstanceId(InstanceId instanceId)
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

py::object PythonEngine::GetPyComponentFromType(py::object type, Entity entity)
{
	for (PyComponent* pyComponent : m_PyComponents)
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

void PythonEngine::OnResize(size_t new_size)
{
	m_PythonModulePaths.resize(new_size * 4 );
	m_PyClassNames.resize(new_size * 4 );
	m_PyModuleNames.resize(new_size * 4);
	m_PyModuleObjs.resize(new_size * 4);

	

	m_PythonInstances.resize(new_size * 4);
	
	m_PyComponents.resize(new_size * 4);
	m_PySystems.resize(new_size * 4);
	m_PyComponentsInfo.resize(new_size * 4);
}

void PythonEngine::OnTriggerEnter(Entity entity, ColliderData * colliderData)
{
	for (auto& pyComponent : m_PyComponents)
	{
		if (pyComponent != nullptr and pyComponent->GetEntity() == entity)
		{
			pyComponent->OnTriggerEnter(colliderData);
		}
	}
}

void PythonEngine::OnTriggerExit(Entity entity, ColliderData * colliderData)
{
	for (auto& pyComponent : m_PyComponents)
	{
		if (pyComponent != nullptr and pyComponent->GetEntity() == entity)
		{
			pyComponent->OnTriggerExit(colliderData);
		}
	}
}

void PythonEngine::OnCollisionEnter(Entity entity, ColliderData * colliderData)
{
	for (auto& pyComponent : m_PyComponents)
	{
		if (pyComponent != nullptr and pyComponent->GetEntity() == entity)
		{
			pyComponent->OnCollisionEnter(colliderData);
		}
	}
}

void PythonEngine::OnCollisionExit(Entity entity, ColliderData * colliderData)
{
	for (auto& pyComponent : m_PyComponents)
	{
		if (pyComponent != nullptr and pyComponent->GetEntity() == entity)
		{
			pyComponent->OnCollisionExit(colliderData);
		}
	}
}



}
