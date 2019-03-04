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

#include <string>
#include <engine/engine.h>
#include <engine/scene.h>
#include <engine/component.h>
#include <utility/json_utility.h>
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <random>
#include <graphics/rect_transform.h>
#include <graphics/ui.h>
#include <graphics/graphics2d.h>
#include <python/python_engine.h>
//#include <minwindef.h>
#include <Windows.h>
#include "audio/audio.h"
#include <stack>
#include <ctime>
#include <input/input.h>
#include <physics/physics2d.h>
#include <engine/entity.h>
#include <engine/transform2d.h>
#include <editor/editor.h>
#include <engine/tilemap.h>
#include <graphics/shape2d.h>

//#define DRAW_EMABLED

// This function will return true if the pointer points to stack. Otherwise false
// Source : https://www.codeproject.com/articles/35206/pointer-pointing-to-stack-or-heap
bool IsMemoryOnStack(LPVOID pVoid)
{
	LPVOID dwStackTop = 0;
	LPVOID dwStackLowCurrent = 0;
	__asm
	{
		mov EAX, FS:[4]
		mov dwStackTop, eax
		mov EAX, FS : [8]
		mov dwStackLowCurrent, eax
	}

	std::cout << "\nStack top : "<< dwStackTop << " | Stack Low Current : "<< dwStackLowCurrent << " | Parameter : " << pVoid;

	if (pVoid <= dwStackTop && pVoid >= dwStackLowCurrent)
	{

		// The memory lie between the stack top and stack committed.
		return true;
	}

	// Pointer doesn't point to the stack
	return false;
}

json CreateFakeGameScene(float mapSize, float buildingFactor, float dwarfFactor, float uiElement)
{
	int mapTotalSize = mapSize * mapSize;
	int buildingNumber = mapTotalSize * buildingFactor;
	int dwarfNumber = buildingNumber * dwarfFactor;

	int totalEntities = mapTotalSize + buildingNumber + dwarfNumber + uiElement;

	std::cout << "Map size : " << mapTotalSize << " | Building number : " << buildingNumber << " | Dwarf number : " << dwarfNumber << " | Total entities : " << totalEntities;

	//std::ofstream debugFile;

	json mapEntitie = 
	{
		{"name", "Tile"},
		{"components", 
			{
				{
					{"name", "Transform"},
					{"type", static_cast<int>(sfge::ComponentType::TRANSFORM2D)},
					{"position", {0.0,0.0}},
					{"scale", {1.0,1.0}},
					{"angle", 0.0}
				},
				{
					{"name", "Sprite"},
					{"type", static_cast<int>(sfge::ComponentType::SPRITE2D)},
					{"path", "data/sprites/other_play.png"},
					{"layer", 1}
				}
			}
		}
	};

	json buildingEntitie =
	{
		{"name", "Building"},
		{"components",
			{
				{
					{"name", "Transform"},
					{"type", static_cast<int>(sfge::ComponentType::TRANSFORM2D)},
					{"position", {0.0,0.0}},
					{"scale", {1.0,1.0}},
					{"angle", 0.0}
				},
				{
					{"name", "Sprite"},
					{"type", static_cast<int>(sfge::ComponentType::SPRITE2D)},
					{"path", "data/sprites/other_play.png"},
					{"layer", 1}
				}
			}
		}
	};

	json dwarfEntitie =
	{
		{"name", "Dwarf"},
		{"components", 
			{
				{
					{"name", "Transform"},
					{"type", static_cast<int>(sfge::ComponentType::TRANSFORM2D)},
					{"position", {0.0,0.0}},
					{"scale", {1.0,1.0}},
					{"angle", 0.0}
				},
				{
					{"name", "Sprite"},
					{"type", static_cast<int>(sfge::ComponentType::SPRITE2D)},
					{"path", "data/sprites/other_play.png"},
					{"layer", 1}
				}
			}
		}
	};

	json uiEntitie =
	{
		{"name", "UIElement"},
		{"components", 
			{
				{
					{"name", "RectTransform"},
					{"type", static_cast<int>(sfge::ComponentType::RECTTRANSFORM)},
					{"basePosition", {0.0,0.0}},
					{"scale", {1.0,1.0}},
					{"angle", 0.0}
				},
				{
					{"name", "Image"},
					{"type", static_cast<int>(sfge::ComponentType::IMAGE)},
					{"spritePath", "data/sprites/other_play.png"},
					{"color", {255,222,173,255}}
				},
				{
					{"name", "Button"},
					{"type", static_cast<int>(sfge::ComponentType::BUTTON)}
				},
				{
					{"name", "Script"},
					{"type", sfge::ComponentType::PYCOMPONENT},
					{"script_path", "scripts/button_test.py"}
				}
			}
		}
	};


	json scene =
	{
		{"name", "ChargeTest"},
		{"entities", {}}
	};

	for (int i = 0; i < mapTotalSize; i++)
	{
		scene["entities"].push_back(mapEntitie);
	}

	for (int i = 0; i < buildingNumber; i++)
	{
		scene["entities"].push_back(buildingEntitie);
	}

	for (int i = 0; i < dwarfNumber; i++)
	{
		scene["entities"].push_back(dwarfEntitie);
	}

	for (int i = 0; i < uiElement; i++)
	{
		scene["entities"].push_back(uiEntitie);
	}

	/*debugFile.open("chargeDebug.log");
	debugFile << scene;
	debugFile.close();*/

	return scene;
}

