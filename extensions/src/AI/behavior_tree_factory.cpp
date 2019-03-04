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

#include <fstream>

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
			const NodeGroup nodeType = rootNodeJson[0]["type"];

			switch (nodeType)
			{
			case NodeGroup::LEAF:
				return AddLeafNodeFromJson(rootNodeJson[0], nullptr, behaviorTree);
			case NodeGroup::COMPOSITE:
				return AddCompositeNodeFromJson(rootNodeJson[0], nullptr, behaviorTree);
			case NodeGroup::DECORATOR:
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
	case NodeType::SEQUENCE_COMPOSITE:
	case NodeType::SELECTOR_COMPOSITE:
		outfile << CompositeNodeToString(node);
		break;
	case NodeType::REPEATER_DECORATOR:
	case NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
	case NodeType::SUCCEEDER_DECORATOR:
	case NodeType::INVERTER_DECORATOR:
		outfile << DecoratorNodeToString(node);
		break;
	case NodeType::WAIT_FOR_PATH_LEAF:
	case NodeType::MOVE_TO_LEAF: 
	case NodeType::HAS_DWELLING_LEAF:
	case NodeType::SET_DWELLING_LEAF: 
	case NodeType::ENTER_DWELLING_LEAF: 
	case NodeType::EXIT_DWELLING_LEAF:
	case NodeType::ENTER_WORKING_PLACE_LEAF: 
	case NodeType::EXIT_WORKING_PLACE_LEAF:
	case NodeType::HAS_JOB_LEAF:
	case NodeType::HAS_STATIC_JOB_LEAF: 
	case NodeType::ASSIGN_JOB_LEAF:
	case NodeType::IS_DAY_TIME_LEAF: 
	case NodeType::IS_NIGHT_TIME_LEAF: 
	case NodeType::WAIT_DAY_TIME_LEAF:
	case NodeType::WAIT_NIGHT_TIME_LEAF: 
	case NodeType::ASK_INVENTORY_TASK_LEAF: 
	case NodeType::TAKE_RESOURCE_LEAF:
	case NodeType::PUT_RESOURCE_LEAF: 
	case NodeType::FIND_PATH_TO_LEAF: 
		outfile << LeafNodeToString(node);
		break;
	default: ;
	}
	outfile << " ]}";

	outfile.close();
}

std::string BehaviorTreeUtility::NodeTypeToString(const NodeType nodeType)
{
	switch (nodeType)
	{
	case NodeType::SEQUENCE_COMPOSITE:
		return "Sequence";
	case NodeType::SELECTOR_COMPOSITE:
		return "Selector";
	case NodeType::REPEATER_DECORATOR:
		return "Repeater";
	case NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
		return "Repeat until fail";
	case NodeType::SUCCEEDER_DECORATOR:
		return "Succeeder";
	case NodeType::INVERTER_DECORATOR:
		return "Inverter";
	case NodeType::WAIT_FOR_PATH_LEAF:
		return "Wait for path";
	case NodeType::MOVE_TO_LEAF:
		return "Move to";
	case NodeType::HAS_DWELLING_LEAF:
		return "Has dwelling";
	case NodeType::SET_DWELLING_LEAF:
		return "Set dwelling";
	case NodeType::ENTER_DWELLING_LEAF:
		return "Enter dwelling";
	case NodeType::EXIT_DWELLING_LEAF:
		return "Exit dwelling";
	case NodeType::ENTER_WORKING_PLACE_LEAF:
		return "Enter working place";
	case NodeType::EXIT_WORKING_PLACE_LEAF:
		return "Exit working place";
	case NodeType::HAS_JOB_LEAF:
		return "Has job";
	case NodeType::HAS_STATIC_JOB_LEAF:
		return "Has static job";
	case NodeType::ASSIGN_JOB_LEAF:
		return "Assign job";
	case NodeType::IS_DAY_TIME_LEAF:
		return "Is day time";
	case NodeType::IS_NIGHT_TIME_LEAF:
		return "Is night time";
	case NodeType::WAIT_DAY_TIME_LEAF:
		return "Wait day time";
	case NodeType::WAIT_NIGHT_TIME_LEAF:
		return "Wait night time";
	case NodeType::ASK_INVENTORY_TASK_LEAF:
		return "Ask inventory task";
	case NodeType::TAKE_RESOURCE_LEAF:
		return "Take resource";
	case NodeType::PUT_RESOURCE_LEAF:
		return "Put resource";
	case NodeType::FIND_PATH_TO_LEAF:
		return "Find path to";
	default:
		std::ostringstream oss;
		oss << "[Error] NodeType to String : the type is not implemented";
		Log::GetInstance()->Error(oss.str());
		return "";
	}
}

