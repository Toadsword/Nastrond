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
#include <utility>
#include <functional>

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
		break;
	case NodeType::MOVE_TO_LEAF: 
		executeFunction = [=](const int index) {this->MoveToLeaf(index); };
		break;
	case NodeType::HAS_DWELLING_LEAF: 
		executeFunction = [=](const int index) {this->HasDwellingLeaf(index); };
		break;
	case NodeType::SET_DWELLING_LEAF:
		executeFunction = [=](const int index) {this->SetDwellingLeaf(index); };
		break;
	case NodeType::ENTER_DWELLING_LEAF: 
		executeFunction = [=](const int index) {this->EnterDwellingLeaf(index); };
		break;
	case NodeType::EXIT_DWELLING_LEAF: 
		executeFunction = [=](const int index) {this->ExitDwellingLeaf(index); };
		break;
	case NodeType::ENTER_WORKING_PLACE_LEAF: 
		executeFunction = [=](const int index) {this->EnterWorkingPlaceLeaf(index); };
		break;
	case NodeType::EXIT_WORKING_PLACE_LEAF: 
		executeFunction = [=](const int index) {this->ExitWorkingPlaceLeaf(index); };
		break;
	case NodeType::HAS_JOB_LEAF: 
		executeFunction = [=](const int index) {this->HasJobLeaf(index); };
		break;
	case NodeType::HAS_STATIC_JOB_LEAF: 
		executeFunction = [=](const int index) {this->HasStaticJobLeaf(index); };
		break;
	case NodeType::ASSIGN_JOB_LEAF: 
		executeFunction = [=](const int index) {this->AssignJobLeaf(index); };
		break;
	case NodeType::IS_DAY_TIME_LEAF: 
		executeFunction = [=](const int index) {this->IsDayTimeLeaf(index); };
		break;
	case NodeType::IS_NIGHT_TIME_LEAF: 
		executeFunction = [=](const int index) {this->IsNightTimeLeaf(index); };
		break;
	case NodeType::WAIT_DAY_TIME_LEAF: 
		executeFunction = [=](const int index) {this->WaitDayTimeLeaf(index); };
		break;
	case NodeType::WAIT_NIGHT_TIME_LEAF: 
		executeFunction = [=](const int index) {this->WaitNightTimeLeaf(index); };
		break;
	case NodeType::ASK_INVENTORY_TASK_LEAF: 
		executeFunction = [=](const int index) {this->AskInventoryTaskLeaf(index); };
		break;
	case NodeType::TAKE_RESOURCE_LEAF: 
		executeFunction = [=](const int index) {this->TakeResourcesLeaf(index); };
		break;
	case NodeType::FIND_PATH_TO_LEAF:
	{
		FindPathToData findPathToData{};
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

void Node::SequenceComposite(const unsigned int index) const
{
	auto* nodeData = static_cast<CompositeData*>(data.get());

#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Sequence (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if(index == specificEntity)
	{
		std::cout << "Sequence (" << index << ")\n";
	}
#endif

	//if last one returned fail => then it's a fail
	if (!m_BehaviorTree->hasSucceeded[index])
	{
		m_BehaviorTree->currentNode[index] = m_ParentNode;
		return;
	}

	//If last one is parent => first time entering the sequence
	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		nodeData->activeChild[index] = 0;
		m_BehaviorTree->currentNode[index] = nodeData->children[0];
		return;
	}

	if(nodeData->activeChild[index] < nodeData->children.size() - 1)
	{
		//Next child
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoDown;
		nodeData->activeChild[index] += 1;
		m_BehaviorTree->currentNode[index] = nodeData->children[nodeData->activeChild[index]];
		return;
	}

	//Go up to parent when all child visited
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->hasSucceeded[index] = true;
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
}

void Node::SelectorComposite(const unsigned int index) const
{
	auto* nodeData = static_cast<CompositeData*>(data.get());

#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Selector (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Selector (" << index << ")\n";
	}
#endif


	//If last one is parent => first time entering the sequence
	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		nodeData->activeChild[index] = 0;
		m_BehaviorTree->currentNode[index] = nodeData->children[0];
		return;
	}

	//If last one is child AND has succeeded => Go up
	if (!m_BehaviorTree->doesFlowGoDown[index] && m_BehaviorTree->hasSucceeded[index]) {
		m_BehaviorTree->currentNode[index] = m_ParentNode;
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->hasSucceeded[index] = true;
		return;
	}

	//Else it means that the previous node is a children
	for (char i = 0; i < nodeData->children.size(); i++)
	{
		if (i == nodeData->activeChild[index])
		{
			//If last one is a success => going out of node
			if (m_BehaviorTree->hasSucceeded[index])
			{
				m_BehaviorTree->currentNode[index] = m_ParentNode;
				return;
			}

			//Else if not last child => go next child
			if (i < nodeData->children.size() - 1)
			{
				m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoDown;
				m_BehaviorTree->currentNode[index] = nodeData->children[i + 1];
				nodeData->activeChild[index]++;
				return;
			}
			//mean that they all failed => return fail
			m_BehaviorTree->currentNode[index] = m_ParentNode;
			m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
			m_BehaviorTree->hasSucceeded[index] = false;
			return;
		}
	}
}