json CreatOverloadedScene(int entityNumber)
{
#pragma region Json_Component_Creation
	json transformJson = 
	{
		{"name", "Transform"},
		{"type", static_cast<int>(sfge::ComponentType::TRANSFORM2D)},
		{"position", {0.0,0.0}},
		{"scale", {1.0,1.0}},
		{"angle", 0.0}
	};

	json spriteJson =
	{
		{"name", "Sprite"},
		{"type", static_cast<int>(sfge::ComponentType::SPRITE2D)},
		{"path", "data/sprites/triangle.png"},
		{"layer", 1}
	};

	json shapeJson =
	{
		{"name", "Shape"},
		{"type", static_cast<int>(sfge::ComponentType::SHAPE2D)},
		{"shape_type", static_cast<int>(sfge::ShapeType::RECTANGLE)},
		{"offset", {0,0}},
		{"size", {1,1}}
	};

	json pyComponentJson =
	{
		{"name", "Script"},
		{"type", sfge::ComponentType::PYCOMPONENT},
		{"script_path", "scripts/component_test.py"}
	};

	json animJson =
	{
		{"name", "Animation"},
		{"path", "data/animSaves/cowboy_walk.json"},
		{"type", static_cast<int>(sfge::ComponentType::ANIMATION2D)}
	};

	json rectTransformJson =
	{
		{"name", "RectTransform"},
		{"type", static_cast<int>(sfge::ComponentType::RECTTRANSFORM)},
		{"basePosition", {0.0,0.0}},
		{"scale", {1.0,1.0}},
		{"angle", 0.0}
	};

	json imageJson =
	{
		{"name", "Image"},
		{"type", static_cast<int>(sfge::ComponentType::IMAGE)},
		{"spritePath", "data/sprites/round.png"},
		{"color", {255,222,173,255}}
	};

	json buttonJson =
	{
		{"name", "Button"},
		{"type", static_cast<int>(sfge::ComponentType::BUTTON)}
	};
#pragma endregion

#pragma region Json_Entity_Creation
	json shapeEntityJson;
	shapeEntityJson["components"] = json::array({ transformJson, shapeJson });

	json spriteEntityJson;
	spriteEntityJson["components"] = json::array({ transformJson, spriteJson });

	json complexeEntityJson;
	complexeEntityJson["components"] = json::array({ transformJson, animJson, pyComponentJson });

	json simpleUiEntityJson;
	simpleUiEntityJson["components"] = json::array({ rectTransformJson, imageJson });

	json buttonUiEntityJson;
	simpleUiEntityJson["components"] = json::array({ rectTransformJson, imageJson, buttonJson, pyComponentJson });
#pragma endregion 

#pragma region Json_Scene_Creation
	json scene;

	int shapeEntityCount = 0;
	int spriteEntityCount = 0;
	int complexeEntityCount = 0;
	int simpleUiEntityCount = 0;
	int btnUiEntityCont = 0;

	scene["name"] = "OverloadedScene";

	int randValue = rand() % entityNumber;
	shapeEntityCount = randValue;
	entityNumber -= shapeEntityCount;

	randValue = rand() % entityNumber;
	spriteEntityCount = randValue;
	entityNumber -= spriteEntityCount;

	randValue = rand() % entityNumber;
	complexeEntityCount = randValue;
	entityNumber -= complexeEntityCount;

	randValue = rand() % entityNumber;
	simpleUiEntityCount = randValue;
	entityNumber -= simpleUiEntityCount;

	btnUiEntityCont = entityNumber;

	for (int i = 0; i < shapeEntityCount; i++)
		scene["entities"].push_back(shapeEntityJson);

	for (int i = 0; i < spriteEntityCount; i++)
		scene["entities"].push_back(spriteEntityJson);

	for (int i = 0; i < complexeEntityCount; i++)
		scene["entities"].push_back(complexeEntityJson);

	for (int i = 0; i < simpleUiEntityCount; i++)
		scene["entities"].push_back(simpleUiEntityJson);

	for (int i = 0; i < btnUiEntityCont; i++)
		scene["entities"].push_back(buttonUiEntityJson);

	std::cout << "\nShape E. : " << std::to_string(shapeEntityCount)
		<< "\nSprite E. : " << std::to_string(spriteEntityCount)
		<< "\nComplexe E. :" << std::to_string(complexeEntityCount)
		<< "\nS.UI E. : " << std::to_string(simpleUiEntityCount)
		<< "\nBtn UI E. : " << std::to_string(btnUiEntityCont);
#pragma endregion

	return scene;
}

