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
#include <extensions/AI/behavior_tree.h>
#include <extensions/AI/behavior_tree_nodes_extension.h>

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

	sceneManager->LoadSceneFromJson(sceneJson);

	auto behaviourTree = engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behavior_tree::BehaviorTree>("BehaviorTree");
	
	//Repeater
	auto repeater = std::make_shared<sfge::ext::behavior_tree::RepeaterDecorator>(behaviourTree, nullptr, 0);
	behaviourTree->SetRootNode(repeater);

	//Sequence
	auto sequence = std::make_shared<sfge::ext::behavior_tree::SequenceComposite>(behaviourTree, repeater);
	repeater->SetChild(sequence);
	
	//Find random path
	auto findPath = std::make_shared<sfge::ext::behavior_tree::FindRandomPathLeaf>(behaviourTree, sequence);
	sequence->AddChild(findPath);

	//Has path
	auto waitForPath = std::make_shared<sfge::ext::behavior_tree::WaitForPathLeaf>(behaviourTree, sequence);
	sequence->AddChild(waitForPath);

	//Follow path
	auto followPath = std::make_shared<sfge::ext::behavior_tree::MoveToLeaf>(behaviourTree, sequence);
	sequence->AddChild(followPath);
	
	engine.Start();
}

TEST(AI, BehaviourTreeLoadRandomPathWithFactory)
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

	sceneManager->LoadSceneFromJson(sceneJson);

	auto behaviourTree = engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behavior_tree::BehaviorTree>("BehaviorTree");

	//Repeater
	auto repeater = sfge::ext::behavior_tree::NodeFactory::GetFactory("RepeaterDecorator")->Create(behaviourTree, nullptr);
	behaviourTree->SetRootNode(repeater);

	//Sequence
	auto sequence = sfge::ext::behavior_tree::NodeFactory::GetFactory("SequenceComposite")->Create(behaviourTree, repeater);
	dynamic_cast<sfge::ext::behavior_tree::DecoratorNode*>(repeater.get())->SetChild(sequence);

	//Find random path
	auto findPath = sfge::ext::behavior_tree::NodeFactory::GetFactory("FindRandomPathLeaf")->Create(behaviourTree, sequence);
	dynamic_cast<sfge::ext::behavior_tree::CompositeNode*>(sequence.get())->AddChild(findPath);

	//Has path
	auto waitForPath = sfge::ext::behavior_tree::NodeFactory::GetFactory("WaitForPathLeaf")->Create(behaviourTree, sequence);
	dynamic_cast<sfge::ext::behavior_tree::CompositeNode*>(sequence.get())->AddChild(waitForPath);

	//Follow path
	auto followPath = sfge::ext::behavior_tree::NodeFactory::GetFactory("MoveToLeaf")->Create(behaviourTree, sequence);
	dynamic_cast<sfge::ext::behavior_tree::CompositeNode*>(sequence.get())->AddChild(followPath);

	engine.Start();
}

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

	sceneManager->LoadSceneFromJson(sceneJson);

	auto behaviourTree = engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behavior_tree::BehaviorTree>("BehaviorTree");

	const auto sceneJsonPtr = sfge::LoadJson("data/behavior_tree/random_path.asset");

	behaviourTree->LoadNodesFromJson(*sceneJsonPtr);

	engine.Start();
}