void Node::RepeaterDecorator(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Repeater (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Repeater (" << index << ")\n";
	}
#endif

	auto* nodeData = static_cast<RepeaterData*>(data.get());

	//If flow goes down => start counter 
	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		m_BehaviorTree->repeaterCounter[index] = 0;
	}
	else
	{
		
		//If limit == 0 => infinity, if m_Counter == m_Limit it's over
		if (nodeData->limit > 0 && ++m_BehaviorTree->repeaterCounter[index] == nodeData->limit)
		{
			m_BehaviorTree->repeaterCounter[index] = 0;

			m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
			m_BehaviorTree->currentNode[index] = m_ParentNode;

			m_BehaviorTree->hasSucceeded[index] = true;
			return;
		}
	}

	//Switch current node to child
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoDown;
	m_BehaviorTree->currentNode[index] = nodeData->child;
}

void Node::RepeatUntilFailDecorator(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Repeat until fail (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Repeat until fail (" << index << ")\n";
	}
#endif

	auto* nodeData = static_cast<DecoratorData*>(data.get());

	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		m_BehaviorTree->currentNode[index] = nodeData->child;
	}
	else
	{
		if (!m_BehaviorTree->hasSucceeded[index])
		{
			m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
			m_BehaviorTree->currentNode[index] = m_ParentNode;
			m_BehaviorTree->hasSucceeded[index] = true;
		}
		else
		{
			m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoDown;
			m_BehaviorTree->currentNode[index] = nodeData->child;
		}
	}
}

void Node::InverterDecorator(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Inverter (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Inverter (" << index << ")\n";
	}
#endif

	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		auto* nodeData = static_cast<DecoratorData*>(data.get());
		m_BehaviorTree->currentNode[index] = nodeData->child;
		return;
	}

	if (m_BehaviorTree->hasSucceeded[index])
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;

		m_BehaviorTree->hasSucceeded[index] = false;
		return;
	}

	if (!m_BehaviorTree->hasSucceeded[index])
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;

		m_BehaviorTree->hasSucceeded[index] = true;
		return;
	}

	m_BehaviorTree->hasSucceeded[index] = false;
}

void Node::SucceederDecorator(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Succeeder (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Succeeder (" << index << ")\n";
	}
#endif

	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		auto* nodeData = static_cast<DecoratorData*>(data.get());
		m_BehaviorTree->currentNode[index] = nodeData->child;
		return;
	}

	m_BehaviorTree->currentNode[index] = m_ParentNode;
	m_BehaviorTree->hasSucceeded[index] = true;
}

void Node::WaitForPath(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Wait for path (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Wait for path (" << index << ")\n";
	}
#endif

	if (m_BehaviorTree->dwarfManager->HasPath(index))
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;

		m_BehaviorTree->hasSucceeded[index] = true;
	}
}

void Node::MoveToLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Move to (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Move to (" << index << ")\n";
	}
#endif
	m_BehaviorTree->dwarfManager->AddPathFollowingBT(index);
	m_BehaviorTree->sleepingEntity[index] = true;

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->hasSucceeded[index] = true;
}

void Node::HasDwellingLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Has dwelling (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Has dwelling (" << index << ")\n";
	}
#endif
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->GetDwellingEntity(index) == INVALID_ENTITY)
	{
		m_BehaviorTree->hasSucceeded[index] = false;
	}
	else
	{
		m_BehaviorTree->hasSucceeded[index] = true;
	}
}

void Node::SetDwellingLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Set dwelling (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Set Dwelling (" << index << ")\n";
	}
#endif
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->AssignDwellingToDwarf(index))
	{
		m_BehaviorTree->hasSucceeded[index] = true;
	}
	else
	{
		m_BehaviorTree->hasSucceeded[index] = false;
	}
}

void Node::EnterDwellingLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Enter dwelling (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Enter dwelling (" << index << ")\n";
	}
#endif
	m_BehaviorTree->dwarfManager->DwarfEnterDwelling(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->hasSucceeded[index] = true;
}

void Node::ExitDwellingLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Exit dwelling (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Exit dwelling (" << index << ")\n";
	}
#endif
	m_BehaviorTree->dwarfManager->DwarfExitDwelling(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->hasSucceeded[index] = true;
}

void Node::EnterWorkingPlaceLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Enter working place (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "enter working place (" << index << ")\n";
	}
#endif
	m_BehaviorTree->dwarfManager->DwarfEnterWorkingPlace(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->hasSucceeded[index] = true;
}

void Node::ExitWorkingPlaceLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Exit working place (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Exit working place (" << index << ")\n";
	}
#endif
	m_BehaviorTree->dwarfManager->DwarfExitWorkingPlace(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->hasSucceeded[index] = true;
}

void Node::HasJobLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Has job (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Has job (" << index << ")\n";
	}
#endif
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->HasJob(index))
	{
		m_BehaviorTree->hasSucceeded[index] = true;
	}
	else
	{
		m_BehaviorTree->hasSucceeded[index] = false;

	}
}