/*void VectorPushAndErase(size_t size, int eraseNumber)
{
	::testing::internal::TimeInMillis elapsed(
		::testing::UnitTest::GetInstance()->elapsed_time());

	std::vector<unsigned> vector;

	//vector.reserve(size);

	for (unsigned i = 0; i < size; i++)
		vector.push_back(i);

	for (int i = 0; i < eraseNumber; i++)
	{
		//int randValue = rand() % eraseNumber;
		vector.erase(vector.begin());
	}

	sfge::Log::GetInstance()->Msg("P " + std::to_string(size) + " : " + std::to_string(elapsed) + "\n");
	//std::cout << "P " << size << " : " << elapsed << "\n";
}

void VectorReserveAndErase(size_t size, int eraseNumber)
{
	::testing::internal::TimeInMillis elapsed(
		::testing::UnitTest::GetInstance()->elapsed_time());

	std::vector<unsigned> vector;

	vector.reserve(size);

	for (unsigned i = 0; i < size; i++)
		vector[i] = i;

	for (int i = 0; i < eraseNumber; i++)
	{
		//int randValue = rand() % eraseNumber;
		//vector.erase(vector.begin());
	}

	sfge::Log::GetInstance()->Msg("R " + std::to_string(size) + " : " + std::to_string(elapsed) + "\n");
	//std::cout << "R " << size << " : " << elapsed << "\n";
}

TEST(Vector, VectorFunction)
{
	VectorPushAndErase(1000, 25);
	VectorReserveAndErase(1000, 25);
	VectorPushAndErase(10000, 250);
	VectorReserveAndErase(10000, 250);
	VectorPushAndErase(50000, 1250);
	VectorReserveAndErase(50000, 1250);
}*/

/*
 * This test tell if the managers are located on the stack and give the size for each one
 */
