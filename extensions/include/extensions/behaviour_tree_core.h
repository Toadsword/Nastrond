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

class Node {
public:
	using ptr = std::shared_ptr<Node>;

	explicit Node(BehaviourTree* bt, ptr parentNode);
	virtual ~Node() = default;
	
	enum class Status {
		SUCCESS,
		FAIL,
		RUNNING
	};

	virtual Status Execute(unsigned int index) = 0;

protected:
	BehaviourTree* m_BehaviourTree;

	ptr m_ParentNode;
};

class BehaviourTree final : public System {
public:
	explicit BehaviourTree(Engine& engine);

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

	void SetRootNode(const Node::ptr& rootNode);

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

class CompositeNode : public Node {
public:
	explicit CompositeNode(BehaviourTree* bt, const ptr& parentNode) : Node(bt, parentNode){}

	void AddChild(const ptr& child);
	bool HasChildren() const;
protected:
	std::vector<ptr> m_Children;
};

class Sequence final : public CompositeNode {
public:
	Sequence(BehaviourTree* bt, const ptr& parentNode) : CompositeNode(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

class Selector final : public CompositeNode {
public:
	Selector(BehaviourTree* bt, const ptr& parentNode) : CompositeNode(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

class Decorator : public Node {
public:

	explicit Decorator(BehaviourTree* bt, const ptr& parentNode) : Node(bt, parentNode) { }

	void SetChild(const Node::ptr& node);
	bool HasChild() const;

protected:
	ptr m_Child = nullptr;
};

class Repeater : public Decorator
{
public:
	Repeater(BehaviourTree* bt, const ptr& parentNode, int limit = 0);
	
	Status Execute(unsigned int index) override;

private:
	int m_Limit;
};

class Inverter : public Decorator
{
public:
	Inverter(BehaviourTree* bt, const ptr& parentNode) : Decorator(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

class Succeeder : public Decorator
{
public:
	Succeeder(BehaviourTree* bt, const ptr& parentNode) : Decorator(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

class Leaf : public Node {
public:
	explicit Leaf(BehaviourTree* bt, const ptr& parentNode) : Node(bt, parentNode){}
};
}

#endif
