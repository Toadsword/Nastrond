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
BehaviorTree::BehaviorTree(sfge::Engine& engine) : System(engine) {}

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

void BehaviorTree::SetRootNode(const Node::ptr& rootNode)
{
	m_RootNode = rootNode;
}

void BehaviorTree::SetEntities(std::vector<Entity>* vectorEntities)
{
	m_Entities = vectorEntities;
}
}
