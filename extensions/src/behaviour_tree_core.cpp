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
#include <extensions/behaviour_tree_core.h>
#include <python/python_engine.h>

namespace sfge::ext::behaviour_tree
{
BehaviourTree::BehaviourTree(sfge::Engine& engine) : System(engine) 
{
}

void BehaviourTree::Init()
{
	dwarfManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<DwarfManager>("DwarfManager");
	dwellingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<DwellingManager>("DwellingManager");
}

void BehaviourTree::Update(float dt)
{
	if(m_CurrentNode.size() < m_Entities->size())
	{
		m_CurrentNode.resize(m_Entities->size(), m_RootNode);
		m_PreviousNode.resize(m_Entities->size());
		m_PreviousStatus.resize(m_Entities->size());
		m_Counter.resize(m_Entities->size(), 0);
	}

	for(size_t i = 0; i < m_Entities->size(); i++)
	{
		if(m_Entities->at(i) == INVALID_ENTITY) {
			continue;
		}

		m_PreviousStatus[i] = m_CurrentNode[i]->Execute(i);

		if(m_PreviousStatus[i] != Node::Status::RUNNING)
		{
			m_PreviousNode[i] = m_CurrentNode[i];
			m_CurrentNode[i] = m_CurrentNode[i]->parentNode;
		}
	}
}

void BehaviourTree::FixedUpdate()
{
}

void BehaviourTree::Draw()
{
}

void BehaviourTree::SetRootNode(const Node::ptr& rootNode)
{
	m_RootNode = rootNode;
	m_RootNode->parentNode = nullptr;
}

void BehaviourTree::SetEntities(std::vector<Entity>* entities)
{
	m_Entities = entities;
}

Repeater::Repeater(BehaviourTree* BT, int limit)
{
	m_Limit = limit;
	behaviourTree = BT;
}

Node::Status Repeater::Execute(unsigned int index)
{
	if (behaviourTree->m_PreviousNode[index] == parentNode) {
		behaviourTree->m_Counter[index] = 0;
	}
	else {
		//If limit == 0 => inifinity, if m_Counter == m_Limit it's over
		if (m_Limit > 0 && ++behaviourTree->m_Counter[index] == m_Limit)
		{
			behaviourTree->m_Counter[index] = 0;
			return Status::SUCCESS;
		}
		else if (m_Limit > 0) {
		}
	}

	//Switch current node to child
	behaviourTree->m_PreviousNode[index] = behaviourTree->m_CurrentNode[index];
	behaviourTree->m_CurrentNode[index] = m_Child;

	return Status::RUNNING;
}

Inverter::Inverter(BehaviourTree* BT)
{
	behaviourTree = BT;
}

Node::Status Inverter::Execute(unsigned index)
{
	if (behaviourTree->m_PreviousNode[index] == parentNode)
	{
		behaviourTree->m_PreviousNode[index] = behaviourTree->m_CurrentNode[index];
		behaviourTree->m_CurrentNode[index] = m_Child;

		return Status::RUNNING;
	}

	if (behaviourTree->m_PreviousStatus[index] == Status::SUCCESS)
	{
		return Status::FAIL;
	}

	if (behaviourTree->m_PreviousStatus[index] == Status::FAIL)
	{
		return Status::SUCCESS;
	}

	if (behaviourTree->m_PreviousStatus[index] == Status::RUNNING)
	{
		std::cout << "Inverter: THIS IS NOT POSSIBLE\n";
	}
}


Sequence::Sequence(BehaviourTree * BT)
{
	behaviourTree = BT;
}

Node::Status Sequence::Execute(unsigned int index)
{
	//if last one returned fail => then it's a fail
	if (behaviourTree->m_PreviousStatus[index] == Status::FAIL)
	{
		return Status::FAIL;
	}

	//If last one is parent => first time entering the sequence
	if (behaviourTree->m_PreviousNode[index] == parentNode)
	{
		behaviourTree->m_PreviousNode[index] = behaviourTree->m_CurrentNode[index];
		behaviourTree->m_CurrentNode[index] = m_Children[0];
		return Status::RUNNING;
	}

	//Else it means that the previous node is a children
	for (int i = 0; i < m_Children.size(); i++)
	{
		if (m_Children[i] == behaviourTree->m_PreviousNode[index])
		{
			if (i == m_Children.size() - 1)
			{
				return Status::SUCCESS;
			}
			else
			{
				behaviourTree->m_PreviousNode[index] = behaviourTree->m_CurrentNode[index];
				behaviourTree->m_CurrentNode[index] = m_Children[i + 1];
				return Status::RUNNING;
			}
		}
	}
	return Status::RUNNING;
}

Selector::Selector(BehaviourTree* BT) {
	behaviourTree = BT;
}

Node::Status Selector::Execute(unsigned index) {
	//If last one is parent => first time entering the sequence
	if (behaviourTree->m_PreviousNode[index] == parentNode)
	{
		behaviourTree->m_PreviousNode[index] = behaviourTree->m_CurrentNode[index];
		behaviourTree->m_CurrentNode[index] = m_Children[0];
		return Status::RUNNING;
	}

	//Else it means that the previous node is a children
	for (int i = 0; i < m_Children.size(); i++)
	{
		if (m_Children[i] == behaviourTree->m_PreviousNode[index])
		{
			//If last one is a success => going out of node
			if (behaviourTree->m_PreviousStatus[index] == Status::SUCCESS) {
				return Status::SUCCESS;
			}

			//Else if not last child => go next chil
			if (i < m_Children.size() - 1)
			{
				behaviourTree->m_PreviousNode[index] = behaviourTree->m_CurrentNode[index];
				behaviourTree->m_CurrentNode[index] = m_Children[i + 1];
				return Status::RUNNING;
			}
			else //mean that they all failed => return fail
			{
				return Status::FAIL;
			}
		}
	}
	return Status::RUNNING;
}

Succeeder::Succeeder(BehaviourTree * BT)
{
	behaviourTree = BT;
}

Node::Status Succeeder::Execute(unsigned index)
{
	if (behaviourTree->m_PreviousNode[index] == parentNode)
	{
		behaviourTree->m_PreviousNode[index] = behaviourTree->m_CurrentNode[index];
		behaviourTree->m_CurrentNode[index] = m_Child;
		return Status::RUNNING;
	}

	return Status::SUCCESS;
}
}