std::string BehaviorTreeUtility::NodeDestinationToString(const NodeDestination nodeDestination)
{
	switch(nodeDestination)
	{
	case NodeDestination::RANDOM: 
		return "Random";
	case NodeDestination::DWELLING: 
		return "Dwelling";
	case NodeDestination::WORKING_PLACE: 
		return "Working place";
	case NodeDestination::INVENTORY_TASK_GIVER: 
		return "Inventory task giver";
	case NodeDestination::INVENTORY_TASK_RECEIVER: 
		return "Inventory task receiver";
	default: 
		std::ostringstream oss;
		oss << "[Error] NodeDestination to String : the destination is not implemented";
		Log::GetInstance()->Error(oss.str());
		return "";
	}

}

Node::ptr BehaviorTreeUtility::AddLeafNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree)
{
	std::cout << "Load Leaf:\n";
	std::shared_ptr<Node> leaf;

	if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::WAIT_FOR_PATH_LEAF))
	{
		std::cout << "   -> Wait for path leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::WAIT_FOR_PATH_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::MOVE_TO_LEAF))
	{
		std::cout << "   -> Move to leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::MOVE_TO_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::FIND_PATH_TO_LEAF))
	{
		std::cout << "   -> Find path to leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::FIND_PATH_TO_LEAF);

		if (CheckJsonExists(behaviorTreeJson, "destination")) {
			static_cast<FindPathToData*>(leaf->data.get())->destination = static_cast<NodeDestination>(behaviorTreeJson["destination"]);
		}
	}else if(behaviorTreeJson["name"] == NodeTypeToString(NodeType::HAS_DWELLING_LEAF))
	{
		std::cout << "   -> Has dwelling leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::HAS_DWELLING_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::SET_DWELLING_LEAF))
	{
		std::cout << "   -> Set dwelling leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::SET_DWELLING_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::ENTER_DWELLING_LEAF))
	{
		std::cout << "   -> Enter dwelling leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::ENTER_DWELLING_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::EXIT_DWELLING_LEAF))
	{
		std::cout << "   -> Exit dwelling leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::EXIT_DWELLING_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::ENTER_WORKING_PLACE_LEAF))
	{
		std::cout << "   -> Enter working place leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::ENTER_WORKING_PLACE_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::EXIT_WORKING_PLACE_LEAF))
	{
		std::cout << "   -> Exit working place leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::EXIT_WORKING_PLACE_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::HAS_JOB_LEAF))
	{
		std::cout << "   -> Has job leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::HAS_JOB_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::HAS_STATIC_JOB_LEAF))
	{
		std::cout << "   -> Has static job leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::HAS_STATIC_JOB_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::ASSIGN_JOB_LEAF))
	{
		std::cout << "   -> Assign job leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::ASSIGN_JOB_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::IS_DAY_TIME_LEAF))
	{
		std::cout << "   -> Is day time leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::IS_DAY_TIME_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::IS_NIGHT_TIME_LEAF))
	{
		std::cout << "   -> Is night time leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::IS_NIGHT_TIME_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::WAIT_DAY_TIME_LEAF))
	{
		std::cout << "   -> Wait day time leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::WAIT_DAY_TIME_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::WAIT_NIGHT_TIME_LEAF))
	{
		std::cout << "   -> Wait night time leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::WAIT_NIGHT_TIME_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::ASK_INVENTORY_TASK_LEAF))
	{
		std::cout << "   -> Ask inventory task leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::ASK_INVENTORY_TASK_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::TAKE_RESOURCE_LEAF))
	{
		std::cout << "   -> Take resource leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::TAKE_RESOURCE_LEAF);
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::PUT_RESOURCE_LEAF))
	{
		std::cout << "   -> Put resource leaf\n";
		leaf = std::make_shared<Node>(behaviorTree, parentNode, NodeType::PUT_RESOURCE_LEAF);
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

	if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::SEQUENCE_COMPOSITE))
	{
		composite = std::make_shared<Node>(behaviorTree, parentNode, NodeType::SEQUENCE_COMPOSITE);
		std::cout << "   ->Sequence composite\n";
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::SELECTOR_COMPOSITE))
	{
		composite = std::make_shared<Node>(behaviorTree, parentNode, NodeType::SELECTOR_COMPOSITE);
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
				const NodeGroup nodeType = childJson["type"];

				switch (nodeType)
				{
				case NodeGroup::NONE:
				{
					std::ostringstream oss;
					oss << "[Error] No type specified for root node : " << childJson;
					Log::GetInstance()->Error(oss.str());
				}
				break;
				case NodeGroup::LEAF:
					static_cast<CompositeData*>(composite->data.get())->children.push_back(AddLeafNodeFromJson(childJson, composite, behaviorTree));
					break;
				case NodeGroup::COMPOSITE:
					static_cast<CompositeData*>(composite->data.get())->children.push_back(AddCompositeNodeFromJson(childJson, composite, behaviorTree));
					break;
				case NodeGroup::DECORATOR:
					static_cast<CompositeData*>(composite->data.get())->children.push_back(AddDecoratorNodeFromJson(childJson, composite, behaviorTree));
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

	if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::REPEAT_UNTIL_FAIL_DECORATOR))
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, NodeType::REPEAT_UNTIL_FAIL_DECORATOR);
		std::cout << "   ->Repeat until fail decorator\n";
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::INVERTER_DECORATOR))
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, NodeType::INVERTER_DECORATOR);
		std::cout << "   ->Inverter decorator\n";
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::REPEATER_DECORATOR))
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, NodeType::REPEATER_DECORATOR);
		std::cout << "   ->Repeater decorator\n";

		if (CheckJsonExists(behaviorTreeJson, "limit"))
		{
			static_cast<RepeaterData*>(decorator->data.get())->limit = behaviorTreeJson["limit"];
		}
	}
	else if (behaviorTreeJson["name"] == NodeTypeToString(NodeType::SUCCEEDER_DECORATOR))
	{
		decorator = std::make_shared<Node>(behaviorTree, parentNode, NodeType::SUCCEEDER_DECORATOR);
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
				const NodeGroup nodeType = childJson["type"];

				switch (nodeType)
				{
				case NodeGroup::LEAF:
					static_cast<DecoratorData*>(decorator->data.get())->child = AddLeafNodeFromJson(childJson, decorator, behaviorTree);
					break;
				case NodeGroup::COMPOSITE:
					static_cast<DecoratorData*>(decorator->data.get())->child = AddCompositeNodeFromJson(childJson, decorator, behaviorTree);
					break;
				case NodeGroup::DECORATOR:
					static_cast<DecoratorData*>(decorator->data.get())->child = AddDecoratorNodeFromJson(childJson, decorator, behaviorTree);
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
	case NodeType::WAIT_FOR_PATH_LEAF:
	case NodeType::MOVE_TO_LEAF:
	case NodeType::HAS_DWELLING_LEAF:
	case NodeType::SET_DWELLING_LEAF:
	case NodeType::ENTER_DWELLING_LEAF:
	case NodeType::EXIT_DWELLING_LEAF:
	case NodeType::ENTER_WORKING_PLACE_LEAF:
	case NodeType::EXIT_WORKING_PLACE_LEAF:
	case NodeType::HAS_JOB_LEAF:
	case NodeType::HAS_STATIC_JOB_LEAF:
	case NodeType::ASSIGN_JOB_LEAF:
	case NodeType::IS_DAY_TIME_LEAF:
	case NodeType::IS_NIGHT_TIME_LEAF:
	case NodeType::WAIT_DAY_TIME_LEAF:
	case NodeType::WAIT_NIGHT_TIME_LEAF:
	case NodeType::ASK_INVENTORY_TASK_LEAF:
	case NodeType::TAKE_RESOURCE_LEAF:
	case NodeType::PUT_RESOURCE_LEAF:
		break;
	case NodeType::FIND_PATH_TO_LEAF:
		outString += "\"destination\" : " + std::to_string(
			static_cast<int>(static_cast<FindPathToData*>(node->data.get())->destination)) + ",";
		break;
	default: ;
	}
	outString += "\"type\" : " + std::to_string(static_cast<int>(NodeGroup::LEAF));

	outString += "}";
	return outString;
}

std::string BehaviorTreeUtility::CompositeNodeToString(const Node::ptr& node)
{
	std::string outString = "{";
	outString += R"("name" : ")" + NodeTypeToString(node->nodeType) + "\",";
	
	outString += "\"type\" : " + std::to_string(static_cast<int>(NodeGroup::COMPOSITE));

	auto* data = static_cast<CompositeData*>(node->data.get());

	if (!data->children.empty())
	{
		outString += ",";
		outString += "\"childs\" : [";

		for (auto child : data->children)
		{
			switch (child->nodeType)
			{
			case NodeType::SEQUENCE_COMPOSITE:
			case NodeType::SELECTOR_COMPOSITE:
				outString += CompositeNodeToString(child);
				break;
			case NodeType::REPEATER_DECORATOR:
			case NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
			case NodeType::SUCCEEDER_DECORATOR:
			case NodeType::INVERTER_DECORATOR:
				outString += DecoratorNodeToString(child);
				break;
			case NodeType::WAIT_FOR_PATH_LEAF:
			case NodeType::MOVE_TO_LEAF:
			case NodeType::HAS_DWELLING_LEAF:
			case NodeType::SET_DWELLING_LEAF:
			case NodeType::ENTER_DWELLING_LEAF:
			case NodeType::EXIT_DWELLING_LEAF:
			case NodeType::ENTER_WORKING_PLACE_LEAF:
			case NodeType::EXIT_WORKING_PLACE_LEAF:
			case NodeType::HAS_JOB_LEAF:
			case NodeType::HAS_STATIC_JOB_LEAF:
			case NodeType::ASSIGN_JOB_LEAF:
			case NodeType::IS_DAY_TIME_LEAF:
			case NodeType::IS_NIGHT_TIME_LEAF:
			case NodeType::WAIT_DAY_TIME_LEAF:
			case NodeType::WAIT_NIGHT_TIME_LEAF:
			case NodeType::ASK_INVENTORY_TASK_LEAF:
			case NodeType::TAKE_RESOURCE_LEAF:
			case NodeType::PUT_RESOURCE_LEAF:
			case NodeType::FIND_PATH_TO_LEAF:
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
	case NodeType::REPEATER_DECORATOR: 
		outString += "\"limit\" : " + std::to_string(static_cast<RepeaterData*>(node->data.get())->limit) + ",";
		break;
	case NodeType::REPEAT_UNTIL_FAIL_DECORATOR: break;
	case NodeType::SUCCEEDER_DECORATOR: break;
	case NodeType::INVERTER_DECORATOR: break;
	default: ;
	}

	outString += "\"type\" : " + std::to_string(static_cast<int>(NodeGroup::DECORATOR));

	auto* data = static_cast<DecoratorData*>(node->data.get());
	if(data->child != nullptr)
	{
		outString += ",";
		outString += "\"childs\" : [";

		switch (data->child->nodeType)
		{
		case NodeType::SEQUENCE_COMPOSITE:
		case NodeType::SELECTOR_COMPOSITE:
			outString += CompositeNodeToString(data->child);
			break;
		case NodeType::REPEATER_DECORATOR:
		case NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
		case NodeType::SUCCEEDER_DECORATOR:
		case NodeType::INVERTER_DECORATOR:
			outString += DecoratorNodeToString(data->child);
			break;
		case NodeType::WAIT_FOR_PATH_LEAF:
		case NodeType::MOVE_TO_LEAF:
		case NodeType::HAS_DWELLING_LEAF:
		case NodeType::SET_DWELLING_LEAF:
		case NodeType::ENTER_DWELLING_LEAF:
		case NodeType::EXIT_DWELLING_LEAF:
		case NodeType::ENTER_WORKING_PLACE_LEAF:
		case NodeType::EXIT_WORKING_PLACE_LEAF:
		case NodeType::HAS_JOB_LEAF:
		case NodeType::HAS_STATIC_JOB_LEAF:
		case NodeType::ASSIGN_JOB_LEAF:
		case NodeType::IS_DAY_TIME_LEAF:
		case NodeType::IS_NIGHT_TIME_LEAF:
		case NodeType::WAIT_DAY_TIME_LEAF:
		case NodeType::WAIT_NIGHT_TIME_LEAF:
		case NodeType::ASK_INVENTORY_TASK_LEAF:
		case NodeType::TAKE_RESOURCE_LEAF:
		case NodeType::PUT_RESOURCE_LEAF:
		case NodeType::FIND_PATH_TO_LEAF:
			outString += LeafNodeToString(data->child);
			break;
		default:;
		}

		outString += "]";
	}

	outString += "}";
	return outString;
}
}
