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
#ifdef AI_DEBUG_COUNT_TIME
	auto t1 = std::chrono::high_resolution_clock::now();
#endif

	//Pre batch active / sleeping entites
	for(auto i = 0; i < m_Entities->size(); i++)
	{
		if(!sleepingEntity[i] && m_Entities->at(i) != INVALID_ENTITY){
			activeEntity[indexActiveEntity] = i;
			indexActiveEntity++;
		}
	}

	auto& threadPool = m_Engine.GetThreadPool();
	const auto coreNmb = threadPool.size();

	if (indexActiveEntity < coreNmb) {
		UpdateRange(0, indexActiveEntity - 1);
	}
	else {

		std::vector<std::future<void>> joinFutures(coreNmb);
		for (auto threadIndex = 0; threadIndex < coreNmb; threadIndex++)
		{
			int start = (threadIndex + 1)*indexActiveEntity / (coreNmb + 1);
			int end = (threadIndex + 2)*indexActiveEntity / (coreNmb + 1) - 1;
			auto updateFunction = std::bind(&BehaviorTree::UpdateRange, this, start, end);
			joinFutures[threadIndex] = threadPool.push(updateFunction);
		}
		UpdateRange(0, indexActiveEntity / (coreNmb + 1) - 1);
		for (int i = 0; i < coreNmb; i++)
		{
			joinFutures[i].get();
		}
	}

	indexActiveEntity = 0;

#ifdef AI_DEBUG_COUNT_TIME
	auto t2 = std::chrono::high_resolution_clock::now();
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
#ifdef BT_SOA
		const auto index = activeEntity[i];
		
		currentNode[index]->executeFunction(index);
#endif

#ifdef BT_AOS
		dataBehaviorTree[i].currentNode->Execute(i);
#endif
	}
}

void BehaviorTree::SetRootNode(const Node::ptr& rootNode)
{
	m_RootNode = rootNode;

	for (auto i = 0; i < m_Entities->size(); i++) {
		currentNode[i] = m_RootNode;
	}
}

void BehaviorTree::SetEntities(std::vector<Entity>* vectorEntities)
{
	m_Entities = vectorEntities;
#ifdef BT_SOA
	if (currentNode.size() < m_Entities->size())
	{
		currentNode.resize(m_Entities->size(), m_RootNode);
		doesFlowGoDown.resize(m_Entities->size(), true);
		repeaterCounter.resize(m_Entities->size(), 0);
		sequenceActiveChild.resize(m_Entities->size(), 0);
		hasSucceeded.resize(m_Entities->size(), true);

		activeEntity.resize(m_Entities->size(), true);
		sleepingEntity.resize(m_Entities->size(), false);
#endif

#ifdef BT_AOS
		if (dataBehaviorTree.size() < m_Entities->size())
		{
			int previousSize = dataBehaviorTree.size();
			dataBehaviorTree.resize(m_Entities->size());

			for (auto i = previousSize; i < dataBehaviorTree.size(); i++)
			{
				dataBehaviorTree[i].currentNode = m_RootNode;
			}
#endif

		}
}
}