TEST(Stack, IsOnStack)
{
#pragma region Initialisation_And_Get_Systems
	sfge::Engine engine;
	engine.Init();

	/*engine.GetSceneManager()->LoadSceneFromPath("data/scenes/test_ui.scene");

	engine.Start();*/

	// Engine managers
	sfge::Graphics2dManager* graphics2dManager = engine.GetGraphics2dManager();
	sfge::AudioManager* audioManager = engine.GetAudioManager();
	sfge::PythonEngine* pythonEngine = engine.GetPythonEngine();
	sfge::PyComponentManager* pyComponentManager = &pythonEngine->GetPyComponentManager();
	sfge::Physics2dManager* physicsManager = engine.GetPhysicsManager();
	sfge::EntityManager* entityManager = engine.GetEntityManager();
	sfge::Transform2dManager* transformManager = engine.GetTransform2dManager();
	sfge::RectTransformManager* rectTransformManager = engine.GetRectTransformManager();
	sfge::UIManager* uiManager = engine.GetUIManager();

	// Graphics2d managers
	sfge::AnimationManager* animationManager = graphics2dManager->GetAnimationManager();
	sfge::ShapeManager* shapeManager = graphics2dManager->GetShapeManager();
	sfge::SpriteManager* spriteManager = graphics2dManager->GetSpriteManager();
	sfge::TextureManager* textureManager = graphics2dManager->GetTextureManager();
	sfge::CameraManager* cameraManager = graphics2dManager->GetCameraManager();

	sf::RenderWindow* m_Window = graphics2dManager->GetWindow();

	// UI managers
	sfge::ButtonManager* buttonManager = uiManager->GetButtonManager();
	sfge::TextManager* textManager = uiManager->GetTextManager();
	sfge::ImageManager* imageManager = uiManager->GetImageManager();
#pragma endregion 

	int OnStack = 1000000000;

	bool check = IsMemoryOnStack(&OnStack);
	check ? std::cout << "\nTest value is on stack\n" : std::cout << "\nTest value is not on stack\n";
	std::cout << "Size : " << sizeof(OnStack) << "\n";

	check = IsMemoryOnStack(graphics2dManager);
	check ? std::cout << "\nGraphics manager is on stack\n" : std::cout << "\nGraphics manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::Graphics2dManager) << "\n";

	check = IsMemoryOnStack(audioManager);
	check ? std::cout << "\nAudio manager is on stack\n" : std::cout << "\nAudio manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::AudioManager) << "\n";

	check = IsMemoryOnStack(pythonEngine);
	check ? std::cout << "\nPython engine is on stack\n" : std::cout << "\nPython engine is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::PythonEngine) << "\n";

	check = IsMemoryOnStack(pyComponentManager);
	check ? std::cout << "\nPy manager is on stack\n" : std::cout << "\nPy manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::PyComponentManager) << "\n";

	check = IsMemoryOnStack(physicsManager);
	check ? std::cout << "\nPhysics manager is on stack\n" : std::cout << "\nPhysics manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::Physics2dManager) << "\n";

	check = IsMemoryOnStack(entityManager);
	check ? std::cout << "\nEntity manager is on stack\n" : std::cout << "\nEntity manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::EntityManager) << "\n";

	check = IsMemoryOnStack(transformManager);
	check ? std::cout << "\nTransform manager is on stack\n" : std::cout << "\nTransform manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::Transform2dManager) << "\n";

	check = IsMemoryOnStack(rectTransformManager);
	check ? std::cout << "\nRect Transform manager is on stack\n" : std::cout << "\nRect Transform manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::RectTransformManager) << "\n";

	check = IsMemoryOnStack(uiManager);
	check ? std::cout << "\nUI manager is on stack\n" : std::cout << "\nUI manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::UIManager) << "\n";

	check = IsMemoryOnStack(animationManager);
	check ? std::cout << "\nAnimation manager is on stack\n" : std::cout << "\nAnimation manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::AnimationManager) << "\n";

	check = IsMemoryOnStack(shapeManager);
	check ? std::cout << "\nShape manager is on stack\n" : std::cout << "\nShape manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::ShapeManager) << "\n";

	check = IsMemoryOnStack(spriteManager);
	check ? std::cout << "\nSprite manager is on stack\n" : std::cout << "\nSprite manager is not on stack\n";
	std::cout << "Size : " << sizeof(sfge::SpriteManager) << "\n";

	check = IsMemoryOnStack(graphics2dManager);
	check ? std::cout << "\nGraphics manager is on stack\n" : std::cout << "\nGraphics manager is not on stack\n";

	check = IsMemoryOnStack(graphics2dManager);
	check ? std::cout << "\nGraphics manager is on stack\n" : std::cout << "\nGraphics manager is not on stack\n";

	check = IsMemoryOnStack(graphics2dManager);
	check ? std::cout << "\nGraphics manager is on stack\n" : std::cout << "\nGraphics manager is not on stack\n";

	check = IsMemoryOnStack(graphics2dManager);
	check ? std::cout << "\nGraphics manager is on stack\n" : std::cout << "\nGraphics manager is not on stack\n";

	return SUCCEED();
}

