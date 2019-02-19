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
#include <python/python_engine.h>

#include <extensions/AI/behavior_tree.h>

namespace sfge::ext::behavior_tree
{
BehaviorTree::BehaviorTree(Engine& engine) : System(engine) {}

void BehaviorTree::Init()
{
	dwarfManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<DwarfManager>("DwarfManager");
}

void BehaviorTree::Update(float dt)
{
#ifdef BT_SOA
	if (currentNode.size() < m_Entities->size())
		{
		currentNode.resize(m_Entities->size(), m_RootNode);
		doesFlowGoDown.resize(m_Entities->size());
		previousStatus.resize(m_Entities->size());
		repeaterCounter.resize(m_Entities->size(), 0);
		sequenceActiveChild.resize(m_Entities->size(), 0);
#endif

#ifdef BT_AOS
	if (dataBehaviorTree.size() < m_Entities->size())
		{
		int previousSize = dataBehaviorTree.size();
		dataBehaviorTree.resize(m_Entities->size());

		for(auto i = previousSize; i < dataBehaviorTree.size(); i++)
		{
			dataBehaviorTree[i].currentNode = m_RootNode;
		}
#endif

	}

	for (size_t i = 0; i < m_Entities->size(); i++)
	{
		if (m_Entities->at(i) == INVALID_ENTITY)
		{
			continue;
		}
#ifdef BT_SOA
		currentNode[i]->Execute(i);
#endif

#ifdef BT_AOS
		dataBehaviorTree[i].currentNode->Execute(i);
#endif

	}
}

void BehaviorTree::FixedUpdate() { }

void BehaviorTree::Draw() { }

void BehaviorTree::LoadNodesFromJson(json& behaviorTreeJson)
{
	if (CheckJsonParameter(behaviorTreeJson, "rootNode", json::value_t::array))
	{
		auto rootNodeJson = behaviorTreeJson["rootNode"];
		if (CheckJsonExists(rootNodeJson[0], "type"))
		{
			const NodeType nodeType = rootNodeJson[0]["type"];

			switch (nodeType)
			{
			case NodeType::NONE:
			{
				std::ostringstream oss;
				oss << "[Error] No type specified for root node : " << rootNodeJson[0];
				Log::GetInstance()->Error(oss.str());
			}
			break;
			case NodeType::LEAF:
				SetRootNode(AddLeafNodeFromJson(rootNodeJson[0], nullptr));
				break;
			case NodeType::COMPOSITE:
				SetRootNode(AddCompositeNodeFromJson(rootNodeJson[0], nullptr));
				break;
			case NodeType::DECORATOR:
				SetRootNode(AddDecoratorNodeFromJson(rootNodeJson[0], nullptr));
				break;
			default: ;
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "[Error] the type for the rootNode is missing : " << rootNodeJson[0];
			Log::GetInstance()->Error(oss.str());
		}
	}
	else
	{
		std::ostringstream oss;
		oss << "[Error] the rootNode parameter is missing : " << behaviorTreeJson;
		Log::GetInstance()->Error(oss.str());
	}
}

void BehaviorTree::SetRootNode(const Node::ptr& rootNode)
{
	m_RootNode = rootNode;
}

void BehaviorTree::SetEntities(std::vector<Entity>* vectorEntities)
{
	m_Entities = vectorEntities;
}

std::shared_ptr<Node> BehaviorTree::AddLeafNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode)
{
	std::cout << "Add leaf\n";
	std::shared_ptr<Node> leaf;

	if (behaviorTreeJson["name"] == "WaitForPathLeaf")
	{
		std::cout << "   -> Wait for path leaf\n";
		leaf = std::make_shared<Node>(this, parentNode, Node::NodeType::WAIT_FOR_PATH_LEAF);
	}
	else if(behaviorTreeJson["name"] == "MoveToLeaf")
	{
		std::cout << "   -> Move to leaf\n";
		leaf = std::make_shared<Node>(this, parentNode, Node::NodeType::MOVE_TO_LEAF);
	}
	else if(behaviorTreeJson["name"] == "FindPathToLeaf")
	{
		std::cout << "   -> Find path to leaf\n";
		leaf = std::make_shared<Node>(this, parentNode, Node::NodeType::FIND_PATH_TO_LEAF);

		if (CheckJsonExists(behaviorTreeJson, "destination")) {
			static_cast<FindPathToData*>(leaf->m_Datas.get())->m_Destination = static_cast<NodeDestination>(behaviorTreeJson["destination"]);
		}
	}else
	{
		std::ostringstream oss;
		oss << "[Error] The name : " << behaviorTreeJson["name"] << " is not assigned to any type of leaf";
		Log::GetInstance()->Error(oss.str());

		return nullptr;
	}

	return leaf;
}

std::shared_ptr<Node> BehaviorTree::AddCompositeNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode)
{
	std::cout << "Add Composite\n";
	std::shared_ptr<Node> composite;

	if (behaviorTreeJson["name"] == "SequenceComposite")
	{
		composite = std::make_shared<Node>(this, parentNode, Node::NodeType::SEQUENCE_COMPOSITE);
		std::cout << "   ->Sequence composite\n";
	}
	else if (behaviorTreeJson["name"] == "SelectorComposite")
	{
		composite = std::make_shared<Node>(this, parentNode, Node::NodeType::SELECTOR_COMPOSITE);
		std::cout << "   ->Selector composite\n";
	}else
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
					static_cast<CompositeData*>(composite->m_Datas.get())->m_Children.push_back(AddLeafNodeFromJson(childJson, composite));
					break;
				case NodeType::COMPOSITE:
					static_cast<CompositeData*>(composite->m_Datas.get())->m_Children.push_back(AddCompositeNodeFromJson(childJson, composite));
					break;
				case NodeType::DECORATOR:
					static_cast<CompositeData*>(composite->m_Datas.get())->m_Children.push_back(AddDecoratorNodeFromJson(childJson, composite));
					break;
				default: ;
				}
			}
		}
	}

	return composite;
}

