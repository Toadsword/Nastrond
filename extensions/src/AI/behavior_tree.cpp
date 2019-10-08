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

BehaviorTree::~BehaviorTree()
{
#ifdef AI_DEBUG_COUNT_TIME
		std::cout << "[BehaviorTree]Update: " << m_TimerMilli / m_TimerCounter << "," << m_TimerMicro / m_TimerCounter
			<< "\n";
#endif
}

void BehaviorTree::Init()
{
	dwarfManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<DwarfManager>("DwarfManager");

	m_ThreadPool = &m_Engine.GetThreadPool();
}

void BehaviorTree::Update(float dt)
{
#ifdef AI_DEBUG_COUNT_TIME
	const auto t1 = std::chrono::high_resolution_clock::now();
#endif

	//Pre batch active / sleeping entities
	for(size_t i = 0; i < m_Entities->size(); i++)
	{
		if(!sleepingEntity[i] && m_Entities->at(i) != INVALID_ENTITY){
			m_ActiveEntity[m_IndexActiveEntity] = i;
			m_IndexActiveEntity++;
		}
	}

	const auto coreNmb = m_ThreadPool->size();

	if (m_IndexActiveEntity > 0) {
		UpdateRange(0, m_IndexActiveEntity - 1);
	}
	/*else {
		std::vector<std::future<void>> joinFutures(coreNmb);
		for (auto threadIndex = 0; threadIndex < coreNmb; threadIndex++)
		{
			auto start = (threadIndex + 1)*m_IndexActiveEntity / (coreNmb + 1);
			auto end = (threadIndex + 2)*m_IndexActiveEntity / (coreNmb + 1) - 1;
			auto updateFunction = std::bind(&BehaviorTree::UpdateRange, this, start, end);
			joinFutures[threadIndex] = m_ThreadPool->push(updateFunction);
		}
		UpdateRange(0, m_IndexActiveEntity / (coreNmb + 1) - 1);
		for (auto i = 0; i < coreNmb; i++)
		{
			joinFutures[i].get();
		}
	}*/

	m_IndexActiveEntity = 0;

#ifdef AI_DEBUG_COUNT_TIME
	const auto t2 = std::chrono::high_resolution_clock::now();
	const auto timerDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	m_TimerMilli += timerDuration / 1000;
	m_TimerMicro += timerDuration % 1000;
	m_TimerCounter++;
#endif
}

void BehaviorTree::FixedUpdate() { }

void BehaviorTree::Draw() { }

void BehaviorTree::UpdateRange(const int startIndex, const int endIndex)
{
	for (auto i = startIndex; i <= endIndex; i++)
	{
		const auto index = m_ActiveEntity[i];
		
		currentNode[index]->executeFunction(index);
	}
}

void BehaviorTree::SetRootNode(const Node::ptr& rootNode)
{
	m_RootNode = rootNode;

	for (size_t i = 0; i < m_Entities->size(); i++) {
		currentNode[i] = m_RootNode;
	}
}

void BehaviorTree::SetEntities(std::vector<Entity>* vectorEntities)
{
	m_Entities = vectorEntities;

	if (currentNode.size() < m_Entities->size())
	{
		currentNode.resize(m_Entities->size(), m_RootNode);
		doesFlowGoDown.resize(m_Entities->size(), true);
		hasSucceeded.resize(m_Entities->size(), true);

		m_ActiveEntity.resize(m_Entities->size(), true);
		sleepingEntity.resize(m_Entities->size(), false);
	}

	if(m_RootNode != nullptr)
	{
		std::vector<Node::ptr> openNodes;
		openNodes.push_back(m_RootNode);

		while(!openNodes.empty())
		{
			const auto node = openNodes.back();
			openNodes.pop_back();

			switch (node->nodeType) { 
				case NodeType::SEQUENCE_COMPOSITE:
				case NodeType::SELECTOR_COMPOSITE:
				{
					auto* compositeData = static_cast<CompositeData*>(node->data.get());

					compositeData->activeChild.resize(m_Entities->size());

					for (auto child : compositeData->children)
					{
						openNodes.push_back(child);
					}
				}
				break;
				case NodeType::REPEATER_DECORATOR:
				{
					auto* repeaterData = static_cast<RepeaterData*>(node->data.get());
					openNodes.push_back(repeaterData->child);
					repeaterData->count.resize(m_Entities->size(), 0);
				}
				break;
				case NodeType::REPEAT_UNTIL_FAIL_DECORATOR: 
				case NodeType::SUCCEEDER_DECORATOR: 
				case NodeType::INVERTER_DECORATOR:
				{
					auto* decoratorData = static_cast<DecoratorData*>(node->data.get());
					openNodes.push_back(decoratorData->child);
				}
				break;
				default: ; }
		}
	}
}

void BehaviorTree::WakeUpEntities(std::vector<int>& entitiesIndex, const int maxIndex)
{
	for (size_t i = 0; i < maxIndex; ++i)
	{
		sleepingEntity[entitiesIndex[i]] = false;
	}
}
}
