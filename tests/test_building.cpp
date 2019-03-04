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

#include <extensions/dwarf_manager.h>
#include <extensions/AI/behavior_tree.h>
#include <extensions/AI/behavior_tree_factory.h>


TEST(Building, TestScene)
{
	sfge::Engine engine;
	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity.SetZero();
	initConfig->devMode = false;
	initConfig->maxFramerate = 0;
	engine.Init(std::move(initConfig));

	json sceneJson;
	json entityJsonTilemap;
	json tilemapJson;

	tilemapJson["type"] = static_cast<int>(sfge::ComponentType::TILEMAP);
	tilemapJson["is_isometric"] = true;
	tilemapJson["layer"] = 1;
	tilemapJson["tile_size"] = json::array({64, 32});
	tilemapJson["map_size"] = json::array({10, 10});
	tilemapJson["map"] = json::array({
		json::array({1, 2, 2, 2, 2, 6, 9, 2, 2, 2}),
		json::array({1, 2, 3, 9, 2, 9, 9, 8, 9, 2}),
		json::array({1, 2, 9, 9, 2, 2, 1, 9, 9, 2}),
		json::array({2, 2, 2, 8, 9, 2, 2, 2, 2, 2}),
		json::array({2, 1, 1, 9, 9, 2, 8, 9, 1, 1}),
		json::array({2, 4, 9, 1, 1, 2, 9, 9, 1, 1}),
		json::array({2, 9, 9, 2, 2, 2, 2, 2, 2, 1}),
		json::array({2, 2, 2, 8, 9, 2, 1, 1, 2, 1}),
		json::array({7, 9, 1, 9, 9, 5, 9, 1, 8, 9}),
		json::array({9, 9, 1, 1, 1, 9, 9, 1, 9, 9})
	});
	entityJsonTilemap["components"] = json::array({tilemapJson});
	entityJsonTilemap["name"] = "Tilemap";

	const auto config = engine.GetConfig();

	sceneJson = {
		{"name", "Test Tilemap Building"}
	};

	json systemJsonDwellingManager = {
		{"systemClassName", "DwellingManager"}
	};

	json systemJsonProductionBuildingManager = {
		{"systemClassName", "ProductionBuildingManager"}
	};

	json systemJsonForgeManager = {
		{"systemClassName", "ForgeManager"}
	};

	json systemJsonWarehouseManager = {
		{"systemClassName", "WarehouseManager"}
	};

	json systemJsonRoadgManager = {
		{"systemClassName", "RoadManager"}
	};

	json systemJsonBuildingManager = {
		{"systemClassName", "BuildingManager"}
	};

	json systemJsonBuildingConstructor = {
		{"systemClassName", "BuildingConstructor"}
	};

	json systemJsonBehaviourTree = {
		{"systemClassName", "BehaviorTree"}
	};
	json systemJsonNavigation = {
		{"systemClassName", "NavigationGraphManager"}
	};
	json systemJsonDwarf = {
		{"systemClassName", "DwarfManager"}
	};
	sceneJson["systems"] = json::array({
		systemJsonDwellingManager, systemJsonProductionBuildingManager, systemJsonForgeManager,
		systemJsonWarehouseManager, systemJsonRoadgManager, systemJsonBuildingManager, systemJsonBuildingConstructor,
		systemJsonBehaviourTree, systemJsonNavigation, systemJsonDwarf
	});

	json entityJsonCamera;
	entityJsonCamera["name"] = "Camera";
	json CameraJson;
	CameraJson["type"] = static_cast<int>(sfge::ComponentType::CAMERA);
	json pyCameraJson;
	pyCameraJson["script_path"] = "scripts/camera_manager.py";
	pyCameraJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);
	entityJsonCamera["components"] = json::array({CameraJson, pyCameraJson});

	sceneJson["entities"] = json::array({entityJsonCamera, entityJsonTilemap});

	sfge::SceneManager* sceneManager = engine.GetSceneManager();

	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);

	//Load behavior tree
	auto behaviourTree = engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behavior_tree::
		BehaviorTree>("BehaviorTree");
	const auto sceneJsonPtr = sfge::LoadJson("data/behavior_tree/CompleteBehaviorTree.asset");
	//const auto sceneJsonPtr = sfge::LoadJson("data/behavior_tree/random_path.asset");
	behaviourTree->SetRootNode(
		sfge::ext::behavior_tree::BehaviorTreeUtility::LoadNodesFromJson(*sceneJsonPtr, behaviourTree));

	auto dwarfManager = engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::DwarfManager>("DwarfManager");
	for(auto i = 0; i < 25; i++)
	{
		dwarfManager->InstantiateDwarf(sfge::Vec2f(0, 0));
	}

	engine.Start();
}

TEST(Building, SpawnBuilding)
{
	sfge::Engine engine;
	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity.SetZero();
	initConfig->devMode = false;
	initConfig->maxFramerate = 0;
	engine.Init(std::move(initConfig));

	json sceneJson;

	const auto config = engine.GetConfig();

	sceneJson = {
		{ "name", "Test Tilemap Building" } };

	json systemJsonDwellingManager = {
		{ "systemClassName", "DwellingManager" } };

	json systemJsonProductionBuildingManager = {
		{ "systemClassName", "ProductionBuildingManager" } };

	json systemJsonForgeManager = {
		{ "systemClassName", "ForgeManager" } };

	json systemJsonWarehouseManager = {
		{ "systemClassName", "WarehouseManager" } };

	json systemJsonBuildingManager = {
		{ "systemClassName", "BuildingManager" } };

	sceneJson["systems"] = json::array({ systemJsonDwellingManager, systemJsonProductionBuildingManager, systemJsonForgeManager, systemJsonWarehouseManager, systemJsonBuildingManager });

	sfge::SceneManager* sceneManager = engine.GetSceneManager();

	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);

	engine.Start();
}
