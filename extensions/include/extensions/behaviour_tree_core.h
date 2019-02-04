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
#include <unordered_map>
#include <engine/vector.h>
#include <engine/globals.h>
#include <extensions/dwarf_manager.h>
#include <extensions/dwelling_manager.h>

namespace sfge::ext::behaviour_tree
{

class BehaviourTree;

class Node {
public:
	enum class Status {
		SUCCESS,
		FAIL,
		RUNNING
	};

	virtual Status Execute(unsigned int index) = 0;

	using ptr = std::shared_ptr<Node>;

	Node::ptr parentNode;

protected:
	BehaviourTree * behaviourTree;
};

class BehaviourTree : public System {
public:
	BehaviourTree(sfge::Engine& engine);

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

	void SetRootNode(const Node::ptr& rootNode);

	void SetEntities(std::vector<Entity>* entities);
	
	std::vector<Entity>* m_Entities;

	std::vector<Node::ptr> m_CurrentNode;
	std::vector<Node::ptr> m_PreviousNode;
	std::vector<Node::Status> m_PreviousStatus;
	std::vector<int> m_Counter;
	DwarfManager* dwarfManager;
	DwellingManager* dwellingManager;

private:

	Node::ptr m_RootNode = nullptr;
};

class CompositeNode : public Node {
public:
	void AddChild(Node::ptr child) { m_Children.push_back(child); }
	bool HasChildren() const { return !m_Children.empty(); }
protected:
	std::vector<Node::ptr> m_Children;
};

class Sequence : public CompositeNode {
public:
	Sequence(BehaviourTree* BT);

	Status Execute(unsigned int index) override;
};

class Selector : public CompositeNode {
public:
	Selector(BehaviourTree* BT);

	Status Execute(unsigned int index) override;
};

class Decorator : public Node {
public:

	void SetChild(Node::ptr node) { m_Child = node; }
	bool HasChild() const { return m_Child != nullptr; }

protected:
	Node::ptr m_Child = nullptr;
};

class Repeater : public Decorator
{
public:
	Repeater(BehaviourTree* BT, int limit = 0);
	
	Status Execute(unsigned int index) override;

private:
	int m_Limit;
};

class Inverter : public Decorator
{
public:
	Inverter(BehaviourTree* BT);

	Status Execute(unsigned int index) override;
};

class Succeeder : public Decorator
{
public:
	Succeeder(BehaviourTree* BT);

	Status Execute(unsigned index) override;
};

class Leaf : public Node {
	virtual Status Execute(unsigned index) override = 0;
};
}

#endif
