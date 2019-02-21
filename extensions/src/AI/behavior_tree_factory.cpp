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
#include <utility/log.h>

namespace sfge::ext::behavior_tree
{
Node::ptr BehaviorTreeUtility::LoadNodesFromJson(json& behaviorTreeJson, BehaviorTree* behaviorTree)
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

void BehaviorTreeUtility::SaveBehaviorTreeToJson(const Node::ptr& node, const std::string& filePath)
{
	std::ofstream outfile(filePath);
	outfile << "{\"rootNode\": [";
	switch (node->nodeType)
	{
	case Node::NodeType::SEQUENCE_COMPOSITE:
	case Node::NodeType::SELECTOR_COMPOSITE:
		outfile << CompositeNodeToString(node);
		break;
	case Node::NodeType::REPEATER_DECORATOR:
	case Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
	case Node::NodeType::SUCCEEDER_DECORATOR:
	case Node::NodeType::INVERTER_DECORATOR:
		outfile << DecoratorNodeToString(node);
		break;
	case Node::NodeType::WAIT_FOR_PATH_LEAF:
	case Node::NodeType::MOVE_TO_LEAF: 
	case Node::NodeType::HAS_DWELLING_LEAF:
	case Node::NodeType::SET_DWELLING_LEAF: 
	case Node::NodeType::ENTER_DWELLING_LEAF: 
	case Node::NodeType::EXIT_DWELLING_LEAF:
	case Node::NodeType::ENTER_WORKING_PLACE_LEAF: 
	case Node::NodeType::EXIT_WORKING_PLACE_LEAF:
	case Node::NodeType::HAS_JOB_LEAF:
	case Node::NodeType::HAS_STATIC_JOB_LEAF: 
	case Node::NodeType::ASSIGN_JOB_LEAF:
	case Node::NodeType::IS_DAY_TIME_LEAF: 
	case Node::NodeType::IS_NIGHT_TIME_LEAF: 
	case Node::NodeType::WAIT_DAY_TIME_LEAF:
	case Node::NodeType::WAIT_NIGHT_TIME_LEAF: 
	case Node::NodeType::ASK_INVENTORY_TASK_LEAF: 
	case Node::NodeType::TAKE_RESOURCE_LEAF:
	case Node::NodeType::PUT_RESOURCE_LEAF: 
	case Node::NodeType::FIND_PATH_TO_LEAF: 
		outfile << LeafNodeToString(node);
		break;
	default: ;
	}
	outfile << " ]}";

	outfile.close();
}

std::string BehaviorTreeUtility::NodeTypeToString(const Node::NodeType nodeType)
{
	switch (nodeType)
	{
	case Node::NodeType::SEQUENCE_COMPOSITE:
		return "Sequence";
	case Node::NodeType::SELECTOR_COMPOSITE:
		return "Selector";
	case Node::NodeType::REPEATER_DECORATOR:
		return "Repeater";
	case Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
		return "Repeat until fail";
	case Node::NodeType::SUCCEEDER_DECORATOR:
		return "Succeeder";
	case Node::NodeType::INVERTER_DECORATOR:
		return "Inverter";
	case Node::NodeType::WAIT_FOR_PATH_LEAF:
		return "Wait for path";
	case Node::NodeType::MOVE_TO_LEAF:
		return "Move to";
	case Node::NodeType::HAS_DWELLING_LEAF:
		return "Has dwelling";
	case Node::NodeType::SET_DWELLING_LEAF:
		return "Set dwelling";
	case Node::NodeType::ENTER_DWELLING_LEAF:
		return "Enter dwelling";
	case Node::NodeType::EXIT_DWELLING_LEAF:
		return "Exit dwelling";
	case Node::NodeType::ENTER_WORKING_PLACE_LEAF:
		return "Enter working place";
	case Node::NodeType::EXIT_WORKING_PLACE_LEAF:
		return "Exit working place";
	case Node::NodeType::HAS_JOB_LEAF:
		return "Has job";
	case Node::NodeType::HAS_STATIC_JOB_LEAF:
		return "Has static job";
	case Node::NodeType::ASSIGN_JOB_LEAF:
		return "Assign job";
	case Node::NodeType::IS_DAY_TIME_LEAF:
		return "Is day time";
	case Node::NodeType::IS_NIGHT_TIME_LEAF:
		return "Is night time";
	case Node::NodeType::WAIT_DAY_TIME_LEAF:
		return "Wait day time";
	case Node::NodeType::WAIT_NIGHT_TIME_LEAF:
		return "Wait night time";
	case Node::NodeType::ASK_INVENTORY_TASK_LEAF:
		return "Ask inventory task";
	case Node::NodeType::TAKE_RESOURCE_LEAF:
		return "Take resource";
	case Node::NodeType::PUT_RESOURCE_LEAF:
		return "Put resource";
	case Node::NodeType::FIND_PATH_TO_LEAF:
		return "Find path to";
	default:
		std::cout << "ERROR\n";
		return "";
	}
}

Node::ptr BehaviorTreeUtility::AddLeafNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree)
{
	std::cout << "Load Leaf:\n";
	std::shared_ptr<Node> leaf;

	if (behaviorTreeJson["name"] == NodeTypeToString(Node::NodeType::WAIT_FOR_PATH_LEAF))
	{
		std::cout << "   -> Wait for path leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::WAIT_FOR_PATH_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(Node::NodeType::MOVE_TO_LEAF))
	{
		std::cout << "   -> Move to leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::MOVE_TO_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(Node::NodeType::FIND_PATH_TO_LEAF))
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

Node::ptr BehaviorTreeUtility::AddCompositeNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree)
{
	std::cout << "Load Composite:\n";
	std::shared_ptr<Node> composite;

	if (behaviorTreeJson["name"] == NodeTypeToString(Node::NodeType::SEQUENCE_COMPOSITE))
	{
		composite = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::SEQUENCE_COMPOSITE);
		std::cout << "   ->Sequence composite\n";
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(Node::NodeType::SELECTOR_COMPOSITE))
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

Node::ptr BehaviorTreeUtility::AddDecoratorNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree)
{
	std::cout << "Load Decorator:\n";
	std::shared_ptr<Node> decorator;

	if (behaviorTreeJson["name"] == NodeTypeToString(Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR))
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR);
		std::cout << "   ->Repeat until fail decorator\n";
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(Node::NodeType::INVERTER_DECORATOR))
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::INVERTER_DECORATOR);
		std::cout << "   ->Inverter decorator\n";
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(Node::NodeType::REPEATER_DECORATOR))
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, Node::NodeType::REPEATER_DECORATOR);
		std::cout << "   ->Repeater decorator\n";

		if (CheckJsonExists(behaviorTreeJson, "limit"))
		{
			static_cast<RepeaterData*>(decorator->m_Datas.get())->m_Limit = behaviorTreeJson["limit"];
		}
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(Node::NodeType::SUCCEEDER_DECORATOR))
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

