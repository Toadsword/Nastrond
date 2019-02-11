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

#include <vector>
#include <memory>

#include <engine/system.h>
#include <utility/json_utility.h>
#include <engine/globals.h>

#include <extensions/dwarf_manager.h>
#include <extensions/AI/behavior_tree_nodes_extension.h>

namespace sfge::ext::behavior_tree
{

/**
* author Nicolas Schneider
*/
class BehaviorTree final : public System
{
public:
	explicit BehaviorTree(Engine& engine);

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

	void LoadNodesFromJson(json& behaviorTreeJson);

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

	//TODO est-ce que ça ferait plus de sens d'avoir un tableau de struct
	std::vector<Node::ptr> currentNode;
	std::vector<bool> doesFlowGoDown;
	std::vector<Node::Status> previousStatus;

	std::vector<char> repeaterCounter;
	std::vector<char> sequenceActiveChild;

	DwarfManager* dwarfManager;

	const bool flowGoDown = true;
	const bool flowGoUp = false;

private:
	std::shared_ptr<Node> AddLeafNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode);

	std::shared_ptr<Node> AddCompositeNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode);

	std::shared_ptr<Node> AddDecoratorNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode);

	Node::ptr m_RootNode = nullptr;
};
}

#endif