std::string FakeEngine(int iteration, json scene)
{
	sfge::Engine engine;
	engine.Init();

	engine.GetSceneManager()->LoadSceneFromJson(scene);

	// Engine managers
	sfge::Graphics2dManager* graphics2dManager = engine.GetGraphics2dManager();
	sfge::AudioManager* audioManager = engine.GetAudioManager();
	sfge::SceneManager* sceneManager = engine.GetSceneManager();
	sfge::InputManager* inputManager = engine.GetInputManager();
	sfge::PythonEngine* pythonEngine = engine.GetPythonEngine();
	sfge::PyComponentManager* pyComponentManager = &pythonEngine->GetPyComponentManager();
	sfge::Physics2dManager* physicsManager = engine.GetPhysicsManager();
	sfge::EntityManager* entityManager = engine.GetEntityManager();
	sfge::Transform2dManager* transformManager = engine.GetTransform2dManager();
	sfge::RectTransformManager* rectTransformManager = engine.GetRectTransformManager();
	sfge::UIManager* uiManager = engine.GetUIManager();
	sfge::Editor* editor = engine.GetEditor();
	sfge::TilemapSystem* tilemapSystem = engine.GetTilemapSystem();

	// Graphics2d managers
	sfge::AnimationManager* animationManager = graphics2dManager->GetAnimationManager();
	sfge::ShapeManager* shapeManager = graphics2dManager->GetShapeManager();
	sfge::SpriteManager* spriteManager = graphics2dManager->GetSpriteManager();
	sfge::TextureManager* textureManager = graphics2dManager->GetTextureManager();
	sfge::CameraManager* cameraManager = graphics2dManager->GetCameraManager();

	sf::RenderWindow* m_Window = graphics2dManager->GetWindow();

	// UI managers
	sfge::ButtonManager* buttonManager = uiManager->GetButtonManager();
	sfge::TextManager* textManager = uiManager->GetTextManager();
	sfge::ImageManager* imageManager = uiManager->GetImageManager();

#pragma region Fake_Engine
	int i = 0;
	sf::Clock globalClock;
	sf::Clock updateClock;
	sf::Clock fixedUpdateClock;
	sf::Clock graphicsUpdateClock;
	sf::Time previousFixedUpdateTime = globalClock.getElapsedTime();
	sf::Time deltaFixedUpdateTime = sf::Time();
	sf::Time dt = sf::Time();

	sfge::Configuration* m_Config = engine.GetConfig();

#ifdef DRAW_EMABLED
	rmt_BindOpenGL();
#endif

	std::clock_t timer;

	timer = std::clock();

	while (i < iteration && m_Window != nullptr)
	{
		rmt_ScopedOpenGLSample(SFGE_Frame_GL);
		rmt_ScopedCPUSample(SFGE_Frame, 0)

			bool isFixedUpdateFrame = false;
#ifdef DRAW_EMABLED
		sf::Event event{};
		while (m_Window != nullptr &&
			m_Window->pollEvent(event))
		{
			editor->ProcessEvent(event);
			if (event.type == sf::Event::Closed)
			{
				m_Window->close();
			}

		}
#endif

		inputManager->Update(dt.asSeconds());
		auto fixedUpdateTime = globalClock.getElapsedTime() - previousFixedUpdateTime;
		if (fixedUpdateTime.asSeconds() > m_Config->fixedDeltaTime)
		{
			fixedUpdateClock.restart();
			physicsManager->FixedUpdate();
			previousFixedUpdateTime = globalClock.getElapsedTime();
			tilemapSystem->FixedUpdate();
			pythonEngine->FixedUpdate();
			sceneManager->FixedUpdate();
			deltaFixedUpdateTime = fixedUpdateClock.getElapsedTime();
			isFixedUpdateFrame = true;
		}
		tilemapSystem->Update(dt.asSeconds());

		pythonEngine->Update(dt.asSeconds());

		sceneManager->Update(dt.asSeconds());

#ifdef DRAW_EMABLED
		editor->Update(dt.asSeconds());
#endif
		graphicsUpdateClock.restart();
		transformManager->Update(dt.asSeconds());

		rectTransformManager->Update(dt.asSeconds());
		uiManager->Update(dt.asSeconds());

		graphics2dManager->Update(dt.asSeconds());

#ifdef DRAW_EMABLED
		sceneManager->Draw();

		uiManager->Draw();
		graphics2dManager->Draw();

		pythonEngine->Draw();
		editor->Draw();
		graphics2dManager->Display();

		const sf::Time graphicsDt = graphicsUpdateClock.getElapsedTime();
#endif
		dt = updateClock.restart();
		i++;
	}

	auto duration = 1000.0 * (std::clock() - timer) / CLOCKS_PER_SEC;

	// Suppression of all the shapes
	std::cout << "\n Destruction of shape";
	std::vector<Entity> e = shapeManager->GetConcernedEntities();
	if (e.size() > 0)
	{
		for (unsigned i = 0; i < 2; i++)
		{
			shapeManager->DestroyComponent(e[i]);
		}
	}

	std::cout << "\n Destruction of sprite";
	e = spriteManager->GetConcernedEntities();
	// Suppression of all the sprites
	if (e.size() > 0)
	{
		for (unsigned i = 0; i < 2; i++)
		{
			spriteManager->DestroyComponent(e[i]);
		}
	}

	std::cout << "\n Destruction of pyComponent";
	e = pyComponentManager->GetConcernedEntities();
	// Suppression of all the python scripts
	if (e.size() > 0)
	{
		for (unsigned i = 0; i < 2; i++)
		{
			pyComponentManager->DestroyComponent(i);
		}
	}

	std::cout << "\n Destruction of animation";
	e = animationManager->GetConcernedEntities();
	// Suppression of all the animations
	if (e.size() > 0)
	{
		for (unsigned i = 0; i < 2; i++)
		{
			animationManager->DestroyComponent(e[i]);
		}
	}

	std::cout << "\n Destruction of button";
	e = buttonManager->GetConcernedEntities();
	// Suppression of all the buttons
	if (e.size() > 0)
	{
		for (unsigned i = 0; i < 2; i++)
		{
			buttonManager->DestroyComponent(e[i]);
		}
	}

	std::cout << "\n Destruction of image";
	e = imageManager->GetConcernedEntities();
	// Suppression of all the images
	if (e.size() > 0)
	{
		for (unsigned i = 0; i < 2; i++)
		{
			imageManager->DestroyComponent(e[i]);
		}
	}

	std::cout << "\n Destruction of transform";
	e = transformManager->GetConcernedEntities();
	// Suppression of all the transforms
	if (e.size() > 0)
	{
		for (unsigned i = 0; i < 2; i++)
		{
			transformManager->DestroyComponent(e[i]);
		}
	}

	std::cout << "\n Destruction of rectTransform";
	e = rectTransformManager->GetConcernedEntities();
	// Suppression of all the rect transforms
	if (e.size() > 0)
	{
		for (unsigned i = 0; i < 2; i++)
		{
			rectTransformManager->DestroyComponent(e[i]);
		}
	}

#ifdef DRAW_EMABLED
	rmt_UnbindOpenGL();
#endif

	engine.Destroy();

	return std::to_string(duration);
}

