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
#ifndef SFGE_EXT_BEHAVIOUR_TREE_CORE_H
#define SFGE_EXT_BEHAVIOUR_TREE_CORE_H

#include <vector>
#include <memory>

#include <engine/system.h>
#include <engine/globals.h>

#include <extensions/dwarf_manager.h>
#include <extensions/dwelling_manager.h>

namespace sfge::ext::behaviour_tree
{
class BehaviourTree;

/**
 * author Nicolas Schneider
 */
class Node {
public:
	/**
	 * \brief shared pointer of node
	 */
	using ptr = std::shared_ptr<Node>;

	explicit Node(BehaviourTree* bt, ptr parentNode);
	virtual ~Node() = default;
	
	/**
	 * \brief Status of nodes
	 */
	enum class Status {
		SUCCESS,
		FAIL,
		RUNNING
	};

	/**
	 * \brief execute the node
	 * \param index of the dwarf
	 * \return 
	 */
	virtual Status Execute(unsigned int index) = 0;

protected:
	BehaviourTree* m_BehaviourTree;
	ptr m_ParentNode;
};

/**
 * author Nicolas Schneider
 */
class BehaviourTree final : public System {
public:
	explicit BehaviourTree(Engine& engine);

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

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
	
	std::vector<Entity>* entities;

	std::vector<Node::ptr> currentNode;
	std::vector<bool> doesFlowGoDown;
	std::vector<Node::Status> previousStatus;

	std::vector<char> repeaterCounter;
	std::vector<char> sequenceActiveChild;

	DwarfManager* dwarfManager;
	DwellingManager* dwellingManager;

	const bool flowGoDown = true;
	const bool flowGoUp = false;

private:
	Node::ptr m_RootNode = nullptr;
};

/**
* author Nicolas Schneider
*/
class CompositeNode : public Node {
public:
	explicit CompositeNode(BehaviourTree* bt, const ptr& parentNode) : Node(bt, parentNode){}

	/**
	 * \brief Add child to composite node
	 * \param child 
	 */
	void AddChild(const ptr& child);

	/**
	 * \brief check if the node as the child
	 * \return 
	 */
	bool HasChildren() const;
protected:
	std::vector<ptr> m_Children;
};

/**
* author Nicolas Schneider
*/
class Sequence final : public CompositeNode {
public:
	Sequence(BehaviourTree* bt, const ptr& parentNode) : CompositeNode(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class Selector final : public CompositeNode {
public:
	Selector(BehaviourTree* bt, const ptr& parentNode) : CompositeNode(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class Decorator : public Node {
public:

	explicit Decorator(BehaviourTree* bt, const ptr& parentNode) : Node(bt, parentNode) { }

	/**
	 * \brief Set the child of the decorator node
	 * \param node 
	 */
	void SetChild(const Node::ptr& node);

	/**
	 * \brief Check if has child
	 * \return 
	 */
	bool HasChild() const;

protected:
	ptr m_Child = nullptr;
};

/**
* author Nicolas Schneider
*/
class Repeater final : public Decorator
{
public:
	Repeater(BehaviourTree* bt, const ptr& parentNode, int limit = 0);
	
	Status Execute(unsigned int index) override;

private:
	int m_Limit;
};

/**
* author Nicolas Schneider
*/
class Inverter final : public Decorator
{
public:
	Inverter(BehaviourTree* bt, const ptr& parentNode) : Decorator(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class Succeeder final : public Decorator
{
public:
	Succeeder(BehaviourTree* bt, const ptr& parentNode) : Decorator(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class Leaf : public Node {
public:
	explicit Leaf(BehaviourTree* bt, const ptr& parentNode) : Node(bt, parentNode){}
};
}

#endif
