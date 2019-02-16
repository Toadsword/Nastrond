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
#ifndef SFGE_EXT_BEHAVIOR_TREE_NODES_CORE_H
#define SFGE_EXT_BEHAVIOR_TREE_NODES_CORE_H

#include <vector>
#include <string>

namespace sfge::ext::behavior_tree
{
	class BehaviorTree;

	/**
	 * \author Nicolas Schneider
	 */
	class Node final
	{
	public:
		/**
		 * \brief shared pointer of node
		 */
		using ptr = std::shared_ptr<Node>;

		explicit Node(BehaviorTree* bt, ptr parentNode);

		/**
		 * \brief Status of nodes
		 */
		enum class Status : unsigned char
		{
			SUCCESS,
			FAIL,
			RUNNING
		};

		/**
		 * \brief used by FindPathNode
		 */
		enum class Destination : unsigned char 
		{
			RANDOM,
			DWELLING,
			WORKING_PLACE,
			INVENTORY_TASK_GIVER,
			INVENTORY_TASK_RECEIVER
		};

		enum class NodeType : unsigned char
		{
			SEQUENCE_COMPOSITE,
			SELECTOR_COMPOSITE,
			REPEATER_DECORATOR,
			REPEAT_UNTIL_FAIL_DECORATOR,
			SUCCEEDER_DECORATOR,
			INVERTER_DECORATOR,
			WAIT_FOR_PATH_LEAF,
			MOVE_TO_LEAF,
			FIND_RANDOM_PATH_LEAF,
			HAS_DWELLING_LEAF,
			SET_DWELLING_LEAF,
			ENTER_DWELLING_LEAF,
			EXIT_DWELLING_LEAF,
			ENTER_WORKING_PLACE_LEAF,
			EXIT_WORKING_PLACE_LEAF,
			HAS_JOB_LEAF,
			HAS_STATIC_JOB_LEAF,
			ASSIGN_JOB_LEAF,
			IS_DAY_TIME_LEAF,
			IS_NIGHT_TIME_LEAF,
			WAIT_DAY_TIME_LEAF,
			WAIT_NIGHT_TIME_LEAF,
			ASK_INVENTORY_TASK_LEAF,
			TAKE_RESOURCE_LEAF,
			FIND_PATH_TO_LEAF
		};

		NodeType nodeType;

		struct Data
		{
		};

		struct CompositeData : Data
		{
			std::vector<ptr> m_Children;
		};

		struct DecoratorData : Data
		{
			ptr m_Child;
		};

		struct RepeaterData : DecoratorData
		{
			int m_Limit = 0;
		};

		struct FindPathToData : Data
		{
			Destination m_Destination;
		};

		/**
		 * \brief execute the node
		 * \param index of the dwarf
		 */
		void Execute(unsigned int index);

		std::unique_ptr<Data> m_Datas;

	protected:
		void SequenceComposite(unsigned int index);

		void SelectorComposite(unsigned int index);

		void RepeaterDecorator(unsigned int index);

		void RepeatUntilFailDecorator(unsigned int index);

		void InverterDecorator(unsigned int index);

		void SucceederDecorator(unsigned int index);

		void WaitForPath(unsigned int index);

		void MoveToLeaf(unsigned int index);

		void FindRandomPathLeaf(unsigned int index);

		void HasDwellingLeaf(unsigned int index);

		void SetDwellingLeaf(unsigned int index);

		void EnterDwellingLeaf(unsigned int index);

		void ExitDwellingLeaf(unsigned int index);

		void EnterWorkingPlaceLeaf(unsigned int index);

		void ExitWorkingPlaceLeaf(unsigned int index);

		void HasJobLeaf(unsigned int index);

		void HasStaticJobLeaf(unsigned int index);

		void AssignJobLeaf(unsigned int index);

		void IsDayTimeLeaf(unsigned int index);

		void IsNightTimeLeaf(unsigned int index);

		void WaitDayTimeLeaf(unsigned int index);

		void WaitNightTimeLeaf(unsigned int index);

		void AskInventoryTaskLeaf(unsigned int index);

		void TakeResourcesLeaf(unsigned int index);

		void PutResourcesLeaf(unsigned int index);

		void FindPathToLeaf(unsigned int index);

		BehaviorTree* m_BehaviorTree;
		ptr m_ParentNode;
	};

	enum class NodeType : char
	{
		NONE = 0,
		LEAF = 1 << 0,
		COMPOSITE = 1 << 1,
		DECORATOR = 1 << 2,
	};
}

#endif
