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
#ifndef SFGE_EXT_BEHAVIOR_TREE_H
#define SFGE_EXT_BEHAVIOR_TREE_H

#define AI_DEBUG_COUNT_TIME

#include <vector>
#include <memory>

#include <engine/system.h>
#include <utility/json_utility.h>
#include <engine/globals.h>

#include <extensions/dwarf_manager.h>
#include <extensions/AI/behavior_tree_nodes_core.h>

namespace sfge::ext::behavior_tree
{
#define BT_SOA
//#define BT_AOS

/**
* author Nicolas Schneider
*/
class BehaviorTree final : public System
{
public:
	explicit BehaviorTree(Engine& engine);

	~BehaviorTree()
	{
#ifdef AI_DEBUG_COUNT_TIME
		std::cout << "[BehaviorTree]Update: " << m_TimerMilli / m_TimerCounter  << "," << m_TimerMicro / m_TimerCounter << "\n";
#endif
	}

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

	void UpdateRange(int startIndex, int endIndex);

	/**
	* \brief Set root node of the behaviour tree
	* \param rootNode
	*/
	void SetRootNode(const Node::ptr& rootNode);

	/**
	* \brief Set array of entity that use the behaviour tree
	* \param vectorEntities
	*/
	void SetEntities(std::vector<Entity>* vectorEntities);

#ifdef BT_AOS
	/**
	 * \brief All data regarding flow in behavior tree
	 */
	struct DataBehaviorTree final
	{
		Node::ptr currentNode; //4

		NodeStatus previousStatus; //1
		bool doesFlowGoDown; // 1
		unsigned char repeaterCounter; // 1
		unsigned char sequenceActiveChild; // 1
	};

	std::vector<DataBehaviorTree> dataBehaviorTree;
#endif

#ifdef BT_SOA
	std::vector<Node::ptr> currentNode;
	std::vector<bool> doesFlowGoDown;
	std::vector<NodeStatus> previousStatus;

	std::vector<char> repeaterCounter;
	std::vector<char> sequenceActiveChild;
#endif

	DwarfManager* dwarfManager;

	const bool flowGoDown = true;
	const bool flowGoUp = false;

private:
	Node::ptr m_RootNode = nullptr;

	std::vector<Entity>* m_Entities;

#ifdef AI_DEBUG_COUNT_TIME
	unsigned int m_TimerMilli = 0u;
	unsigned int m_TimerMicro = 0u;
	int m_TimerCounter = 0;
#endif
};
}

#endif