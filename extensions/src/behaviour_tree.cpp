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
#include <iostream>

#include <extensions/behaviour_tree.h>

namespace sfge::ext::behaviour_tree
{
	BehaviourTree::BehaviourTree(sfge::Engine& engine) : System(engine) 
	{
	}

	void BehaviourTree::Init()
	{

	}

	void BehaviourTree::Update(float dt)
	{
		if(m_CurrentNode.size() < m_Entities->size())
		{
			m_CurrentNode.resize(m_Entities->size(), m_RootNode);
		}

		for(int i = 0; i < m_Entities->size(); i++)
		{
			Node::Status status = m_CurrentNode[i]->Execute(i);

			if(status == Node::Status::SUCCESS)
			{
				m_CurrentNode[i] = m_CurrentNode[i]->parentNode;
			}
		}
	}

	void BehaviourTree::FixedUpdate()
	{
	}

	void BehaviourTree::Draw()
	{
	}

	void BehaviourTree::SetRootNode(const Node::ptr& rootNode)
	{
		m_RootNode = rootNode;
		m_RootNode->parentNode = nullptr;
	}

	void BehaviourTree::SetEntities(std::vector<Entity>* entities)
	{
		m_Entities = entities;
	}

	void BehaviourTree::SetBools(const std::string& key, std::vector<bool>* value)
	{
		m_Bools[key] = value;
	}

	bool BehaviourTree::GetBool(const std::string& key, const Entity entity)
	{
		if (m_Bools.find(key) == m_Bools.end()) {
			m_Bools[key] = nullptr;
		}
		return  m_Bools[key]->at(GetInt("entityIndex", entity));
	}

	bool BehaviourTree::HasBool(const std::string& key) const
	{
		return m_Bools.find(key) != m_Bools.end();
	}

	void BehaviourTree::SetInts(const std::string& key, std::vector<int>* value) {
		m_Ints[key] = value;
	}

	int BehaviourTree::GetInt(const std::string& key, const unsigned int index) {
		if (m_Ints.find(key) == m_Ints.end()) {
			m_Ints[key] = nullptr;
		}
		return  m_Ints[key]->at(index);
	}

	bool BehaviourTree::HasInt(const std::string& key) {
		return m_Ints.find(key) != m_Ints.end();
	}

	void BehaviourTree::SetFloats(const std::string& key, std::vector<float>* value)
	{
		m_Floats[key] = value;
	}

	float BehaviourTree::GetFloat(const std::string& key, const unsigned int index)
	{
		if (m_Floats.find(key) == m_Floats.end()) {
			m_Floats[key] = nullptr;
		}
		return m_Floats[key]->at(index);
	}

	bool BehaviourTree::HasFloat(const std::string& key) const
	{
		return m_Floats.find(key) != m_Floats.end();
	}

	void BehaviourTree::SetString(const std::string& key, std::vector<std::string>* value) {
		m_Strings[key] = value;
	}

	std::string BehaviourTree::GetString(const std::string& key, const unsigned int index) {
		if (m_Strings.find(key) == m_Strings.end()) {
			m_Strings[key] = nullptr;
		}
		return m_Strings[key]->at(index);
	}

	bool BehaviourTree::HasString(const std::string& key) const {
		return m_Strings.find(key) != m_Strings.end();
	}

	void BehaviourTree::SetVec2f(const std::string& key, std::vector<Vec2f>* value)
	{
		m_Vec2fs[key] = value;
	}

	Vec2f BehaviourTree::GetVec2f(const std::string& key, const unsigned int index)
	{
		if (m_Vec2fs.find(key) == m_Vec2fs.end()) {
			m_Vec2fs[key] = nullptr;
		}
		return m_Vec2fs[key]->at(index);
	}

	bool BehaviourTree::HasVec2f(const std::string& key) const
	{
		return m_Vec2fs.find(key) != m_Vec2fs.end();
	}

	Repeater::Repeater(BehaviourTree* BT, int limit)
	{
		m_Limit = limit;
		behaviourTree = BT;
	}

	void Repeater::Init()
	{
		m_Counter = 0;
	}

	Node::Status Repeater::Execute(unsigned int index)
	{
		std::cout << "Execute: Repeater\n";
		//If limit == 0 => inifinity, if m_Counter == m_Limit it's over
		if(m_Limit > 0 && ++m_Counter == m_Limit)
		{
			return Status::SUCCESS;
		}

		//Switch current node to child
		behaviourTree->m_CurrentNode[index] = m_Child;

		return Status::RUNNING;
	}

	void DebugUpdateLeaf::Init()
	{
	}

	Node::Status DebugUpdateLeaf::Execute(unsigned index)
	{
		std::cout << "Execute: DebugUpdateLeaf\n";
		return Status::SUCCESS;
	}
}
