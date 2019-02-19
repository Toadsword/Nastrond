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

#include <extensions/AI/behavior_tree_factory.h>

namespace sfge::ext::behavior_tree
{
Node::ptr BehaviorTreeFactory::LoadNodesFromJson(json& behaviorTreeJson, BehaviorTree* behaviorTree)
{
	if (CheckJsonParameter(behaviorTreeJson, "rootNode", json::value_t::array))
	{
		auto rootNodeJson = behaviorTreeJson["rootNode"];
		if (CheckJsonExists(rootNodeJson[0], "type"))
		{
			const NodeType nodeType = rootNodeJson[0]["type"];

			switch (nodeType)
			{
			case NodeType::LEAF:
				return AddLeafNodeFromJson(rootNodeJson[0], nullptr, behaviorTree);
			case NodeType::COMPOSITE:
				return AddCompositeNodeFromJson(rootNodeJson[0], nullptr, behaviorTree);
			case NodeType::DECORATOR:
				return AddDecoratorNodeFromJson(rootNodeJson[0], nullptr, behaviorTree);
			default:
				std::ostringstream oss;
				oss << "[Error] No type specified for root node : " << rootNodeJson[0];
				Log::GetInstance()->Error(oss.str());

				return nullptr;
			}
		}

		std::ostringstream oss;
		oss << "[Error] the type for the rootNode is missing : " << rootNodeJson[0];
		Log::GetInstance()->Error(oss.str());

		return nullptr;
	}

	std::ostringstream oss;
	oss << "[Error] the rootNode parameter is missing : " << behaviorTreeJson;
	Log::GetInstance()->Error(oss.str());

	return nullptr;
}

Node::ptr BehaviorTreeFactory::AddLeafNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree)
{
	std::cout << "Load Leaf:\n";
	std::shared_ptr<Node> leaf;

	if (behaviorTreeJson["name"] == "WaitForPathLeaf")
	{
		std::cout << "   -> Wait for path leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::WAIT_FOR_PATH_LEAF);
	}
	else if (behaviorTreeJson["name"] == "MoveToLeaf")
	{
		std::cout << "   -> Move to leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::MOVE_TO_LEAF);
	}
	else if (behaviorTreeJson["name"] == "FindPathToLeaf")
	{
		std::cout << "   -> Find path to leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::FIND_PATH_TO_LEAF);

		if (CheckJsonExists(behaviorTreeJson, "destination")) {
			static_cast<FindPathToData*>(leaf->m_Datas.get())->m_Destination = static_cast<NodeDestination>(behaviorTreeJson["destination"]);
		}
	}
	else
	{
		std::ostringstream oss;
		oss << "[Error] The name : " << behaviorTreeJson["name"] << " is not assigned to any type of leaf";
		Log::GetInstance()->Error(oss.str());

		return nullptr;
	}

	return leaf;
}

Node::ptr BehaviorTreeFactory::AddCompositeNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree)
{
	std::cout << "Load Composite:\n";
	std::shared_ptr<Node> composite;

	if (behaviorTreeJson["name"] == "SequenceComposite")
	{
		composite = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::SEQUENCE_COMPOSITE);
		std::cout << "   ->Sequence composite\n";
	}
	else if (behaviorTreeJson["name"] == "SelectorComposite")
	{
		composite = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::SELECTOR_COMPOSITE);
		std::cout << "   ->Selector composite\n";
	}
	else
	{
		std::ostringstream oss;
		oss << "[Error] The name : " << behaviorTreeJson["name"] << " is not assigned to any type of composite";
		Log::GetInstance()->Error(oss.str());

		return nullptr;
	}

	if (CheckJsonExists(behaviorTreeJson, "childs"))
	{
		for (auto& childJson : behaviorTreeJson["childs"])
		{
			if (CheckJsonExists(childJson, "type"))
			{
				const NodeType nodeType = childJson["type"];

				switch (nodeType)
				{
				case NodeType::NONE:
				{
					std::ostringstream oss;
					oss << "[Error] No type specified for root node : " << childJson;
					Log::GetInstance()->Error(oss.str());
				}
				break;
				case NodeType::LEAF:
					static_cast<CompositeData*>(composite->m_Datas.get())->m_Children.push_back(AddLeafNodeFromJson(childJson, composite, behaviorTree));
					break;
				case NodeType::COMPOSITE:
					static_cast<CompositeData*>(composite->m_Datas.get())->m_Children.push_back(AddCompositeNodeFromJson(childJson, composite, behaviorTree));
					break;
				case NodeType::DECORATOR:
					static_cast<CompositeData*>(composite->m_Datas.get())->m_Children.push_back(AddDecoratorNodeFromJson(childJson, composite, behaviorTree));
					break;
				default:;
				}
			}
		}
	}

	return composite;
}

Node::ptr BehaviorTreeFactory::AddDecoratorNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree)
{
	std::cout << "Load Decorator:\n";
	std::shared_ptr<Node> decorator;

	if (behaviorTreeJson["name"] == "RepeatUntilFailDecorator")
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR);
		std::cout << "   ->Repeat until fail decorator\n";
	}
	else if (behaviorTreeJson["name"] == "InverterDecorator")
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::INVERTER_DECORATOR);
		std::cout << "   ->Inverter decorator\n";
	}
	else if (behaviorTreeJson["name"] == "RepeaterDecorator")
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::REPEATER_DECORATOR);
		std::cout << "   ->Repeater decorator\n";

		if (CheckJsonExists(behaviorTreeJson, "limit"))
		{
			static_cast<RepeaterData*>(decorator->m_Datas.get())->m_Limit = behaviorTreeJson["limit"];
		}
	}
	else if (behaviorTreeJson["name"] == "SucceederDecorator")
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::SUCCEEDER_DECORATOR);
		std::cout << "   ->Succeeder decorator\n";
	}
	else
	{
		std::ostringstream oss;
		oss << "[Error] The name : " << behaviorTreeJson["name"] << " is not assigned to any type of decorator";
		Log::GetInstance()->Error(oss.str());

		return nullptr;
	}

	if (CheckJsonExists(behaviorTreeJson, "childs"))
	{
		for (auto& childJson : behaviorTreeJson["childs"])
		{
			if (CheckJsonExists(childJson, "type"))
			{
				const NodeType nodeType = childJson["type"];

				switch (nodeType)
				{
				case NodeType::LEAF:
					static_cast<DecoratorData*>(decorator->m_Datas.get())->m_Child = AddLeafNodeFromJson(childJson, decorator, behaviorTree);
					break;
				case NodeType::COMPOSITE:
					static_cast<DecoratorData*>(decorator->m_Datas.get())->m_Child = AddCompositeNodeFromJson(childJson, decorator, behaviorTree);
					break;
				case NodeType::DECORATOR:
					static_cast<DecoratorData*>(decorator->m_Datas.get())->m_Child = AddDecoratorNodeFromJson(childJson, decorator, behaviorTree);
					break;
				default:
					std::ostringstream oss;
					oss << "[Error] No type specified for root node : " << childJson;
					Log::GetInstance()->Error(oss.str());;
				}
			}
		}
	}

	return decorator;
}
}