std::string IterateFakeEngine(int en, int i, int ei,json scene)
{
	std::string result;
	std::string Treatment;

	for (int j = 0; j < i; j++)
	{
		std::cout << "\n---------------------Start test S1 iteration " << j << " without draw---------------------";
		Treatment = FakeEngine(ei, scene);
		result += (std::to_string(en) + ";" + Treatment + "\n");
		std::cout << "Treatment : " << Treatment;
		std::cout << "\n----------------------End test S1 iteration " << j << " without draw---------------------";
	}

	return result;
}

TEST(Charge, TestChargeLight)
{
	std::string returnedValues = "Entity Number;Treatment\n";
	//std::ofstream resultFile;

	const int iteration = 1;
	const int engineIterations = 1;
	const int eN1 = 32;
	/*const int eN2 = 32;
	const int eN3 = 64;
	const int eN4 = 128;
	const int eN5 = 256;
	const int eN6 = 512;
	const int eN7 = 1024;
	const int eN8 = 2048;
	const int eN9 = 4096;
	const int eN10 = 8192;
	const int eN11 = 16384;*/

	json s1 = CreatOverloadedScene(eN1);
	/*json s2 = CreatOverloadedScene(eN2);
	json s3 = CreatOverloadedScene(eN3);
	json s4 = CreatOverloadedScene(eN4);
	json s5 = CreatOverloadedScene(eN5);
	json s6 = CreatOverloadedScene(eN6);
	json s7 = CreatOverloadedScene(eN7);
	json s8 = CreatOverloadedScene(eN8);
	json s9 = CreatOverloadedScene(eN9);
	json s10 = CreatOverloadedScene(eN10);
	json s11 = CreatOverloadedScene(eN11);*/

	returnedValues += IterateFakeEngine(eN1, iteration, engineIterations, s1);
	/*returnedValues += IterateFakeEngine(eN2, iteration, engineIterations, s2);
	returnedValues += IterateFakeEngine(eN3, iteration, engineIterations, s3);
	returnedValues += IterateFakeEngine(eN4, iteration, engineIterations, s4);
	returnedValues += IterateFakeEngine(eN5, iteration, engineIterations, s5);
	returnedValues += IterateFakeEngine(eN6, iteration, engineIterations, s6);
	returnedValues += IterateFakeEngine(eN7, iteration, engineIterations, s7);
	returnedValues += IterateFakeEngine(eN8, iteration, engineIterations, s8);
	returnedValues += IterateFakeEngine(eN9, iteration, engineIterations, s9);
	returnedValues += IterateFakeEngine(eN10, iteration, engineIterations, s10);
	returnedValues += IterateFakeEngine(eN11, iteration, engineIterations, s11);

	resultFile.open("fakeEngineRunConcernedNoDraw.csv");
	resultFile << returnedValues;
	resultFile.close();*/

	return SUCCEED();
}