std::shared_ptr<Node> BehaviorTree::AddDecoratorNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode)
{
	std::cout << "Add Decorator\n";
	std::shared_ptr<Node> decorator;

	if (behaviorTreeJson["name"] == "RepeatUntilFailDecorator")
	{
		decorator = std::make_shared<Node>(this, parentNode, Node::NodeType::REPEAT_UNTIL_FAIL_DECORATOR);
		std::cout << "   ->Repeat until fail decorator\n";
	}
	else if (behaviorTreeJson["name"] == "InverterDecorator")
	{
		decorator = std::make_shared<Node>(this, parentNode, Node::NodeType::INVERTER_DECORATOR);
		std::cout << "   ->Inverter decorator\n";
	}
	else if (behaviorTreeJson["name"] == "RepeaterDecorator")
	{
		decorator = std::make_shared<Node>(this, parentNode, Node::NodeType::REPEATER_DECORATOR);
		std::cout << "   ->Repeater decorator\n";

		if (CheckJsonExists(behaviorTreeJson, "limit"))
		{
			static_cast<RepeaterData*>(decorator->m_Datas.get())->m_Limit = behaviorTreeJson["limit"];
		}
	}
	else if (behaviorTreeJson["name"] == "SucceederDecorator")
	{
		decorator = std::make_shared<Node>(this, parentNode, Node::NodeType::SUCCEEDER_DECORATOR);
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
					static_cast<DecoratorData*>(decorator->m_Datas.get())->m_Child = AddLeafNodeFromJson(childJson, decorator);
					break;
				case NodeType::COMPOSITE:
					static_cast<DecoratorData*>(decorator->m_Datas.get())->m_Child = AddCompositeNodeFromJson(childJson, decorator);
					break;
				case NodeType::DECORATOR:
					static_cast<DecoratorData*>(decorator->m_Datas.get())->m_Child = AddDecoratorNodeFromJson(childJson, decorator);
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
