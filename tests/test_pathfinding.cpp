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

#include <extensions/python_extensions.h>
#include <extensions/behaviour_tree_nodes.h>

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

TEST(AI, BehaviourTreeRandomPath)
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

	sceneManager->LoadSceneFromJson(sceneJson);

	auto behaviourTree = engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behaviour_tree::BehaviourTree>("BehaviourTree");
	
	//Repeater
	auto repeater = std::make_shared<sfge::ext::behaviour_tree::Repeater>(behaviourTree, nullptr, 0);
	behaviourTree->SetRootNode(repeater);

	//Sequence
	auto sequence = std::make_shared<sfge::ext::behaviour_tree::Sequence>(behaviourTree, repeater);
	repeater->SetChild(sequence);
	
	//Find random path
	auto findPath = std::make_shared<sfge::ext::behaviour_tree::FindRandomPath>(behaviourTree, sequence);
	sequence->AddChild(findPath);

	//Has path
	auto waitForPath = std::make_shared<sfge::ext::behaviour_tree::WaitForPath>(behaviourTree, sequence);
	sequence->AddChild(waitForPath);

	//Follow path
	auto followPath = std::make_shared<sfge::ext::behaviour_tree::MoveToLeaf>(behaviourTree, sequence);
	sequence->AddChild(followPath);
	
	engine.Start();
}