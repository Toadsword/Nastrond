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

#include <engine/engine.h>
#include <engine/config.h>
#include <engine/scene.h>
#include <utility/json_utility.h>
#include <gtest/gtest.h>
#include <graphics/shape2d.h>

TEST(TestScene, TestLoadScene)
{
	sfge::Engine engine;
	engine.Init();

	auto* sceneManager = engine.GetSceneManager();
	sceneManager->LoadSceneFromPath ("data/scenes/test.scene");


	engine.Start();

}

TEST(TestScene, TestLotOfEntites)
{
	const int entityNmb = 10'000;
	
	sfge::Engine engine;
	engine.Init();
	const auto config = engine.GetConfig();
	json sceneJson = {
		{"name", "Lot Of Entities Scene"}};
	json entitiesArray = json::array();
	for(int i = 0; i < entityNmb;i++)
	{
		//Adding transform
		json transformJson = 
		{
			{"position", {rand() % config->screenResolution.x, rand() % config->screenResolution.y}},
			{"angle", {rand() % 360}},
			{"type", static_cast<int>(sfge::ComponentType::TRANSFORM2D)},
		};

		json rectShapeJson = 
		{
			{"size", {10,10}},
			{"type", static_cast<int>(sfge::ComponentType::SHAPE2D)},
			{"shape_type", static_cast<int>(sfge::ShapeType ::RECTANGLE)}
		};

		std::ostringstream oss;
		oss << "Entity " << i;
		const json entityJson = 
		{
			{"name", oss.str()},
			{"components", {
				transformJson, rectShapeJson
			}
			}
		};
		entitiesArray.push_back(entityJson);
	}
	sceneJson["entities"] = entitiesArray;
	auto* sceneManager = engine.GetSceneManager();
	sceneManager->LoadSceneFromJson(sceneJson);

	engine.Start();
}
