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


#ifdef WIN32
#define _USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#endif

//#include <cmath>
#include <sstream>
#include <list>

#include <imgui.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <python/python_engine.h>
#include <utility/log.h>
#include <engine/scene.h>
#include <engine/engine.h>
#include <engine/systems_container.h>
#include <engine/config.h>
#include <input/input.h>
#include <audio/audio.h>
#include <graphics/shape2d.h>
#include <graphics/sprite2d.h>
#include <graphics/texture.h>
#include <graphics/graphics2d.h>
#include <physics/physics2d.h>
#include <python/pycomponent.h>
#include <python/pysystem.h>

#include <SFML/Graphics/Texture.hpp>

#include <utility/file_utility.h>
#include <utility/time_utility.h>
#include <extensions/python_extensions.h>


namespace sfge
{

PYBIND11_EMBEDDED_MODULE(SFGE, m)
{
	py::class_<Engine> engine(m, "tool_engine");
	engine
		.def_property_readonly("config", [](Engine* engine)
	{
		return engine->GetConfig();
	}, py::return_value_policy::reference);

	py::class_<Configuration, std::unique_ptr<Configuration, py::nodelete>> config(m, "Configuration");
	config
		.def_property_readonly("screen_size", [](Configuration* config) {return Vec2f(config->screenResolution.x, config->screenResolution.y); });
	py::class_<System, PySystem> system(m, "System");
	system
		.def(py::init<Engine&>(), py::return_value_policy::reference)
		.def("init", &System::Init)
		.def("update", &System::Update)
		.def("fixed_update", &System::FixedUpdate)
		.def("draw", &System::Draw);

	py::class_<SceneManager> sceneManager(m, "SceneManager");
	sceneManager
		.def(py::init<Engine&>(), py::return_value_policy::reference)
		.def("load_scene", &SceneManager::LoadSceneFromName)
		.def("get_scenes", &SceneManager::GetAllScenes);

	py::class_<CameraManager> cameraManager(m, "CameraManager");
	cameraManager
		.def(py::init<Engine&>(), py::return_value_policy::reference)
		.def("add_component", &CameraManager::AddComponent, py::return_value_policy::reference)
		.def("get_component", &CameraManager::GetComponentRef, py::return_value_policy::reference);

	py::class_<InputManager> inputManager(m, "InputManager");
	inputManager
		.def(py::init<Engine&>(), py::return_value_policy::reference)
		.def_property_readonly("keyboard", &InputManager::GetKeyboardManager, py::return_value_policy::reference)
		.def_property_readonly("mouse", &InputManager::GetMouseManager, py::return_value_policy::reference);

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
		.value("Num0", sf::Keyboard::Num0)
		.export_values();

	py::class_<MouseManager> mouseManager(m, "MouseManager");
	mouseManager
		.def("is_button_down", &MouseManager::IsButtonDown)
		.def("is_button_held", &MouseManager::IsButtonHeld)
		.def("is_button_up", &MouseManager::IsButtonUp)
		.def("get_local_position", &MouseManager::GetLocalPosition)
		.def("get_world_position", &MouseManager::GetWorldPosition);

	//Todo Fix
	//py::enum_<sf::Mouse::Button>(mouseManager, "Button")
	//.value("Middle", sf::Mouse::Middle)
	//.value("Left", sf::Mouse::Left)
	//.value("Right", sf::Mouse::Button::Right)
	//.export_values();


	py::class_<Transform2dManager> transform2dManager(m , "Transform2dManager");
	transform2dManager
	    .def(py::init<Engine&>(), py::return_value_policy::reference)
		.def("add_component", &Transform2dManager::AddComponent, py::return_value_policy::reference)
	    .def("get_component", &Transform2dManager::GetComponentRef, py::return_value_policy::reference);

	py::class_<EntityManager> entityManager(m, "EntityManager");
	entityManager
	    .def(py::init<Engine&>(), py::return_value_policy::reference)
	    .def("create_entity", &EntityManager::CreateEntity)
	    .def("destroy_entity", &EntityManager::DestroyEntity)
	    .def("has_component", &EntityManager::HasComponent)
		.def("resize", &EntityManager::ResizeEntityNmb);

	py::class_<Physics2dManager> physics2dManager(m, "Physics2dManager");
	physics2dManager
	    .def(py::init<Engine&>(), py::return_value_policy::reference)
	    .def_property_readonly ("body2d_manager", &Physics2dManager::GetBodyManager, py::return_value_policy::reference)
		.def("pixel2meter", [](float v) {return pixel2meter(v); })
		.def("pixel2meter", [](sf::Vector2f v) {return pixel2meter(v); })
		.def("pixel2meter", [](Vec2f v) {return pixel2meter(v); })
		.def("meter2pixel", [](float v) {return meter2pixel(v); })
		.def("meter2pixel", [](b2Vec2 v)->Vec2f {return meter2pixel(v); });

	py::class_<Body2dManager> body2dManager(m, "Body2dManager");
	body2dManager
	    .def("add_component", &Body2dManager::AddComponent, py::return_value_policy::reference)
	    .def("get_component", &Body2dManager::GetComponentRef, py::return_value_policy::reference);

	py::class_<Graphics2dManager> graphics2dManager(m, "Graphics2dManager");
	graphics2dManager
		.def(py::init<Engine&>(), py::return_value_policy::reference)
		.def_property_readonly("sprite_manager", &Graphics2dManager::GetSpriteManager, py::return_value_policy::reference)
		.def_property_readonly("texture_manager", &Graphics2dManager::GetTextureManager, py::return_value_policy::reference)
		.def_property_readonly("shape_manager", &Graphics2dManager::GetShapeManager, py::return_value_policy::reference)
		.def("get_size_window", &Graphics2dManager::GetSizeWindow, py::return_value_policy::reference)
		.def("get_position_window", &Graphics2dManager::GetPositionWindow, py::return_value_policy::reference);

	py::class_<TextureManager> textureManager(m, "texture_manager");
	textureManager
		.def("load_texture", [](TextureManager* textureManager, std::string name)
		{
			const auto textureId = textureManager->LoadTexture(name);
			return textureManager->GetTexture(textureId);
		}, py::return_value_policy::reference);
	py::class_<sf::Texture, std::unique_ptr<sf::Texture, py::nodelete>> sfTexture(m, "sfTexture");

	py::class_<SpriteManager> spriteManager(m, "SpriteManager");
	spriteManager
		.def("create_component", [](SpriteManager* spriteManager, Entity entity, std::string texturePath)
		{
			TextureManager* textureManager = spriteManager->GetEngine().GetGraphics2dManager()->GetTextureManager();

			const auto textureId = textureManager->LoadTexture(texturePath);
			auto* texture = textureManager->GetTexture(textureId);
			auto* sprite = spriteManager->AddComponent(entity);
			sprite->SetTexture(texture);

			auto& spriteInfo = spriteManager->GetComponentInfo(entity);
			spriteInfo.name = "Sprite";
			spriteInfo.textureId = textureId;
			spriteInfo.texturePath = texturePath;
		}, py::return_value_policy::reference);

	py::class_<ShapeManager> shapeManager(m, "ShapeManager");
	shapeManager
		.def(py::init<Engine&>(), py::return_value_policy::reference);

	py::class_<PythonEngine> pythonEngine(m, "PythonEngine");
	pythonEngine
	    .def(py::init<Engine&>(), py::return_value_policy::reference)
		.def("load_pycomponent", [](PythonEngine* pythonEngineInstance, Entity entity, std::string scriptPath){
			auto moduleId = pythonEngineInstance->LoadPyModule(scriptPath);
			auto pyComponentId = pythonEngineInstance->GetPyComponentManager().LoadPyComponent(moduleId, entity);
			pythonEngineInstance->SpreadClasses();
			return py::cast(pythonEngineInstance->GetPyComponentManager().GetPyComponentFromInstanceId(pyComponentId));
		}, py::return_value_policy::reference);

	py::class_<PySystemManager, System> pySystemManager(m, "pySystemManager");
	pySystemManager
		.def(py::init<Engine&>(), py::return_value_policy::reference)
		.def("get_pysystem", &PySystemManager::GetPySystemFromClassName, py::return_value_policy::reference);

	py::class_<Behavior, PyBehavior> component(m, "Component");
	component
		.def(py::init<Engine&, Entity>(), py::return_value_policy::reference)
		.def("init", &Behavior::Init)
		.def("update", &Behavior::Update)
		.def("fixed_update", &Behavior::FixedUpdate)
		.def_property_readonly("entity", &Behavior::GetEntity)
		.def("get_component", &Behavior::GetComponent)
		.def("get_component", &Behavior::GetPyComponent)
		.def("on_trigger_enter", &Behavior::OnTriggerEnter)
		.def("on_collision_enter", &Behavior::OnCollisionEnter)
		.def("on_trigger_exit", &Behavior::OnTriggerExit)
		.def("on_collision_exit", &Behavior::OnCollisionExit);

	py::enum_<ComponentType>(component, "ComponentType")
		.value("PyComponent", ComponentType::PYCOMPONENT)
		.value("Shape", ComponentType::SHAPE2D)
		.value("Body", ComponentType::BODY2D)
		.value("Sprite", ComponentType::SPRITE2D)
		.value("Sound", ComponentType::SOUND)
		.value("Transform2d", ComponentType::TRANSFORM2D)
		.value("Camera", ComponentType::CAMERA)
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

	py::class_<Camera> camera(m, "Camera");
	camera
		.def("get_position", &Camera::GetPosition, py::return_value_policy::reference)
		.def("set_position", &Camera::SetPosition, py::return_value_policy::reference)
		.def("on_resize", &Camera::OnResize, py::return_value_policy::reference);

	py::class_<Shape> shape(m, "Shape");
	shape
	.def(py::init(), py::return_value_policy::reference)
		.def("set_fill_color", &Shape::SetFillColor);
	py::class_<Sprite> sprite(m, "Sprite");
	sprite
		.def("set_texture", &Sprite::SetTexture, py::return_value_policy::reference);
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
		.def("get_current", &Timer::GetCurrentRatio)
		.def("get_current_time", &Timer::GetTimeFromStart)
		.def("is_over", &Timer::IsOver)
		.def_property("period", &Timer::GetPeriod, &Timer::SetPeriod)
		.def("__repr__", [](const Timer &timer)
	{
		std::ostringstream oss;
		oss << "(" << timer.GetTime() << ", " << timer.GetPeriod() << ")";
		return oss.str();
	});

	py::class_<Vec2f> vec2f(m, "Vec2f");
	vec2f
        .def(py::init<float, float>())
        .def(py::init<>())
        .def(py::self + py::self)
        .def(py::self += py::self)
        .def(py::self - py::self)
        .def(py::self -= py::self)
        .def(py::self * float())
        .def(py::self / float())
        .def_property_readonly("magnitude", &Vec2f::GetMagnitude)
        .def_static("dot", &Vec2f::Dot)
        .def_static("angle_between", &Vec2f::AngleBetween)
        .def_static("lerp", &Vec2f::Lerp)
        .def("rotate", [](Vec2f& v1, float angle){
          float radianAngle = angle/180.0f*M_PI;
          v1 = Vec2f(cos(radianAngle)*v1.x-sin(radianAngle)*v1.y,sin(radianAngle)*v1.x+cos(radianAngle)*v1.y);
        })
        .def_property_readonly_static("down", [](py::object){ return Vec2f(0.0f,1.0f);})
        .def_property_readonly_static("up", [](py::object){ return Vec2f(0.0f,-1.0f);})
        .def_property_readonly_static("right", [](py::object){ return Vec2f(1.0f,0.0f);})
        .def_property_readonly_static("left", [](py::object){ return Vec2f(-1.0f,0.0f);})
        .def_readwrite("x", &Vec2f::x)
        .def_readwrite("y", &Vec2f::y)
        .def("__repr__", [](const Vec2f &vec)
        {
          std::ostringstream oss;
          oss << "Vec2f(" << vec.x << ", " << vec.y << ")";
          return oss.str();
        });

	py::class_<sf::Vector2i> vector2i(m, "Vector2i");
	vector2i
		.def(py::init<int, int>())
		.def(py::init<>())
		.def(py::self + py::self)
		.def(py::self += py::self)
		.def(py::self - py::self)
		.def(py::self -= py::self)
		.def_property_readonly_static("down", [](py::object){ return sf::Vector2i(0,1);})
		.def_property_readonly_static("up", [](py::object){ return sf::Vector2i(0,-1);})
		.def_property_readonly_static("right", [](py::object){ return sf::Vector2i(1,0);})
		.def_property_readonly_static("left", [](py::object){ return sf::Vector2i(-1,0);})
		.def_readwrite("x", &sf::Vector2i::x)
		.def_readwrite("y", &sf::Vector2i::y)
		.def("__repr__", [](const sf::Vector2i &vec)
		{
			std::ostringstream oss;
			oss << "sf::Vector2i(" << vec.x << ", " << vec.y << ")";
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
			.def_static("dot", [](const sf::Vector2f& v1, const sf::Vector2f& v2)
		{
			return v1.x*v2.x + v1.y*v2.y;
		})
			.def_static("angle_between", [](const sf::Vector2f& v1, const sf::Vector2f& v2)
		{
			float dot = v1.x*v2.x + v1.y*v2.y;
			float angle = acosf(dot) / M_PI * 180.0f;
			return angle;

		})
			.def_static("lerp", [](const sf::Vector2f&v1, const sf::Vector2f&v2, float t)
		{
			return v1 + (v2 - v1)*t;
		})
			.def("rotate", [](sf::Vector2f& v1, float angle) {
			float radianAngle = angle / 180.0f*M_PI;
			v1 = sf::Vector2f(cos(radianAngle)*v1.x - sin(radianAngle)*v1.y, sin(radianAngle)*v1.x + cos(radianAngle)*v1.y);
		})
			.def_property_readonly_static("down", [](py::object) { return sf::Vector2f(0.0f, 1.0f); })
			.def_property_readonly_static("up", [](py::object) { return sf::Vector2f(0.0f, -1.0f); })
			.def_property_readonly_static("right", [](py::object) { return sf::Vector2f(1.0f, 0.0f); })
			.def_property_readonly_static("left", [](py::object) { return sf::Vector2f(-1.0f, 0.0f); })
			.def_readwrite("x", &sf::Vector2f::x)
			.def_readwrite("y", &sf::Vector2f::y)
			.def("__repr__", [](const sf::Vector2f &vec)
		{
			std::ostringstream oss;
			oss << "sf::Vector2f(" << vec.x << ", " << vec.y << ")";
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
			oss << "b2Vec2(" << vec.x << ", " << vec.y << ")";
			return oss.str();
		});

	ext::ExtendPython(m);
	
}

void PythonEngine::Init()
{

	Log::GetInstance()->Msg("Initialise the python embed interpretor");
	System::Init();
	m_PyComponentManager.Init();
	m_PySystemManager.Init();

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
		sfgeModule.attr("python_engine") = py::cast(m_Engine.GetPythonEngine(), py::return_value_policy::reference);
		sfgeModule.attr("graphics2d_manager") = py::cast(m_Engine.GetGraphics2dManager(), py::return_value_policy::reference);
	}
	catch (py::error_already_set& e)
	{
		std::ostringstream oss;
		oss << "[ERROR] Python already set error: " << e.what();
		Log::GetInstance()->Error(oss.str());
	}
	LoadScripts();
}

void PythonEngine::InitScriptsInstances()
{
	m_PyComponentManager.InitPyComponents();
}


void PythonEngine::Update(float dt)
{
	rmt_ScopedCPUSample(PythonUpdate,0);
	m_PyComponentManager.Update(dt);
	m_PySystemManager.Update(dt);
}

void PythonEngine::FixedUpdate()
{

	rmt_ScopedCPUSample(PythonFixedUpdate,0);
	m_PyComponentManager.FixedUpdate();
	m_PySystemManager.FixedUpdate();
}

void PythonEngine::Draw()
{
	rmt_ScopedCPUSample(PythonDraw,0);
	m_PyComponentManager.Draw();
	m_PySystemManager.Draw();
}


void PythonEngine::Destroy()
{
	System::Destroy();
	m_PyComponentManager.Destroy();
	m_PySystemManager.Destroy();
	m_PyModuleObjs.clear();
	Log::GetInstance()->Msg("Finalize the python embed interpretor");
	py::finalize_interpreter();
}

void PythonEngine::Clear()
{

}

ModuleId PythonEngine::LoadPyModule(std::string moduleFilename)
{
	const auto folderLastIndex = moduleFilename.find_last_of('/');
	std::string filename = moduleFilename.substr(folderLastIndex + 1, moduleFilename.size());
	const auto filenameExtensionIndex = filename.find_last_of('.');
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
			if(m_PythonModulePaths[testedModuleId - 1] == moduleFilename)
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
				m_PyModuleObjs[moduleId-1] = import(moduleName, moduleFilename, globals);
				m_PyModuleNames[moduleId-1] = moduleName;
				m_PythonModulePaths[moduleId-1] = moduleFilename;
				m_PyClassNames[moduleId-1] = className;

				{
					std::ostringstream oss;
					oss << "Loading module: " << moduleName << " with class: " << className << ",\n";
					Log::GetInstance()->Msg(oss.str());
				}
			}
			catch (const std::runtime_error& e)
			{
				std::stringstream oss;
				oss << "[PYTHON ERROR] on script file: " << moduleFilename << "\n" << e.what();
				Log::GetInstance()->Error(oss.str());
					return INVALID_MODULE;
			}
			SpreadClasses();
			m_IncrementalModuleId++;
			return moduleId;
		}
	}
    {
        std::ostringstream oss;
        oss << "[Error]: Could not load " << moduleName << " because it is not a regular file.\n";
        Log::GetInstance()->Msg(oss.str());
    }
	return INVALID_MODULE;
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

		if (IsDirectory(entry) and entry.find("tools") == std::string::npos)
		{
			IterateDirectory(entry, LoadAllPyModules);
		}
	};
	IterateDirectory(dirname, LoadAllPyModules);
	SpreadClasses();
}