class TestComponentClass
{
public:
	void Update(sfge::Vec2f* incrementBy)
	{
		testValue += *incrementBy;
	};

	bool valide = false;
protected:
	sfge::Vec2f testValue = { 0,0 };	
};

class TestComponentClassManager
{
public:
	void Init(const unsigned vectorSize, int entityPercentage)
	{
		m_Components.clear();
		m_ConcernedEntity.clear();

		m_Components = std::vector<TestComponentClass>{ vectorSize };

		for (int i = 0; i < m_Components.size(); i++)
		{
			int vr = rand() % 100;

			if (vr < entityPercentage)
			{
				m_Components[i].valide = true;
				m_ConcernedEntity.push_back(i);	
			}
		}

		componentPercentage = (m_ConcernedEntity.size() * 100) / vectorSize;

		std::cout << "\nThere is " << m_ConcernedEntity.size() << " valide component out of " << vectorSize;
	};

	void ClassicUpdate()
	{
		for(int i = 0; i < m_Components.size(); i++)
		{
			if (m_Components[i].valide)
				m_Components[i].Update(&increment);
		}
	};

	void ConcernedUpdate()
	{
		for (int i = 0; i < m_ConcernedEntity.size(); i++)
			m_Components[m_ConcernedEntity[i]].Update(&increment);
	};
	float componentPercentage = 0.0f;
private:
	std::vector<TestComponentClass> m_Components;
	std::vector<int> m_ConcernedEntity;
	sfge::Vec2f increment = { 0.1f,0.1f };
};

