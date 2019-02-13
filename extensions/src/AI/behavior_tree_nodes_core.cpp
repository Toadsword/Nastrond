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
Node::Node(BehaviorTree* bt, ptr parentNode)
{
	m_BehaviorTree = bt;
	m_ParentNode = std::move(parentNode);
}

RepeaterDecorator::RepeaterDecorator(BehaviorTree* bt, const ptr& parentNode, const int limit) : DecoratorNode(bt, parentNode)
{
	m_Limit = limit;
}

void RepeaterDecorator::Execute(const unsigned int index)
{

#ifdef BT_AOS
	//If flow goes down => start counter 
	if (m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown)
	{
		m_BehaviorTree->dataBehaviorTree[index].repeaterCounter = 0;
	}
	else
	{
		//If limit == 0 => infinity, if m_Counter == m_Limit it's over
		if (m_Limit > 0 && ++m_BehaviorTree->dataBehaviorTree[index].repeaterCounter == m_Limit)
		{
			m_BehaviorTree->dataBehaviorTree[index].repeaterCounter = 0;

			m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
			m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

			m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
			return;
		}

		if (m_Limit > 0) {}
	}

	//Switch current node to child
	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoDown;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_Child;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
#endif

#ifdef BT_SOA
	//If flow goes down => start counter 
	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		m_BehaviorTree->repeaterCounter[index] = 0;
	}
	else
	{
		//If limit == 0 => infinity, if m_Counter == m_Limit it's over
		if (m_Limit > 0 && ++m_BehaviorTree->repeaterCounter[index] == m_Limit)
		{
			m_BehaviorTree->repeaterCounter[index] = 0;

			m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
			m_BehaviorTree->currentNode[index] = m_ParentNode;

			m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
			return;
		}

		if (m_Limit > 0) { }
	}

	//Switch current node to child
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoDown;
	m_BehaviorTree->currentNode[index] = m_Child;

	m_BehaviorTree->previousStatus[index] = Status::RUNNING;
#endif
}

void RepeatUntilFailDecorator::Execute(const unsigned int index)
{
#ifdef BT_AOS
	if (m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown)
	{
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_Child;

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
	}
	else
	{
		if (m_BehaviorTree->dataBehaviorTree[index].previousStatus == Status::FAIL)
		{
			m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;
			m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
		}
		else
		{
			m_BehaviorTree->dataBehaviorTree[index].currentNode = m_Child;
			m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
		}
	}
#endif

#ifdef BT_SOA
	if(m_BehaviorTree->doesFlowGoDown[index])
	{
		m_BehaviorTree->currentNode[index] = m_Child;

		m_BehaviorTree->previousStatus[index] = Status::RUNNING;
	}
	else
	{
		if(m_BehaviorTree->previousStatus[index] == Status::FAIL)
		{
			m_BehaviorTree->currentNode[index] = m_ParentNode;
			m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
		}else
		{
			m_BehaviorTree->currentNode[index] = m_Child;
			m_BehaviorTree->previousStatus[index] = Status::RUNNING;
		}
	}
#endif
}

void InverterDecorator::Execute(const unsigned int index)
{
#ifdef BT_AOS
	if (m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown)
	{
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_Child;

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
		return;
	}

	if (m_BehaviorTree->dataBehaviorTree[index].previousStatus == Status::SUCCESS)
	{
		m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;
		return;
	}

	if (m_BehaviorTree->dataBehaviorTree[index].previousStatus == Status::FAIL)
	{
		m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
		return;
	}

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;
#endif

#ifdef BT_SOA
	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		m_BehaviorTree->currentNode[index] = m_Child;

		m_BehaviorTree->previousStatus[index] = Status::RUNNING;
		return;
	}

	if (m_BehaviorTree->previousStatus[index] == Status::SUCCESS)
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;

		m_BehaviorTree->previousStatus[index] = Status::FAIL;
		return;
	}

	if (m_BehaviorTree->previousStatus[index] == Status::FAIL)
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;

		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
		return;
	}

	m_BehaviorTree->previousStatus[index] = Status::FAIL;
#endif
}

void CompositeNode::AddChild(const ptr& child)
{
	m_Children.push_back(child);
}

bool CompositeNode::HasChildren() const
{
	return !m_Children.empty();
}