void PythonEngine::SpreadClasses()
{
    //Spread the class name in all the scripts
    for(ModuleId moduleId = 1U; moduleId < m_IncrementalModuleId; moduleId++)
    {
        for (ModuleId otherModuleId = 1U; otherModuleId < m_IncrementalModuleId; otherModuleId++)
        {
            if(moduleId == otherModuleId) continue;
            try
            {
                m_PyModuleObjs[moduleId-1].attr(py::str(m_PyClassNames[otherModuleId-1])) =
                        m_PyModuleObjs[otherModuleId-1].attr(py::str(m_PyClassNames[otherModuleId-1]));
            }
            catch (std::runtime_error& e)
            {
                std::ostringstream oss;
                oss << "[PYTHON ERROR] Could not import class: " << m_PyClassNames[otherModuleId-1] << " into module: " << moduleId << " with error: " << e.what();
                Log::GetInstance()->Error(oss.str());
            }
        }

    }
}
const std::string &PythonEngine::GetClassNameFrom(ModuleId moduleId)
{
	return m_PyClassNames[moduleId-1];
}
const std::string &PythonEngine::GetModuleNameFrom(ModuleId moduleId)
{
	return m_PyModuleNames[moduleId-1];
}
const std::string &PythonEngine::GetModulePathFrom(ModuleId moduleId)
{
	return m_PythonModulePaths[moduleId-1];
}
const pybind11::object & PythonEngine::GetModuleObjFrom(ModuleId moduleId)
{
	return m_PyModuleObjs[moduleId-1];
}
void PythonEngine::ExecutePythonCommand(std::string pythonCommand)
{
	try
	{
		py::exec(pythonCommand);
	}
	catch(py::error_already_set& e)
	{
		Log::GetInstance()->Error(e.what());
	}

}

}
