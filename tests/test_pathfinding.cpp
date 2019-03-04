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

#include <gtest/gtest.h>

#include <engine/engine.h>
#include <engine/scene.h>
#include <engine/component.h>

#include <python/python_engine.h>

#include <extensions/python_extensions.h>
#include <extensions/AI/behavior_tree.h>
#include <extensions/AI/behavior_tree_factory.h>

TEST(AI, BehaviourTreeLoadRandomPathFromJson)
{
	sfge::Engine engine;

	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity.SetZero();
	initConfig->devMode = false;
	initConfig->maxFramerate = 0;
	engine.Init(std::move(initConfig));

	auto* sceneManager = engine.GetSceneManager();

	json sceneJson = {
		{ "name", "Behavior tree" } };
	json systemJsonBehaviourTree = {
		{ "systemClassName", "BehaviorTree" }
	};
	json systemJsonNavigation = {
		{ "systemClassName", "NavigationGraphManager" }
	};
	json systemJsonDwarf = {
		{ "systemClassName", "DwarfManager" }
	};
	json systemJsonDwelling = {
		{ "systemClassName", "DwellingManager"}
	};

	sceneJson["systems"] = json::array({ systemJsonBehaviourTree,  systemJsonNavigation, systemJsonDwarf, systemJsonDwelling });

	json entityCamera;
	json CameraJson;
	CameraJson["type"] = static_cast<int>(sfge::ComponentType::CAMERA);
	json pyCameraJson;
	pyCameraJson["script_path"] = "scripts/camera_manager.py";
	pyCameraJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);
	entityCamera["components"] = json::array({ CameraJson, pyCameraJson });


	sceneJson["entities"] = json::array({ entityCamera});

	sceneManager->LoadSceneFromJson(sceneJson);

	auto behaviourTree = engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behavior_tree::BehaviorTree>("BehaviorTree");

	const auto sceneJsonPtr = sfge::LoadJson("data/behavior_tree/random_path.asset");

	behaviourTree->SetRootNode(sfge::ext::behavior_tree::BehaviorTreeUtility::LoadNodesFromJson(*sceneJsonPtr, behaviourTree));

	engine.Start();

	system("pause");
}

TEST(AI, PathfindingTilemap)
{
	sfge::Engine engine;

	auto initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity.SetZero();
	initConfig->devMode = false;
	initConfig->maxFramerate = 0;
	engine.Init(std::move(initConfig));

	auto* sceneManager = engine.GetSceneManager();

	//System
	json systemJsonBehaviourTree;
	systemJsonBehaviourTree["systemClassName"] = "BehaviorTree";

	json systemJsonNavigation;
	systemJsonNavigation["systemClassName"] = "NavigationGraphManager";

	json systemJsonDwarf;
	systemJsonDwarf["systemClassName"] = "DwarfManager";

	json systemJsonDwelling;
	systemJsonDwelling["systemClassName"] = "DwellingManager";

	//Camera entity
	json cameraJson;
	cameraJson["type"] = static_cast<int>(sfge::ComponentType::CAMERA);

	json pyCameraJson;
	pyCameraJson["script_path"] = "scripts/camera_manager.py";
	pyCameraJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);

	json entityCamera;
	entityCamera["name"] = "CameraEntity";
	entityCamera["components"] = json::array({ cameraJson, pyCameraJson });

	//Tilemap entity
	json tilemapJson;

	tilemapJson["type"] = static_cast<int>(sfge::ComponentType::TILEMAP);
	tilemapJson["is_isometric"] = true;
	tilemapJson["layer"] = 1;
	tilemapJson["tile_size"] = json::array({ 64, 32 });
	tilemapJson["map_size"] = json::array({ 25, 25 });
	tilemapJson["map"] = json::array({
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}),
		json::array({1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1})
		});

	json tilemapEntity;
	tilemapEntity["name"] = "TilemapEntity";
	tilemapEntity["components"] = json::array({ tilemapJson });

	//Scene json
	json sceneJson;
	sceneJson["name"] = "Path finding in isometric tilemap";
	sceneJson["entities"] = json::array({ entityCamera, tilemapEntity });
	sceneJson["systems"] = json::array({ systemJsonBehaviourTree,  systemJsonNavigation, systemJsonDwarf, systemJsonDwelling });

	//Load Scene
	sceneManager->LoadSceneFromJson(sceneJson);

	//Load behavior tree
	auto behaviourTree = engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behavior_tree::BehaviorTree>("BehaviorTree");
	const auto sceneJsonPtr = sfge::LoadJson("data/behavior_tree/random_path.asset");
	behaviourTree->SetRootNode(sfge::ext::behavior_tree::BehaviorTreeUtility::LoadNodesFromJson(*sceneJsonPtr, behaviourTree));

	//Start engine
	engine.Start();
}