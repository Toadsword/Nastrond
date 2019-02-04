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

#include <engine/system.h>
#include <vector>
#include <memory>
#include <engine/globals.h>

#include <extensions/dwarf_manager.h>
#include <extensions/dwelling_manager.h>

namespace sfge::ext::behaviour_tree
{

class BehaviourTree;

class Node {
public:
	using ptr = std::shared_ptr<Node>;

	explicit Node(BehaviourTree* BT, ptr parentNode);
	virtual ~Node() = default;

	enum class Status {
		SUCCESS,
		FAIL,
		RUNNING
	};

	virtual Status Execute(unsigned int index) = 0;

protected:
	BehaviourTree* behaviourTree;

	ptr m_parentNode;
};

class BehaviourTree : public System {
public:
	BehaviourTree(Engine& engine);

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

	void SetRootNode(const Node::ptr& rootNode);

	void SetEntities(std::vector<Entity>* entities);
	
	std::vector<Entity>* m_Entities;

	std::vector<Node::ptr> m_CurrentNode;
	std::vector<bool> m_FlowGoesDown;
	std::vector<Node::Status> m_PreviousStatus;

	std::vector<char> m_Counter;
	std::vector<char> m_Child;

	DwarfManager* dwarfManager;
	DwellingManager* dwellingManager;

	const bool FLOW_GOES_DOWN = true;
	const bool FLOW_GOES_UP = false;

private:
	Node::ptr m_RootNode = nullptr;
};

class CompositeNode : public Node {
public:
	explicit CompositeNode(BehaviourTree* BT, ptr parentNode) : Node(BT, parentNode){}

	void AddChild(ptr child) { m_Children.push_back(child); }
	bool HasChildren() const { return !m_Children.empty(); }
protected:
	std::vector<ptr> m_Children;
};

class Sequence : public CompositeNode {
public:
	Sequence(BehaviourTree* BT, ptr parentNode) : CompositeNode(BT, parentNode) {}

	Status Execute(unsigned int index) override;
};

class Selector : public CompositeNode {
public:
	Selector(BehaviourTree* BT, ptr parentNode) : CompositeNode(BT, parentNode) {}
	~Selector() = default;

	Status Execute(unsigned int index) override;
};

class Decorator : public Node {
public:

	explicit Decorator(BehaviourTree* BT, ptr parentNode) : Node(BT, parentNode) { }

	void SetChild(Node::ptr node) { m_Child = node; }
	bool HasChild() const { return m_Child != nullptr; }

protected:
	ptr m_Child = nullptr;
};

class Repeater : public Decorator
{
public:
	Repeater(BehaviourTree* BT, ptr parentNode, int limit = 0);
	
	Status Execute(unsigned int index) override;

private:
	int m_Limit;
};

class Inverter : public Decorator
{
public:
	Inverter(BehaviourTree* BT, ptr parentNode) : Decorator(BT, parentNode) {}

	Status Execute(unsigned int index) override;
};

class Succeeder : public Decorator
{
public:
	Succeeder(BehaviourTree* BT, ptr parentNode) : Decorator(BT, parentNode) {}

	Status Execute(unsigned index) override;
};

class Leaf : public Node {
public:
	explicit Leaf(BehaviourTree* BT, ptr parentNode) : Node(BT, parentNode){}
};
}

#endif
