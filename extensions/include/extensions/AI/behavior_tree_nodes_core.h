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
	class Node;

	/**
	 * \brief Represent all type of node available 
	 */
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
		PUT_RESOURCE_LEAF,
		FIND_PATH_TO_LEAF
	};

	/**
	 * \brief Used by FindPathNode
	 */
	enum class NodeDestination : unsigned char
	{
		RANDOM,
		DWELLING,
		WORKING_PLACE,
		INVENTORY_TASK_GIVER,
		INVENTORY_TASK_RECEIVER,
		LENGTH
	};

	/**
	 * \brief Status of nodes
	 */
	enum NodeStatus : unsigned char
	{
		SUCCESS,
		FAIL,
		RUNNING
	};

	enum FlowDirection : unsigned char
	{
		UP,
		DOWN,
		NEXT,
		NONE
	};

#pragma region nodeDatas
	struct NodeData
	{
		std::shared_ptr<Node> next = nullptr;
	};

	struct CompositeData : NodeData
	{
		std::vector<std::shared_ptr<Node>> children;
	};

	struct DecoratorData : NodeData
	{
		std::shared_ptr<Node> child;
	};

	struct RepeaterData : DecoratorData
	{
		int limit = 0;
	};

	struct FindPathToData : NodeData
	{
		NodeDestination destination;
	};
#pragma endregion

	/**
	 * \author Nicolas Schneider
	 */
	class Node
	{
	public:
		using Entity = unsigned int;

		/**
		 * \brief shared pointer of node
		 */
		using ptr = std::shared_ptr<Node>;

		/**
		 * \brief Constructor
		 * \param bt behavior tree. Used to store data
		 * \param parentNode, if null => is root node
		 * \param type
		 */
		Node(BehaviorTree* bt, ptr parentNode, NodeType type);
		~Node();

		/**
		 * \brief Called to destroy the node
		 */
		void Destroy();

		/**
		 * \brief Add a new child to a node of it's a composite or a decorator
		 * \param type of new node
		 */
		void AddChild(NodeType type);

		/**
		 * \brief execute the node
		 * \param index of the dwarf
		 */
		void Execute(unsigned int index) const;

		std::unique_ptr<NodeData> data;

		NodeType nodeType;

		std::function<void(unsigned int)> executeFunction;

		std::vector<Entity> nextEntity;
		int indexNextEntity = 0;
		std::vector<Entity> currentEntity;
		int indexCurrentEntity = 0;

		std::vector<NodeStatus> previousStatus;
		std::vector<NodeStatus> nextStatus;

		std::vector<FlowDirection> previousFlowDirection;
		std::vector<FlowDirection> nextFlowDirection;

		std::vector<ptr> nextNode;

		void Resize(int newSize);

		void FollowFlow();

		void UpdateFlow();

		void AddEntity(Entity e);
		void AddEntity(Entity e, NodeStatus status, FlowDirection flow);
	protected:

		void DestroyChild(Node* childNode);

#pragma region Core nodes
		void SequenceComposite(unsigned int index);

		void SelectorComposite(unsigned int index) const;

		void RepeaterDecorator(unsigned int index);

		void RepeatUntilFailDecorator(unsigned int index) const;

		void InverterDecorator(unsigned int index) const;

		void SucceederDecorator(unsigned int index) const;
#pragma endregion 

#pragma region Extensions nodes
		void WaitForPath(unsigned int index);

		void MoveToLeaf(unsigned int index);

		void HasDwellingLeaf(unsigned int index) const;

		void SetDwellingLeaf(unsigned int index) const;

		void EnterDwellingLeaf(unsigned int index) const;

		void ExitDwellingLeaf(unsigned int index) const;

		void EnterWorkingPlaceLeaf(unsigned int index) const;

		void ExitWorkingPlaceLeaf(unsigned int index) const;

		void HasJobLeaf(unsigned int index) const;

		void HasStaticJobLeaf(unsigned int index) const;

		void AssignJobLeaf(unsigned int index) const;

		void IsDayTimeLeaf(unsigned int index) const;

		void IsNightTimeLeaf(unsigned int index) const;

		void WaitDayTimeLeaf(unsigned int index) const;

		void WaitNightTimeLeaf(unsigned int index) const;

		void AskInventoryTaskLeaf(unsigned int index) const;

		void TakeResourcesLeaf(unsigned int index) const;

		void PutResourcesLeaf(unsigned int index) const;

		void FindPathToLeaf(unsigned int index);
#pragma endregion 

#pragma region Datas
		BehaviorTree* m_BehaviorTree;
		ptr m_ParentNode;
#pragma endregion 
	};

	enum class NodeGroup : char
	{
		NONE = 0,
		LEAF = 1 << 0,
		COMPOSITE = 1 << 1,
		DECORATOR = 1 << 2,
	};
}

#endif