std::string BehaviorTreeUtility::LeafNodeToString(const Node::ptr& node)
{
	std::string outString = "{";
	outString += R"("name" : ")" + NodeTypeToString(node->nodeType) + "\",";

	switch (node->nodeType)
	{
	case Node::NodeType::WAIT_FOR_PATH_LEAF:
	case Node::NodeType::MOVE_TO_LEAF:
	case Node::NodeType::HAS_DWELLING_LEAF:
	case Node::NodeType::SET_DWELLING_LEAF:
	case Node::NodeType::ENTER_DWELLING_LEAF:
	case Node::NodeType::EXIT_DWELLING_LEAF:
	case Node::NodeType::ENTER_WORKING_PLACE_LEAF:
	case Node::NodeType::EXIT_WORKING_PLACE_LEAF:
	case Node::NodeType::HAS_JOB_LEAF:
	case Node::NodeType::HAS_STATIC_JOB_LEAF:
	case Node::NodeType::ASSIGN_JOB_LEAF:
	case Node::NodeType::IS_DAY_TIME_LEAF:
	case Node::NodeType::IS_NIGHT_TIME_LEAF:
	case Node::NodeType::WAIT_DAY_TIME_LEAF:
	case Node::NodeType::WAIT_NIGHT_TIME_LEAF:
	case Node::NodeType::ASK_INVENTORY_TASK_LEAF:
	case Node::NodeType::TAKE_RESOURCE_LEAF:
	case Node::NodeType::PUT_RESOURCE_LEAF:
		break;
	case Node::NodeType::FIND_PATH_TO_LEAF:
		outString += "\"destination\" : " + std::to_string(
			static_cast<int>(static_cast<FindPathToData*>(node->m_Datas.get())->m_Destination)) + ",";
		break;
	default: ;
	}
	outString += "\"type\" : " + std::to_string(static_cast<int>(NodeType::LEAF));

	outString += "}";
	return outString;
}