void Node::HasStaticJobLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Has static job (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Has static job (" << index << ")\n";
	}
#endif
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->HasJob(index))
	{
		m_BehaviorTree->hasSucceeded[index] = true;
	}
	else
	{
		m_BehaviorTree->hasSucceeded[index] = false;

	}
}

void Node::AssignJobLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Assign job (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Assign job (" << index << ")\n";
	}
#endif
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->AssignJob(index))
	{
		m_BehaviorTree->hasSucceeded[index] = true;
	}
	else
	{
		m_BehaviorTree->hasSucceeded[index] = false;
	}
}

void Node::IsDayTimeLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Is day time (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Is day time (" << index << ")\n";
	}
#endif
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->IsDayTime())
	{
		m_BehaviorTree->hasSucceeded[index] = true;
	}
	else
	{
		m_BehaviorTree->hasSucceeded[index] = false;
	}
}

void Node::IsNightTimeLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Is night time (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Is night time (" << index << ")\n";
	}
#endif
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->IsNightTime())
	{
		m_BehaviorTree->hasSucceeded[index] = true;
	}
	else
	{
		m_BehaviorTree->hasSucceeded[index] = false;
	}
}

void Node::WaitDayTimeLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Wait day (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Wait day (" << index << ")\n";
	}
#endif
	if (m_BehaviorTree->dwarfManager->IsDayTime())
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;
		m_BehaviorTree->hasSucceeded[index] = true;
	}
}

void Node::WaitNightTimeLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Wait night (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Wait night (" << index << ")\n";
	}
#endif
	if (m_BehaviorTree->dwarfManager->IsNightTime())
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;
		m_BehaviorTree->hasSucceeded[index] = true;
	}
}

void Node::AskInventoryTaskLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Ask inventory task (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Ask inventory task (" << index << ")\n";
	}
#endif
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->AddInventoryTaskBT(index))
	{
		m_BehaviorTree->hasSucceeded[index] = true;
	}else
	{
		m_BehaviorTree->hasSucceeded[index] = false;
	}
}

void Node::TakeResourcesLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Take resource (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Take resource (" << index << ")\n";
	}
#endif
	m_BehaviorTree->dwarfManager->TakeResources(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->hasSucceeded[index] = true;
}

void Node::PutResourcesLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Put resource (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Put resource (" << index << ")\n";
	}
#endif
	m_BehaviorTree->dwarfManager->PutResources(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->hasSucceeded[index] = true;
}

void Node::FindPathToLeaf(const unsigned int index) const
{
#ifdef AI_BT_NODE_NAME 
#ifndef AI_BT_SPECIFIC_ENTITY
	std::cout << "Find path to (" << index << ")\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
	if (index == specificEntity)
	{
		std::cout << "Find path to (" << index << ")\n";
	}
#endif
	switch (static_cast<FindPathToData*>(data.get())->destination)
	{
	case NodeDestination::RANDOM:
#ifdef AI_BT_NODE_NAME
#ifndef AI_BT_SPECIFIC_ENTITY
		std::cout << " random\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
		if (index == specificEntity)
		{
			std::cout << "random\n";
		}
#endif
		m_BehaviorTree->dwarfManager->AddFindRandomPathBT(index);
		break;
	case NodeDestination::DWELLING:
#ifdef AI_BT_NODE_NAME
#ifndef AI_BT_SPECIFIC_ENTITY
		std::cout << " dwelling\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
		if (index == specificEntity)
		{
			std::cout << " dwelling\n";
		}
#endif
		m_BehaviorTree->dwarfManager->AddFindPathToDestinationBT(index, m_BehaviorTree->dwarfManager->GetDwellingAssociatedPosition(index));
		break;
	case NodeDestination::WORKING_PLACE:
#ifdef AI_BT_NODE_NAME
#ifndef AI_BT_SPECIFIC_ENTITY
		std::cout << " working place\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
		if (index == specificEntity)
		{
			std::cout << " working place\n";
		}
#endif
		m_BehaviorTree->dwarfManager->AddFindPathToDestinationBT(index, m_BehaviorTree->dwarfManager->GetWorkingPlaceAssociatedPosition(index));
		break;
	case NodeDestination::INVENTORY_TASK_GIVER:
#ifdef AI_BT_NODE_NAME
#ifndef AI_BT_SPECIFIC_ENTITY
		std::cout << " inventory task giver\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
		if (index == specificEntity)
		{
			std::cout << " inventory task giver\n";
		}
#endif
		m_BehaviorTree->dwarfManager->AddInventoryTaskPathToGiver(index);
		break;
	case NodeDestination::INVENTORY_TASK_RECEIVER:
#ifdef AI_BT_NODE_NAME
#ifndef AI_BT_SPECIFIC_ENTITY
		std::cout << " inventory task receiver\n";
#endif
#endif
#ifdef AI_BT_SPECIFIC_ENTITY
		if (index == specificEntity)
		{
			std::cout << " inventory task receiver\n";
		}
#endif
		m_BehaviorTree->dwarfManager->AddInventoryTaskPathToReceiver(index);
		break;
	default: 
		;
	}

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->hasSucceeded[index] = true;
}
}
