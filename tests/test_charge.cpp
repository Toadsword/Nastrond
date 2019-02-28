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

TEST(Charge, TestChargeLight)
{
	/*float mapSize = 50.0f;
	float buildingFactor = 0.05f;
	float dwarfFactor = 4.0f;
	float uiElement = 50.0f;*/

	const int engineIterations = 250;
	const int eNumber = 5000;

	std::cout << "\n\nInitilize the engine...\n";
	sfge::Engine engine;
	engine.Init();

	std::cout << "\nCreating the scene...\n";
	//engine.GetSceneManager()->LoadSceneFromJson(CreateFakeGameScene(mapSize, buildingFactor, dwarfFactor, uiElement));
	engine.GetSceneManager()->LoadSceneFromJson(CreatOverloadedScene(eNumber));

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

	std::cout << "\nEntities concerned vectors : "
		<< "\nTransform : " << std::to_string(transformManager->GetConcernedEntities().size())
		<< "\nRectTransform : " << std::to_string(rectTransformManager->GetConcernedEntities().size())
		<< "\nShape : " << std::to_string(shapeManager->GetConcernedEntities().size())
		<< "\nSprite : " << std::to_string(spriteManager->GetConcernedEntities().size())
		<< "\nAnimations : " << std::to_string(animationManager->GetConcernedEntities().size())
		<< "\nPyComponent : " << std::to_string(pyComponentManager->GetConcernedEntities().size())
		<< "\nImage : " << std::to_string(imageManager->GetConcernedEntities().size())
		<< "\nButton : " << std::to_string(buttonManager->GetConcernedEntities().size());

	std::cout << "\nStart the fake engine...\n";
	//engine.Start();

	float iterationsTimes[engineIterations];

#pragma region Fake_Engine
	int iteration = 0;

	sf::Clock globalClock;
	sf::Clock updateClock;
	sf::Clock fixedUpdateClock;
	sf::Clock graphicsUpdateClock;
	sf::Time previousFixedUpdateTime = globalClock.getElapsedTime();
	sf::Time deltaFixedUpdateTime = sf::Time();
	sf::Time dt = sf::Time();

	rmt_BindOpenGL();
	while (iteration < engineIterations && m_Window != nullptr)
	{
		//std::cout << "\n" << std::to_string(iteration);

		rmt_ScopedOpenGLSample(SFGE_Frame_GL);
		rmt_ScopedCPUSample(SFGE_Frame, 0)

			bool isFixedUpdateFrame = false;

		auto fixedUpdateTime = globalClock.getElapsedTime() - previousFixedUpdateTime;
		if (fixedUpdateTime.asSeconds() > engine.GetConfig()->fixedDeltaTime)
		{
			fixedUpdateClock.restart();
			physicsManager->FixedUpdate();
			pythonEngine->FixedUpdate();
			isFixedUpdateFrame = true;
		}

		pythonEngine->Update(dt.asSeconds());
		graphicsUpdateClock.restart();
		transformManager->Update(dt.asSeconds());

		rectTransformManager->Update(dt.asSeconds());
		uiManager->Update(dt.asSeconds());

		graphics2dManager->Update(dt.asSeconds());

		uiManager->Draw();
		graphics2dManager->Draw();

		pythonEngine->Draw();
		graphics2dManager->Display();

		const sf::Time graphicsDt = graphicsUpdateClock.getElapsedTime();
		iterationsTimes[iteration] = dt.asMicroseconds();
		dt = updateClock.restart();
		iteration++;
	}
#pragma endregion 

	float loopEngineTotalTime = 0;
	float loopEngineAverageTime;
	float fIterations = engineIterations;

	for (int i = 0; i < engineIterations; i++)
		loopEngineTotalTime += iterationsTimes[i];

	loopEngineAverageTime = fIterations / loopEngineTotalTime;

	std::cout << "\nEngine loop finished\n total time : " << loopEngineTotalTime << " | Average time " << loopEngineAverageTime;

	std::cout << "\nDelete all shapes...";
	// Suppression of all the shapes
	for(unsigned i = 0; i < eNumber; i++)
	{
		shapeManager->DestroyComponent(i);
	}

	std::cout << "\nDelete all sprites...";
	// Suppression of all the sprites
	for (unsigned i = 0; i < eNumber; i++)
	{
		spriteManager->DestroyComponent(i);
	}

	std::cout << "\nDelete all scripts...";
	// Suppression of all the python scripts
	for (unsigned i = 0; i < eNumber; i++)
	{
		pyComponentManager->DestroyComponent(i);
	}

	std::cout << "\nDelete all animations...";
	// Suppression of all the animations
	for (unsigned i = 0; i < eNumber; i++)
	{
		animationManager->DestroyComponent(i);
	}

	std::cout << "\nDelete all buttons...";
	// Suppression of all the buttons
	for (unsigned i = 0; i < eNumber; i++)
	{
		buttonManager->DestroyComponent(i);
	}

	std::cout << "\nDelete all images...";
	// Suppression of all the images
	for (unsigned i = 0; i < eNumber; i++)
	{
		imageManager->DestroyComponent(i);
	}

	std::cout << "\nDelete all transforms...";
	// Suppression of all the transforms
	for (unsigned i = 0; i < eNumber; i++)
	{
		transformManager->DestroyComponent(i);
	}

	std::cout << "\nDelete all rect transforms...";
	// Suppression of all the rect transforms
	for (unsigned i = 0; i < eNumber; i++)
	{
		rectTransformManager->DestroyComponent(i);
	}

	rmt_UnbindOpenGL();
	engine.Destroy();

	std::cout << "\nEverything has been destroyed, test success !";
	return SUCCEED();
}

/*TEST(Charge, TestChargeMediumLow)
{
	int mapSize = 250;
	int buildingFactor = 0.2;
	int dwarfFactor = 4;
	int uiElement = 50;

	sfge::Engine engine;
	engine.Init();

	engine.GetSceneManager()->LoadSceneFromJson(CreateScene(mapSize, buildingFactor, dwarfFactor, uiElement));

	engine.Start();
}

TEST(Charge, TestChargeMediumUp)
{
	int mapSize = 250;
	int buildingFactor = 0.2;
	int dwarfFactor = 4;
	int uiElement = 50;

	sfge::Engine engine;
	engine.Init();

	engine.GetSceneManager()->LoadSceneFromJson(CreateScene(mapSize, buildingFactor, dwarfFactor, uiElement));

	engine.Start();
}

TEST(Charge, TestChargeHeavy)
{
	int mapSize = 250;
	int buildingFactor = 0.2;
	int dwarfFactor = 4;
	int uiElement = 50;

	sfge::Engine engine;
	engine.Init();

	engine.GetSceneManager()->LoadSceneFromJson(CreateScene(mapSize, buildingFactor, dwarfFactor, uiElement));

	engine.Start();
}*/