void SequenceComposite::Execute(const unsigned int index)
{
#ifdef BT_AOS
		//if last one returned fail => then it's a fail
		if (m_BehaviorTree->dataBehaviorTree[index].previousStatus == Status::FAIL)
		{
			m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

			m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;
			return;
		}

	//If last one is parent => first time entering the sequence
	if (m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown)
	{
		m_BehaviorTree->dataBehaviorTree[index].sequenceActiveChild = 0;
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_Children[0];

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
		return;
	}

	//Else it means that the previous node is a children
	for (size_t i = 0; i < m_Children.size(); i++)
	{
		if (i == m_BehaviorTree->dataBehaviorTree[index].sequenceActiveChild)
		{
			if (i == m_Children.size() - 1)
			{
				m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

				m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
				return;
			}
			else
			{
				m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoDown;
				m_BehaviorTree->dataBehaviorTree[index].sequenceActiveChild++;
				m_BehaviorTree->dataBehaviorTree[index].currentNode = m_Children[i + 1];

				m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
				return;
			}
		}
	}
	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
#endif

#ifdef BT_SOA
	//if last one returned fail => then it's a fail
	if (m_BehaviorTree->previousStatus[index] == Status::FAIL)
	{
		m_BehaviorTree->currentNode[index] = m_ParentNode;

		m_BehaviorTree->previousStatus[index] = Status::FAIL;
		return;
	}

	//If last one is parent => first time entering the sequence
	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		m_BehaviorTree->sequenceActiveChild[index] = 0;
		m_BehaviorTree->currentNode[index] = m_Children[0];

		m_BehaviorTree->previousStatus[index] = Status::RUNNING;
		return;
	}

	//Else it means that the previous node is a children
	for (size_t i = 0; i < m_Children.size(); i++)
	{
		if (i == m_BehaviorTree->sequenceActiveChild[index])
		{
			if (i == m_Children.size() - 1)
			{
				m_BehaviorTree->currentNode[index] = m_ParentNode;

				m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
				return;
			}
			else
			{
				m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoDown;
				m_BehaviorTree->sequenceActiveChild[index]++;
				m_BehaviorTree->currentNode[index] = m_Children[i + 1];

				m_BehaviorTree->previousStatus[index] = Status::RUNNING;
				return;
			}
		}
	}
	m_BehaviorTree->previousStatus[index] = Status::RUNNING;
#endif
}

void SelectorComposite::Execute(const unsigned int index)
{
#ifdef BT_AOS
	//If last one is parent => first time entering the sequence
	if (m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown)
	{
		m_BehaviorTree->dataBehaviorTree[index].sequenceActiveChild = 0;
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_Children[0];

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
		return;
	}

	//Else it means that the previous node is a children
	for (size_t i = 0; i < m_Children.size(); i++)
	{
		if (i == m_BehaviorTree->dataBehaviorTree[index].sequenceActiveChild)
		{
			//If last one is a success => going out of node
			if (m_BehaviorTree->dataBehaviorTree[index].previousStatus == Status::SUCCESS)
			{
				m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

				m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
				return;
			}

			//Else if not last child => go next child
			if (i < m_Children.size() - 1)
			{
				m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoDown;
				m_BehaviorTree->dataBehaviorTree[index].currentNode = m_Children[i + 1];
				m_BehaviorTree->dataBehaviorTree[index].sequenceActiveChild++;

				m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
				return;
			}
			else //mean that they all failed => return fail
			{
				m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

				m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;
				return;
			}
		}
	}
	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
#endif

#ifdef BT_SOA
	//If last one is parent => first time entering the sequence
	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		m_BehaviorTree->sequenceActiveChild[index] = 0;
		m_BehaviorTree->currentNode[index] = m_Children[0];

		m_BehaviorTree->previousStatus[index] = Status::RUNNING;
		return;
	}

	//Else it means that the previous node is a children
	for (size_t i = 0; i < m_Children.size(); i++)
	{
		if (i == m_BehaviorTree->sequenceActiveChild[index])
		{
			//If last one is a success => going out of node
			if (m_BehaviorTree->previousStatus[index] == Status::SUCCESS)
			{
				m_BehaviorTree->currentNode[index] = m_ParentNode;

				m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
				return;
			}

			//Else if not last child => go next child
			if (i < m_Children.size() - 1)
			{
				m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoDown;
				m_BehaviorTree->currentNode[index] = m_Children[i + 1];
				m_BehaviorTree->sequenceActiveChild[index]++;

				m_BehaviorTree->previousStatus[index] = Status::RUNNING;
				return;
			}
			else //mean that they all failed => return fail
			{
				m_BehaviorTree->currentNode[index] = m_ParentNode;

				m_BehaviorTree->previousStatus[index] = Status::FAIL;
				return;
			}
		}
	}
	m_BehaviorTree->previousStatus[index] = Status::RUNNING;
#endif
}

void DecoratorNode::SetChild(const Node::ptr& node)
{
	m_Child = node;
}

bool DecoratorNode::HasChild() const
{
	return m_Child != nullptr;
}

void SucceederDecorator::Execute(const unsigned int index)
{
#ifdef BT_AOS
	if (m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown)
	{
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_Child;

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
		return;
	}

	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;
	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	if (m_BehaviorTree->doesFlowGoDown[index])
	{
		m_BehaviorTree->currentNode[index] = m_Child;

		m_BehaviorTree->previousStatus[index] = Status::RUNNING;
		return;
	}

	m_BehaviorTree->currentNode[index] = m_ParentNode;
	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}
}
