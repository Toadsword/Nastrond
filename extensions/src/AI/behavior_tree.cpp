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
#include <python/python_engine.h>
#include <utility>

#include <extensions/AI/behavior_tree.h>

namespace sfge::ext::behavior_tree
{
BehaviorTree::BehaviorTree(sfge::Engine& engine) : System(engine) { }

void BehaviorTree::Init()
{
	dwarfManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<DwarfManager>("DwarfManager");
}

void BehaviorTree::Update(float dt)
{
	if (currentNode.size() < entities->size())
	{
		currentNode.resize(entities->size(), m_RootNode);
		doesFlowGoDown.resize(entities->size());
		previousStatus.resize(entities->size());
		repeaterCounter.resize(entities->size(), 0);
		sequenceActiveChild.resize(entities->size(), 0);
	}

	for (size_t i = 0; i < entities->size(); i++)
	{
		if (entities->at(i) == INVALID_ENTITY)
		{
			continue;
		}

		currentNode[i]->Execute(i);
	}
}

void BehaviorTree::FixedUpdate() { }

void BehaviorTree::Draw() { }

void BehaviorTree::LoadNodesFromJson(json& behaviorTreeJson)
{
	if (CheckJsonParameter(behaviorTreeJson, "rootNode", json::value_t::array))
	{
		if (CheckJsonExists(behaviorTreeJson, "type"))
		{
			const NodeType nodeType = behaviorTreeJson["type"];

			switch (nodeType) { 
			case NodeType::NONE: {
				std::ostringstream oss;
				oss << "[Error] No type specified for root node : " << behaviorTreeJson;
				Log::GetInstance()->Error(oss.str());
			}
				break;
			case NodeType::LEAF: 
				SetRootNode(AddLeafNodeFromJson(behaviorTreeJson, nullptr));
				break;
			case NodeType::COMPOSITE: 
				SetRootNode(AddCompositeNodeFromJson(behaviorTreeJson, nullptr));
				break;
			case NodeType::DECORATOR: 
				SetRootNode(AddDecoratorNodeFromJson(behaviorTreeJson, nullptr));
				break;
			default: ; }
		}
	}
}

void BehaviorTree::SetRootNode(const Node::ptr& rootNode)
{
	m_RootNode = rootNode;
}

void BehaviorTree::SetEntities(std::vector<Entity>* vectorEntities)
{
	entities = vectorEntities;
}

std::shared_ptr<Node> BehaviorTree::AddLeafNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode)
{
	return nullptr;
}

std::shared_ptr<Node> BehaviorTree::AddCompositeNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode)
{
	return nullptr;
}

std::shared_ptr<Node> BehaviorTree::AddDecoratorNodeFromJson(json& behaviorTreeJson, const Node::ptr& parentNode)
{
	return nullptr;
}
}
