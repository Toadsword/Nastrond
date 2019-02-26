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
#include <utility>

#include <extensions/AI/behavior_tree_nodes_core.h>
#include <extensions/AI/behavior_tree.h>

namespace sfge::ext::behavior_tree
{
Node::Node(BehaviorTree* bt, ptr parentNode, NodeType type)
{
	m_BehaviorTree = bt;

	m_ParentNode = std::move(parentNode);
	nodeType = type;
	switch (type) { 
	case NodeType::SEQUENCE_COMPOSITE:
	{
		CompositeData compositeData;
		compositeData.children = std::vector<std::shared_ptr<Node>>{};
		data = std::make_unique<CompositeData>(compositeData);


		executeFunction = [=](const int index) {this->SequenceComposite(index); };
	}
	break;
	case NodeType::SELECTOR_COMPOSITE:
	{
		CompositeData compositeData;
		compositeData.children = std::vector<std::shared_ptr<Node>>{};
		data = std::make_unique<CompositeData>(compositeData);

		executeFunction = [=](const int index) {this->SelectorComposite(index); };
	}
		break;
	case NodeType::REPEATER_DECORATOR:
	{
		RepeaterData repeaterData;
		repeaterData.child = nullptr;
		repeaterData.limit = 0;
		data = std::make_unique<RepeaterData>(repeaterData);

		executeFunction = [=](const int index) {this->RepeaterDecorator(index); };
	}
		break;
	case NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
	{
		DecoratorData decoratorData;
		decoratorData.child = nullptr;
		data = std::make_unique<DecoratorData>(decoratorData);

		executeFunction = [=](const int index) {this->RepeatUntilFailDecorator(index); };
	}
	break;
	case NodeType::SUCCEEDER_DECORATOR:
	{
		DecoratorData decoratorData;
		decoratorData.child = nullptr;
		data = std::make_unique<DecoratorData>(decoratorData);

		executeFunction = [=](const int index) {this->SucceederDecorator(index); };
	}
	break;
	case NodeType::INVERTER_DECORATOR: 
	{
		DecoratorData decoratorData;
		decoratorData.child = nullptr;
		data = std::make_unique<DecoratorData>(decoratorData);

		executeFunction = [=](const int index) {this->InverterDecorator(index); };
	}
		break;
	case NodeType::WAIT_FOR_PATH_LEAF: 
		executeFunction = [=](const int index) {this->WaitForPath(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::MOVE_TO_LEAF: 
		executeFunction = [=](const int index) {this->MoveToLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::HAS_DWELLING_LEAF: 
		executeFunction = [=](const int index) {this->HasDwellingLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::SET_DWELLING_LEAF:
		executeFunction = [=](const int index) {this->SetDwellingLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::ENTER_DWELLING_LEAF: 
		executeFunction = [=](const int index) {this->EnterDwellingLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::EXIT_DWELLING_LEAF: 
		executeFunction = [=](const int index) {this->ExitDwellingLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::ENTER_WORKING_PLACE_LEAF: 
		executeFunction = [=](const int index) {this->EnterWorkingPlaceLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::EXIT_WORKING_PLACE_LEAF: 
		executeFunction = [=](const int index) {this->ExitWorkingPlaceLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::HAS_JOB_LEAF: 
		executeFunction = [=](const int index) {this->HasJobLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::HAS_STATIC_JOB_LEAF: 
		executeFunction = [=](const int index) {this->HasStaticJobLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::ASSIGN_JOB_LEAF: 
		executeFunction = [=](const int index) {this->AssignJobLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::IS_DAY_TIME_LEAF: 
		executeFunction = [=](const int index) {this->IsDayTimeLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::IS_NIGHT_TIME_LEAF: 
		executeFunction = [=](const int index) {this->IsNightTimeLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::WAIT_DAY_TIME_LEAF: 
		executeFunction = [=](const int index) {this->WaitDayTimeLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::WAIT_NIGHT_TIME_LEAF: 
		executeFunction = [=](const int index) {this->WaitNightTimeLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::ASK_INVENTORY_TASK_LEAF: 
		executeFunction = [=](const int index) {this->AskInventoryTaskLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::TAKE_RESOURCE_LEAF: 
		executeFunction = [=](const int index) {this->TakeResourcesLeaf(index); };
		data = std::make_unique<NodeData>();
		break;
	case NodeType::FIND_PATH_TO_LEAF:
	{
		FindPathToData findPathToData;
		findPathToData.destination = NodeDestination::RANDOM;
		data = std::make_unique<FindPathToData>(findPathToData);

		executeFunction = [=](const int index) {this->FindPathToLeaf(index); };
	}
		break;
	case NodeType::PUT_RESOURCE_LEAF: 
		executeFunction = [=](const int index) {this->PutResourcesLeaf(index); };
		break;
	default: ; }
}

Node::~Node()
{
}

	void Node::Resize(int newSize)
	{
		nextEntity.resize(newSize);
		currentEntity.resize(newSize);
		previousStatus.resize(newSize);
		nextStatus.resize(newSize);
		previousFlowDirection.resize(newSize);
		nextFlowDirection.resize(newSize);
		nextNode.resize(newSize);
	}

	void Node::FollowFlow()
	{
		for (int i = 0; i < indexCurrentEntity; i++)
		{
			if (nextNode[i] != nullptr) {
				nextNode[i]->AddEntity(currentEntity[i], nextStatus[i], nextFlowDirection[i]);
			}else
			{
				AddEntity(currentEntity[i], nextStatus[i], nextFlowDirection[i]);
			}
		}
	}

	void Node::UpdateFlow()
	{
	for(int i = 0; i < indexNextEntity; i++)
	{
		currentEntity[i] = nextEntity[i];
	}

	for (int i = 0; i < indexNextEntity; i++)
	{
		previousStatus[i] = nextStatus[i];
	}

	for (int i = 0; i < indexNextEntity; i++)
	{
		previousFlowDirection[i] = nextFlowDirection[i];
	}
		indexCurrentEntity = indexNextEntity;
		indexNextEntity = 0;
	}

	void Node::AddEntity(Entity e)
	{
		nextEntity[indexNextEntity] = e;
		nextStatus[indexNextEntity] = NodeStatus::RUNNING;
		nextFlowDirection[indexNextEntity] = FlowDirection::DOWN;
		indexNextEntity++;
	}

	void Node::AddEntity(Entity e, NodeStatus status, FlowDirection flow)
	{
		nextEntity[indexNextEntity] = e;
		nextStatus[indexNextEntity] = status;
		nextFlowDirection[indexNextEntity] = flow;
		indexNextEntity++;
	}

	void Node::DestroyChild(Node* childNode)
{
	switch (nodeType) { 
	case NodeType::SEQUENCE_COMPOSITE:
	case NodeType::SELECTOR_COMPOSITE:
	{
		std::vector<ptr> newChildren;

		for (const auto& child : static_cast<CompositeData*>(data.get())->children)
		{
			if (child.get() != childNode)
			{
				newChildren.push_back(child);
			}
		}

		static_cast<CompositeData*>(data.get())->children = newChildren;
		}
			

		break;
	case NodeType::REPEATER_DECORATOR:
	case NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
	case NodeType::SUCCEEDER_DECORATOR: 
	case NodeType::INVERTER_DECORATOR:
		static_cast<DecoratorData*>(data.get())->child = nullptr;
		break;
	
	default: 
		std::ostringstream oss;
		oss << "[Error] A child is destroy from a non implemented node : " << std::to_string(static_cast<int>(nodeType));
		Log::GetInstance()->Error(oss.str());
	}
}

void Node::Destroy()
{
	if(m_ParentNode != nullptr)
	{
		m_ParentNode->DestroyChild(this);
	}
}

void Node::AddChild(NodeType type)
{
	switch(nodeType)
	{
	case NodeType::SEQUENCE_COMPOSITE:
	case NodeType::SELECTOR_COMPOSITE:
	{
		const auto child = std::make_shared<Node>(m_BehaviorTree, m_ParentNode, type);

		static_cast<CompositeData*>(data.get())->children.push_back(child);
	}
		break;
	case NodeType::REPEATER_DECORATOR:
	case NodeType::REPEAT_UNTIL_FAIL_DECORATOR:
	case NodeType::SUCCEEDER_DECORATOR:
	case NodeType::INVERTER_DECORATOR:
	{
		const auto child = std::make_shared<Node>(m_BehaviorTree, m_ParentNode, type);

		static_cast<DecoratorData*>(data.get())->child = child;
	}
		break;
	default: 
		std::ostringstream oss;
		oss << "[Error] A child cannot be add to this node : " << std::to_string(static_cast<int>(nodeType));
		Log::GetInstance()->Error(oss.str());
	}

}

void Node::Execute(const unsigned int index) const
{
	executeFunction(index);
}

void Node::SequenceComposite(const unsigned int index)
{
	auto* nodeData = static_cast<CompositeData*>(data.get());

#ifdef BT_SOA
	for (auto i = 0; i < indexCurrentEntity; i++)
	{
		if(previousStatus[i] == NodeStatus::FAIL)
		{
			nextFlowDirection[i] = FlowDirection::UP;
			nextStatus[i] = NodeStatus::FAIL;
		}else
		{
			if(previousFlowDirection[i] == FlowDirection::UP)
			{
				nextFlowDirection[i] = FlowDirection::UP;
			}
			else if (previousFlowDirection[i] == FlowDirection::DOWN){
				nextFlowDirection[i] = FlowDirection::DOWN;
			}
			nextStatus[i] = NodeStatus::SUCCESS;
		}
	}

	for (auto i = 0; i < indexCurrentEntity; i++)
	{
		switch (nextFlowDirection[i]) { 
			case FlowDirection::UP:
				nextNode[i] = m_ParentNode;
			break;
		case FlowDirection::DOWN:
			nextNode[i] = nodeData->children[0];
			break;
		case FlowDirection::NEXT:
			nextNode[i] = nodeData->next;
			break;
		case FlowDirection::NONE:
			nextNode[i] = nullptr;
			break;
		default: ;
		}
	}
#endif
}

void Node::SelectorComposite(const unsigned int index) const
{
	auto* nodeData = static_cast<CompositeData*>(data.get());

//#ifdef BT_SOA
//	//If last one is parent => first time entering the sequence
//	if (m_BehaviorTree->doesFlowGoDown[index])
//	{
//		m_BehaviorTree->sequenceActiveChild[index] = 0;
//		m_BehaviorTree->currentNode[index] = nodeData->children[0];
//
//		m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
//		return;
//	}
//
//	//Else it means that the previous node is a children
//	for (size_t i = 0; i < nodeData->children.size(); i++)
//	{
//		if (i == m_BehaviorTree->sequenceActiveChild[index])
//		{
//			//If last one is a success => going out of node
//			if (m_BehaviorTree->previousStatus[index] == NodeStatus::SUCCESS)
//			{
//				m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//				m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//				return;
//			}
//
//			//Else if not last child => go next child
//			if (i < nodeData->children.size() - 1)
//			{
//				m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoDown;
//				m_BehaviorTree->currentNode[index] = nodeData->children[i + 1];
//				m_BehaviorTree->sequenceActiveChild[index]++;
//
//				m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
//				return;
//			}
//			//mean that they all failed => return fail
//			m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//			m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//			return;
//		}
//	}
//	m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
//#endif
}

void Node::RepeaterDecorator(const unsigned int index)
{
	auto* nodeData = static_cast<RepeaterData*>(data.get());

#ifdef BT_SOA
	for(auto i = 0; i < indexCurrentEntity; i++)
	{
		nextFlowDirection[i] = FlowDirection::DOWN;
		nextStatus[i] = NodeStatus::SUCCESS;
	}

	for (auto i = 0; i < currentEntity.size(); i++)
	{
		nextNode[i] = nodeData->child;
	}
#endif
}

void Node::RepeatUntilFailDecorator(const unsigned int index) const
{
	auto* nodeData = static_cast<DecoratorData*>(data.get());

//#ifdef BT_SOA
//	if (m_BehaviorTree->doesFlowGoDown[index])
//	{
//		m_BehaviorTree->currentNode[index] = nodeData->child;
//
//		m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
//	}
//	else
//	{
//		if (m_BehaviorTree->previousStatus[index] == NodeStatus::FAIL)
//		{
//			m_BehaviorTree->currentNode[index] = m_ParentNode;
//			m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//		}
//		else
//		{
//			m_BehaviorTree->currentNode[index] = nodeData->child;
//			m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
//		}
//	}
//#endif
}

void Node::InverterDecorator(const unsigned int index) const
{
	auto* nodeData = static_cast<DecoratorData*>(data.get());

//#ifdef BT_SOA
//	if (m_BehaviorTree->doesFlowGoDown[index])
//	{
//		m_BehaviorTree->currentNode[index] = nodeData->child;
//
//		m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
//		return;
//	}
//
//	if (m_BehaviorTree->previousStatus[index] == NodeStatus::SUCCESS)
//	{
//		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//		m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//		m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//		return;
//	}
//
//	if (m_BehaviorTree->previousStatus[index] == NodeStatus::FAIL)
//	{
//		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//		m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//		return;
//	}
//
//	m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//#endif
}

void Node::SucceederDecorator(const unsigned int index) const
{
	auto* nodeData = static_cast<DecoratorData*>(data.get());
#ifdef BT_AOS
	if (m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown)
	{
		m_BehaviorTree->dataBehaviorTree[index].currentNode = nodeData->child;

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = NodeStatus::RUNNING;
		return;
	}

	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;
	m_BehaviorTree->dataBehaviorTree[index].previousStatus = NodeStatus::SUCCESS;
#endif

#ifdef BT_SOA
	//if (m_BehaviorTree->doesFlowGoDown[index])
	//{
	//	m_BehaviorTree->currentNode[index] = nodeData->child;

	//	m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
	//	return;
	//}

	//m_BehaviorTree->currentNode[index] = m_ParentNode;
	//m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
#endif
}

void Node::WaitForPath(const unsigned int index)
{
#ifdef BT_SOA
	std::vector<bool> hasPath;
	hasPath.resize(indexCurrentEntity, false);
	
	for (auto i = 0; i < indexCurrentEntity; i++)
	{
		hasPath[i] = m_BehaviorTree->dwarfManager->HasPath(currentEntity[i]);
	}

	for (auto i = 0; i < indexCurrentEntity; i++)
	{
		if (hasPath[i])
		{
			if (data->next != nullptr) {
				nextFlowDirection[i] = FlowDirection::NEXT;
			}else
			{
				nextFlowDirection[i] = FlowDirection::UP;
			}
			nextStatus[i] = NodeStatus::SUCCESS;
		}else
		{
			nextFlowDirection[i] = FlowDirection::NONE;
			nextStatus[i] = NodeStatus::RUNNING;
		}
	}

	for (auto i = 0; i < indexCurrentEntity; i++)
	{
		switch (nextFlowDirection[i]) {
		case FlowDirection::UP:
			nextNode[i] = m_ParentNode;
			break;
		case FlowDirection::NEXT:
			nextNode[i] = data->next;
			break;
		case FlowDirection::NONE:
			nextNode[i] = nullptr;
			break;
		default:;
		}
	}
#endif
}

void Node::MoveToLeaf(const unsigned int index)
{
#ifdef BT_SOA
	std::vector<int> indexesAtDestination;
	indexesAtDestination.resize(indexCurrentEntity);
	auto indexDwarfAtDestination = 0;

	std::vector<int> indexesNotAtDestination;
	indexesNotAtDestination.resize(indexCurrentEntity);
	auto indexNotDwarfAtDestination = 0;

	//Sort between at destination and not at destination
	for (auto i = 0; i < indexCurrentEntity; i++)
	{
		if (m_BehaviorTree->dwarfManager->IsDwarfAtDestination(currentEntity[i])) {
			indexesAtDestination[indexDwarfAtDestination] = i;
			indexDwarfAtDestination++;
		}else
		{
			m_BehaviorTree->dwarfManager->AddPathFollowingBT(currentEntity[i]);
			indexesNotAtDestination[indexNotDwarfAtDestination] = i;
			indexNotDwarfAtDestination++;
		}
	}

	//Update at destination
	for (auto i = 0; i < indexDwarfAtDestination; i++)
	{
		if (data->next != nullptr) {
			nextFlowDirection[indexesAtDestination[i]] = FlowDirection::NEXT;
		}
		else
		{
			nextFlowDirection[indexesAtDestination[i]] = FlowDirection::UP;
		}
		nextStatus[indexesAtDestination[i]] = NodeStatus::SUCCESS;
	}

	//Update not at destination
	for(auto i = 0; i < indexNotDwarfAtDestination; i++)
	{
		nextFlowDirection[indexesNotAtDestination[i]] = FlowDirection::NONE;
		nextStatus[indexesNotAtDestination[i]] = NodeStatus::RUNNING;
	}

	for (auto i = 0; i < indexCurrentEntity; i++)
	{
		switch(nextFlowDirection[i]) { 
			case FlowDirection::UP:
				nextNode[i] = m_ParentNode;
			break;
		case FlowDirection::NEXT:
			nextNode[i] = data->next;
			break;
		case FlowDirection::NONE:
			nextNode[i] = nullptr;
			break;
		default: ;
		}
}

#endif
}

void Node::HasDwellingLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	if (m_BehaviorTree->dwarfManager->GetDwellingEntity(index) == INVALID_ENTITY)
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//	}
//	else
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//#endif
}

void Node::SetDwellingLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	if (m_BehaviorTree->dwarfManager->AssignDwellingToDwarf(index))
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//	}
//	else
//	{
//
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//#endif
}

void Node::EnterDwellingLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->dwarfManager->DwarfEnterDwelling(index);
//
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//#endif
}

void Node::ExitDwellingLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->dwarfManager->DwarfExitDwelling(index);
//
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//#endif
}

void Node::EnterWorkingPlaceLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->dwarfManager->DwarfEnterWorkingPlace(index);
//
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//#endif
}

void Node::ExitWorkingPlaceLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->dwarfManager->DwarfExitWorkingPlace(index);
//
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//#endif
}

void Node::HasJobLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	if (m_BehaviorTree->dwarfManager->HasJob(index))
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//	else
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//
//	}
//#endif
}

void Node::HasStaticJobLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	if (m_BehaviorTree->dwarfManager->HasJob(index))
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//	else
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//
//	}
//#endif
}

void Node::AssignJobLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	if (m_BehaviorTree->dwarfManager->AssignJob(index))
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//	else
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//	}
//#endif
}

void Node::IsDayTimeLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	if (m_BehaviorTree->dwarfManager->IsDayTime())
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//	else
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//	}
//#endif
}

void Node::IsNightTimeLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	if (m_BehaviorTree->dwarfManager->IsNightTime())
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//	else
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::FAIL;
//	}
//#endif
}

void Node::WaitDayTimeLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	if (m_BehaviorTree->dwarfManager->IsDayTime())
//	{
//		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//		m_BehaviorTree->currentNode[index] = m_ParentNode;
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//	else
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
//	}
//#endif
}

void Node::WaitNightTimeLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	if (m_BehaviorTree->dwarfManager->IsNightTime())
//	{
//		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//		m_BehaviorTree->currentNode[index] = m_ParentNode;
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//	else
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
//	}
//#endif
}

void Node::AskInventoryTaskLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	if (m_BehaviorTree->dwarfManager->AddInventoryTaskBT(index))
//	{
//		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//		m_BehaviorTree->currentNode[index] = m_ParentNode;
//		m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//	}
//	else
//	{
//		m_BehaviorTree->previousStatus[index] = NodeStatus::RUNNING;
//	}
//#endif
}

void Node::TakeResourcesLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->dwarfManager->TakeResources(index);
//
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//#endif
}

void Node::PutResourcesLeaf(const unsigned int index) const
{
//#ifdef BT_SOA
//	m_BehaviorTree->dwarfManager->PutResources(index);
//
//	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
//	m_BehaviorTree->currentNode[index] = m_ParentNode;
//
//	m_BehaviorTree->previousStatus[index] = NodeStatus::SUCCESS;
//#endif
}

void Node::FindPathToLeaf(const unsigned int index)
{
	for (auto i = 0; i < indexCurrentEntity; i++) {
		switch (static_cast<FindPathToData*>(data.get())->destination)
		{
		case NodeDestination::RANDOM:
			m_BehaviorTree->dwarfManager->AddFindRandomPathBT(currentEntity[i]);
			break;
		case NodeDestination::DWELLING:
			m_BehaviorTree->dwarfManager->AddFindPathToDestinationBT(currentEntity[i], m_BehaviorTree->dwarfManager->GetDwellingAssociatedPosition(index));
			break;
		case NodeDestination::WORKING_PLACE:
			m_BehaviorTree->dwarfManager->AddFindPathToDestinationBT(currentEntity[i], m_BehaviorTree->dwarfManager->GetWorkingPlaceAssociatedPosition(index));
			break;
		case NodeDestination::INVENTORY_TASK_GIVER:
			m_BehaviorTree->dwarfManager->AddInventoryTaskPathToGiver(currentEntity[i]);
			break;
		case NodeDestination::INVENTORY_TASK_RECEIVER:
			m_BehaviorTree->dwarfManager->AddInventoryTaskPathToReceiver(currentEntity[i]);
			break;
		case NodeDestination::LENGTH: break;
		default:
			;
		}
	}

#ifdef BT_SOA
	for (auto i = 0; i < indexCurrentEntity; i++) {
		if (data->next != nullptr) {
			nextFlowDirection[i] = FlowDirection::NEXT;
		}
		else
		{
			nextFlowDirection[i] = FlowDirection::UP;
		}

		nextStatus[i] = NodeStatus::SUCCESS;
	}

	for (auto i = 0; i < indexCurrentEntity; i++)
	{
		switch (nextFlowDirection[i]) {
		case FlowDirection::UP:
			nextNode[i] = m_ParentNode;
			break;
		case FlowDirection::NEXT:
			nextNode[i] = data->next;
			break;
		default:;
		}
	}
#endif
}
}
