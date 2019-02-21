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
FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef SFGE_EXT_BEHAVIOR_TREE_FACTORY_H
#define SFGE_EXT_BEHAVIOR_TREE_FACTORY_H

#include <memory>
#include <map>
#include <string>

#include <utility/json_utility.h>
#include <extensions/AI/behavior_tree_nodes_core.h>
#include <fstream>
#include <iostream>

namespace sfge::ext::behavior_tree
{

/**
* author Nicolas Schneider
*/
class BehaviorTreeUtility final
{
public:
	/**
	 * \brief Load complete behavior tree from json fil
	 * \param behaviorTreeJson 
	 */
	static Node::ptr LoadNodesFromJson(json& behaviorTreeJson, BehaviorTree* behaviorTree);

	/**
	 * \brief Save behavior tree into a file
	 * \param node: root node
	 * \param filePath: where the behavior tree will be saved
	 */
	static void SaveBehaviorTreeToJson(const Node::ptr& node, const std::string& filePath);

	static std::string NodeTypeToString(const Node::NodeType nodeType);

private:
	static Node::ptr AddLeafNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree);

	static Node::ptr AddCompositeNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree);

	static Node::ptr AddDecoratorNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode, BehaviorTree* behaviorTree);

	static std::string LeafNodeToString(const Node::ptr& node);

	static std::string CompositeNodeToString(const Node::ptr& node);

	static std::string DecoratorNodeToString(const Node::ptr& node);
};
}

#endif