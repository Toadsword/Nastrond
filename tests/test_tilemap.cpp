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
#include <graphics/tilemap.h>
#include <utility/json_utility.h>
#include <gtest/gtest.h>
#include <fstream>

TEST(Tilemap, TestLoadTilemap)
{
	sfge::Engine engine;

	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->devMode = false;
	initConfig->maxFramerate = 0;
	engine.Init(std::move(initConfig));
	
	json sceneJson;
	json entityJson;
	json tilemapJson;

	tilemapJson["type"] = static_cast<int>(sfge::ComponentType::TILEMAP);
	tilemapJson["reference_path"] = "./data/tilemap/nastrond_tiles.asset";
	tilemapJson["is_isometric"] = true;
	tilemapJson["layer"] = 1;
	tilemapJson["tile_size"] = json::array({512, 256});
	tilemapJson["map_size"] = json::array({10, 10});
	tilemapJson["map"] = json::array({
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1})
	});
	entityJson["components"] = json::array({ tilemapJson });
	entityJson["name"] = "TilemapTest";

	json entityJson2;
	json CameraJson;
	CameraJson["type"] = static_cast<int>(sfge::ComponentType::CAMERA);
	json pyCameraJson;
	pyCameraJson["script_path"] = "scripts/camera_manager.py";
	pyCameraJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);
	entityJson2["components"] = json::array({ CameraJson, pyCameraJson });

	sceneJson["entities"] = json::array({ entityJson2, entityJson });
	sceneJson["name"] = "Test Tilemap";
	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);
	

	//engine.GetSceneManager()->LoadSceneFromPath("data/scenes/saved_scene1.scene");

    engine.Start();
}