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

json CreateScene(float mapSize, float buildingFactor, float dwarfFactor, float uiElement)
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
	float mapSize = 250.0f;
	float buildingFactor = 0.05f;
	float dwarfFactor = 4.0f;
	float uiElement = 50.0f;

	sfge::Engine engine;
	engine.Init();

	engine.GetSceneManager()->LoadSceneFromJson(CreateScene(mapSize, buildingFactor, dwarfFactor, uiElement));

	engine.Start();
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