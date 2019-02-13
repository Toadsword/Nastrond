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
#include <map>
#include <iostream>

#include <utility/json_utility.h>

namespace sfge::ext::behavior_tree
{
class Node;
class BehaviorTree;

/**
 * \brief Factory to build any node
 * \author Nicolas Schneider
 */
class NodeFactory
{
public:
	virtual std::shared_ptr<Node> Create(BehaviorTree* bt, std::shared_ptr<Node> parentNode, json& nodeJson) = 0;

	static void RegisterType(const std::string& name, NodeFactory* factory) {
		if (m_Factories.find(name) == m_Factories.end()) {
			std::cout << "Register : " << name << "\n";
			m_Factories[name] = factory;
		}
	}

	static NodeFactory* GetFactory(const std::string& name)
	{
		return m_Factories[name];
	}

private:
	inline static std::map<std::string, NodeFactory*> m_Factories;
};

#define REGISTER_NODE_TYPE(klass) \
    class klass##Factory : public NodeFactory { \
    public: \
        klass##Factory() \
        { \
            RegisterType(#klass, this); \
        } \
        virtual std::shared_ptr<Node> Create(BehaviorTree* bt, std::shared_ptr<Node> parentNode, json& nodeJson) { \
            return std::make_shared<klass>(bt, parentNode, nodeJson); \
        } \
    }; \
    static klass##Factory global_##klass##Factory;

/**
 * author Nicolas Schneider
 */
class Node
{
public:
	/**
	 * \brief shared pointer of node
	 */
	using ptr = std::shared_ptr<Node>;

	explicit Node(BehaviorTree* bt, ptr parentNode);
	virtual ~Node() = default;

	/**
	 * \brief Status of nodes
	 */
	enum class Status
	{
		SUCCESS,
		FAIL,
		RUNNING
	};

	/**
	 * \brief execute the node
	 * \param index of the dwarf
	 * \return 
	 */
	virtual void Execute(unsigned int index) = 0; //TODO trouver un moyen de la remettre en virtual pure

protected:
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

/**
* author Nicolas Schneider
*/
class CompositeNode : public Node
{
public:
	explicit CompositeNode(BehaviorTree* bt, const ptr& parentNode) : Node(bt, parentNode) {}

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
class SequenceComposite final : public CompositeNode
{
public:
	SequenceComposite(BehaviorTree* bt, const ptr& parentNode, json& nodeJson) : CompositeNode(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(SequenceComposite)

/**
* author Nicolas Schneider
*/
class SelectorComposite final : public CompositeNode
{
public:
	SelectorComposite(BehaviorTree* bt, const ptr& parentNode, json& nodeJson) : CompositeNode(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(SelectorComposite)

/**
* author Nicolas Schneider
*/
class DecoratorNode : public Node
{
public:

	explicit DecoratorNode(BehaviorTree* bt, const ptr& parentNode) : Node(bt, parentNode) { }

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
class RepeaterDecorator final : public DecoratorNode
{
public:
	RepeaterDecorator(BehaviorTree* bt, const ptr& parentNode, json& nodeJson) : DecoratorNode(bt, parentNode)
	{
		if(CheckJsonExists(nodeJson, "limit"))
		{
			m_Limit = nodeJson["limit"];
			std::cout << "limit = " << m_Limit << "\n";
		}
	}
	RepeaterDecorator(BehaviorTree* bt, const ptr& parentNode, int limit = 0);

	void Execute(unsigned int index) override;

private:
	int m_Limit;
};
REGISTER_NODE_TYPE(RepeaterDecorator)

/**
* author Nicolas Schneider
*/
class RepeatUntilFailDecorator final : public DecoratorNode
{
public:
	RepeatUntilFailDecorator(BehaviorTree* bt, const ptr& parentNode, json& nodeJson) : DecoratorNode(bt, parentNode) { }

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(RepeatUntilFailDecorator)

/**
* author Nicolas Schneider
*/
class InverterDecorator final : public DecoratorNode
{
public:
	InverterDecorator(BehaviorTree* bt, const ptr& parentNode, json& nodeJson) : DecoratorNode(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(InverterDecorator)

/**
* author Nicolas Schneider
*/
class SucceederDecorator final : public DecoratorNode
{
public:
	SucceederDecorator(BehaviorTree* bt, const ptr& parentNode, json& nodeJson) : DecoratorNode(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(SucceederDecorator)

/**
* author Nicolas Schneider
*/
class Leaf : public Node
{
public:
	explicit Leaf(BehaviorTree* bt, const ptr& parentNode) : Node(bt, parentNode) {}
};

}

#endif