std::string UpdateManager(TestComponentClassManager* manager, int iteration)
{
	std::clock_t timer;

	timer = std::clock();

	for (int i = 0; i < iteration; i++)
	{
		manager->ClassicUpdate();
	}

	auto durationClassic =  1000.0 * (std::clock() - timer) / CLOCKS_PER_SEC;

	timer = std::clock();

	for (int i = 0; i < iteration; i++)
	{
		manager->ConcernedUpdate();
	}

	auto durationConcerned = 1000.0 *(std::clock() - timer) / CLOCKS_PER_SEC;

	std::cout << "\nClassic : " << durationClassic << " ms\tConcerned : " << durationConcerned << " ms";
	return std::to_string(durationClassic) + ";" + std::to_string(durationConcerned);
}

TEST(Component, ComponentUpdateManagement)
{
	std::string returnedValues = "Entity number;Valide component percentage;Classic treatment;Concerned treatement;\n";
	std::ofstream resultFile;

	int loopIteration = 1;
	int componentPercentage = 100;
	int testIteration = 1;	
	int i = 0;

	const unsigned entityNumber1 = 1 << 4;
	const unsigned entityNumber2 = 1 << 5;
	const unsigned entityNumber3 = 1 << 6;
	const unsigned entityNumber4 = 1 << 7;
	const unsigned entityNumber5 = 1 << 8;
	const unsigned entityNumber6 = 1 << 9;
	const unsigned entityNumber7 = 1 << 10;
	const unsigned entityNumber8 = 1 << 11;
	const unsigned entityNumber9 = 1 << 12;
	const unsigned entityNumber10 = 1 << 13;
	const unsigned entityNumber11 = 1 << 14;
	TestComponentClassManager m1;
	TestComponentClassManager m2;
	TestComponentClassManager m3;
	TestComponentClassManager m4;
	TestComponentClassManager m5;
	TestComponentClassManager m6;
	TestComponentClassManager m7;
	TestComponentClassManager m8;
	TestComponentClassManager m9;
	TestComponentClassManager m10;
	TestComponentClassManager m11;

	while (componentPercentage > 0)
	{
		std::cout << "\n---------------------Start test with " << componentPercentage << "% of valide components---------------------";
		m1.Init(entityNumber1, componentPercentage);
		m2.Init(entityNumber2, componentPercentage);
		m3.Init(entityNumber3, componentPercentage);
		m4.Init(entityNumber4, componentPercentage);
		m5.Init(entityNumber5, componentPercentage);
		m6.Init(entityNumber6, componentPercentage);
		m7.Init(entityNumber7, componentPercentage);
		m8.Init(entityNumber8, componentPercentage);
		m9.Init(entityNumber9, componentPercentage);
		m10.Init(entityNumber10, componentPercentage);
		m11.Init(entityNumber11, componentPercentage);

		returnedValues += (std::to_string(entityNumber1) + ";" + std::to_string(m1.componentPercentage) + ";" + UpdateManager(&m1, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber2) + ";" + std::to_string(m2.componentPercentage) + ";" + UpdateManager(&m2, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber3) + ";" + std::to_string(m3.componentPercentage) + ";" + UpdateManager(&m3, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber4) + ";" + std::to_string(m4.componentPercentage) + ";" + UpdateManager(&m4, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber5) + ";" + std::to_string(m5.componentPercentage) + ";" + UpdateManager(&m5, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber6) + ";" + std::to_string(m6.componentPercentage) + ";" + UpdateManager(&m6, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber7) + ";" + std::to_string(m7.componentPercentage) + ";" + UpdateManager(&m7, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber8) + ";" + std::to_string(m8.componentPercentage) + ";" + UpdateManager(&m8, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber9) + ";" + std::to_string(m9.componentPercentage) + ";" + UpdateManager(&m9, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber10) + ";" + std::to_string(m10.componentPercentage) + ";" + UpdateManager(&m10, loopIteration) + ";\n");
		returnedValues += (std::to_string(entityNumber11) + ";" + std::to_string(m11.componentPercentage) + ";" + UpdateManager(&m11, loopIteration) + ";\n");
		std::cout << "\n----------------------End test with " << componentPercentage << "% of valide components---------------------";
		i++;

		if (i % 5 == 0)
			componentPercentage -= 5;
	}

	/*resultFile.open("componentResultData.csv");
	resultFile << returnedValues;
	resultFile.close();*/

	return SUCCEED();
}