std::string BehaviorTreeUtility::CompositeNodeToString(const Node::ptr& node)
{
	std::string outString = "{";
	outString += R"("name" : ")" + NodeTypeToString(node->nodeType) + "\",";
	
	outString += "\"type\" : " + std::to_string(static_cast<int>(NodeType::COMPOSITE));

	auto* data = static_cast<CompositeData*>(node->m_Datas.get());

	if (!data->m_Children.empty())
	{
		outString += ",";
		outString += "\"childs\" : [";

		for (auto child : data->m_Children)
		{
			switch (child->nodeType)
			{
			case Node::NodeType::SEQUENCE_COMPOSITE:
			case Node::NodeType::SELECTOR_COMPOSITE:
				outString += CompositeNodeToString(child);
				break;
			case Node::NodeType::REPEATER_DECORATOR:
			case Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
			case Node::NodeType::SUCCEEDER_DECORATOR:
			case Node::NodeType::INVERTER_DECORATOR:
				outString += DecoratorNodeToString(child);
				break;
			case Node::NodeType::WAIT_FOR_PATH_LEAF:
			case Node::NodeType::MOVE_TO_LEAF:
			case Node::NodeType::HAS_DWELLING_LEAF:
			case Node::NodeType::SET_DWELLING_LEAF:
			case Node::NodeType::ENTER_DWELLING_LEAF:
			case Node::NodeType::EXIT_DWELLING_LEAF:
			case Node::NodeType::ENTER_WORKING_PLACE_LEAF:
			case Node::NodeType::EXIT_WORKING_PLACE_LEAF:
			case Node::NodeType::HAS_JOB_LEAF:
			case Node::NodeType::HAS_STATIC_JOB_LEAF:
			case Node::NodeType::ASSIGN_JOB_LEAF:
			case Node::NodeType::IS_DAY_TIME_LEAF:
			case Node::NodeType::IS_NIGHT_TIME_LEAF:
			case Node::NodeType::WAIT_DAY_TIME_LEAF:
			case Node::NodeType::WAIT_NIGHT_TIME_LEAF:
			case Node::NodeType::ASK_INVENTORY_TASK_LEAF:
			case Node::NodeType::TAKE_RESOURCE_LEAF:
			case Node::NodeType::PUT_RESOURCE_LEAF:
			case Node::NodeType::FIND_PATH_TO_LEAF:
				outString += LeafNodeToString(child);
				break;
			default: ;
			}
			outString += ",";
		}
		//remove last ","
		outString = outString.substr(0, outString.size() - 1);

		outString += "]";
	}

	outString += "}";
	return outString;
}

std::string BehaviorTreeUtility::DecoratorNodeToString(const Node::ptr& node)
{
	std::string outString = "{";
	outString += R"("name" : ")" + NodeTypeToString(node->nodeType) + "\",";

	switch(node->nodeType)
	{
	case Node::NodeType::REPEATER_DECORATOR: 
		outString += "\"limit\" : " + std::to_string(static_cast<RepeaterData*>(node->m_Datas.get())->m_Limit) + ",";
		break;
	case Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR: break;
	case Node::NodeType::SUCCEEDER_DECORATOR: break;
	case Node::NodeType::INVERTER_DECORATOR: break;
	default: ;
	}

	outString += "\"type\" : " + std::to_string(static_cast<int>(NodeType::DECORATOR));

	auto* data = static_cast<DecoratorData*>(node->m_Datas.get());
	if(data->m_Child != nullptr)
	{
		outString += ",";
		outString += "\"childs\" : [";

		switch (data->m_Child->nodeType)
		{
		case Node::NodeType::SEQUENCE_COMPOSITE:
		case Node::NodeType::SELECTOR_COMPOSITE:
			outString += CompositeNodeToString(data->m_Child);
			break;
		case Node::NodeType::REPEATER_DECORATOR:
		case Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
		case Node::NodeType::SUCCEEDER_DECORATOR:
		case Node::NodeType::INVERTER_DECORATOR:
			outString += DecoratorNodeToString(data->m_Child);
			break;
		case Node::NodeType::WAIT_FOR_PATH_LEAF:
		case Node::NodeType::MOVE_TO_LEAF:
		case Node::NodeType::HAS_DWELLING_LEAF:
		case Node::NodeType::SET_DWELLING_LEAF:
		case Node::NodeType::ENTER_DWELLING_LEAF:
		case Node::NodeType::EXIT_DWELLING_LEAF:
		case Node::NodeType::ENTER_WORKING_PLACE_LEAF:
		case Node::NodeType::EXIT_WORKING_PLACE_LEAF:
		case Node::NodeType::HAS_JOB_LEAF:
		case Node::NodeType::HAS_STATIC_JOB_LEAF:
		case Node::NodeType::ASSIGN_JOB_LEAF:
		case Node::NodeType::IS_DAY_TIME_LEAF:
		case Node::NodeType::IS_NIGHT_TIME_LEAF:
		case Node::NodeType::WAIT_DAY_TIME_LEAF:
		case Node::NodeType::WAIT_NIGHT_TIME_LEAF:
		case Node::NodeType::ASK_INVENTORY_TASK_LEAF:
		case Node::NodeType::TAKE_RESOURCE_LEAF:
		case Node::NodeType::PUT_RESOURCE_LEAF:
		case Node::NodeType::FIND_PATH_TO_LEAF:
			outString += LeafNodeToString(data->m_Child);
			break;
		default:;
		}

		outString += "]";
	}

	outString += "}";
	return outString;
}
}
