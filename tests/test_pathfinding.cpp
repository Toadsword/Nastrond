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
#include <utility/priority_queue.h>

#include <engine/engine.h>
#include <engine/scene.h>
#include <engine/component.h>

#include <python/python_engine.h>

#include <extensions/behaviour_tree.h>
#include "extensions/python_extensions.h"

TEST(AI, PriorityQueue)
{
	sfge::PriorityQueue<std::string, int> testList;

	testList.Put("0 => 10", 10);
	testList.Put("1 => 0", 0);
	testList.Put("2 => 3", 3);
	testList.Put("4 => 1", 1);

	while(!testList.Empty()) {
		std::cout << testList.Get() << "\n";
	}
}

TEST(AI, NavigationGraphManager) 
{
	sfge::Engine engine;
	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();

	engine.Init(std::move(initConfig));
	json sceneJson = {
		{ "name", "Test Navigation Graph SystemCPP" } };
	json systemJson = {
		{ "systemClassName", "NavigationGraphManager" }
	};

	sceneJson["systems"] = json::array({ systemJson });
	auto* sceneManager = engine.GetSceneManager();
	sceneManager->LoadSceneFromJson(sceneJson);

	engine.Start();
}

TEST(AI,DwarfMovement)
{
	sfge::Engine engine;

	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity.SetZero();
	initConfig->devMode = false;
	initConfig->maxFramerate = 0;
	engine.Init(std::move(initConfig));

	const auto config = engine.GetConfig();

	json sceneJson = {
		{ "name", "Dwarf Movements" } };
	json systemJsonNavigation = {
		{ "systemClassName", "NavigationGraphManager" }
	}; 
	json systemJsonDwarf = {
		{ "systemClassName", "DwarfManager" }
	};

	sceneJson["systems"] = json::array({ systemJsonNavigation, systemJsonDwarf });

	auto* sceneManager = engine.GetSceneManager();
	sceneManager->LoadSceneFromJson(sceneJson);

	engine.Start();
}

TEST(AI, BehaviourTree)
{
	sfge::Engine engine;

	std::unique_ptr<sfge::Configuration> initConfig = std::make_unique<sfge::Configuration>();
	initConfig->gravity.SetZero();
	initConfig->devMode = false;
	initConfig->maxFramerate = 0;
	engine.Init(std::move(initConfig));

	auto* sceneManager = engine.GetSceneManager();

	json sceneJson = {
		{ "name", "Behaviour tree" } };
	json systemJsonBehaviourTree = {
		{ "systemClassName", "BehaviourTree" }
	};

	sceneJson["systems"] = json::array({systemJsonBehaviourTree});

	sceneManager->LoadSceneFromJson(sceneJson);

	auto behaviourTree = engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behaviour_tree::BehaviourTree>("BehaviourTree");

	//Repeater
	auto repeater = std::make_shared<sfge::ext::behaviour_tree::Repeater>(behaviourTree, 0);
	behaviourTree->SetRootNode(repeater);

	//Sequence
	auto sequence = std::make_shared<sfge::ext::behaviour_tree::Sequence>(behaviourTree);
	sequence->parentNode = repeater;
	repeater->SetChild(sequence);

	//Leaf 1
	auto debugLeaf = std::make_shared<sfge::ext::behaviour_tree::DebugUpdateLeaf>();
	debugLeaf->parentNode = sequence;
	sequence->AddChild(debugLeaf);

	//Leaf 2
	auto debugLeaf2 = std::make_shared<sfge::ext::behaviour_tree::DebugUpdateLeaf2>();
	debugLeaf2->parentNode = sequence;
	sequence->AddChild(debugLeaf2);

	//Selector
	auto selector = std::make_shared<sfge::ext::behaviour_tree::Selector>(behaviourTree);
	selector->parentNode = sequence;
	sequence->AddChild(selector);

	//Inverter
	auto inverter = std::make_shared<sfge::ext::behaviour_tree::Inverter>(behaviourTree);
	inverter->parentNode = selector;
	selector->AddChild(inverter);

	//Leaf 3
	auto debugLeaf3 = std::make_shared<sfge::ext::behaviour_tree::DebugUpdateLeaf3>();
	debugLeaf3->parentNode = inverter;
	inverter->SetChild(debugLeaf3);

	//Repeater 2
	auto repeater2 = std::make_shared<sfge::ext::behaviour_tree::Repeater>(behaviourTree, 4);
	repeater2->parentNode = selector;
	selector->AddChild(repeater2);

	//Succeeder
	auto succeeder = std::make_shared<sfge::ext::behaviour_tree::Succeeder>(behaviourTree);
	succeeder->parentNode = repeater2;
	repeater2->SetChild(succeeder);

	//Leaf 4
	auto debugLeaf4 = std::make_shared<sfge::ext::behaviour_tree::DebugUpdateLeaf4>();
	debugLeaf4->parentNode = succeeder;
	succeeder->SetChild(debugLeaf4);
	
	auto* entityManager = engine.GetEntityManager();
	const auto newEntity = entityManager->CreateEntity(0);

	std::vector<Entity> entities = std::vector<Entity>();
	entities.push_back(newEntity);

	behaviourTree->SetEntities(&entities);

	engine.Start();
}