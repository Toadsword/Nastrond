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
#ifndef SFGE_EXT_BEHAVIOUR_TREE_H
#define SFGE_EXT_BEHAVIOUR_TREE_H

#include <engine/system.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <engine/vector.h>
#include "engine/globals.h"


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
	virtual void Init() = 0;

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

	//Blackboard

	//Bools
	void SetBools(const std::string& key, std::vector<bool>* value);
	bool GetBool(const std::string& key, const unsigned int index);
	bool HasBool(const std::string& key) const;

	//Ints
	void SetInts(const std::string& key, std::vector<int>* value);
	int GetInt(const std::string& key, const unsigned int index);
	bool HasInt(const std::string& key);

	//Floats
	void SetFloats(const std::string& key, std::vector<float>* value);
	float GetFloat(const std::string& key, const unsigned int index);
	bool HasFloat(const std::string& key) const;

	//Strings
	void SetString(const std::string& key, std::vector<std::string>* value);
	std::string GetString(const std::string& key, const unsigned int index);
	bool HasString(const std::string& key) const;

	//Vec2f
	void SetVec2f(const std::string& key,std::vector<Vec2f>* value);
	Vec2f GetVec2f(const std::string& key, const unsigned int index);
	bool HasVec2f(const std::string& key) const;

	std::vector<Entity>* m_Entities;

	std::vector<Node::ptr> m_CurrentNode;
	std::vector<Node::ptr> m_PreviousNode;
	std::vector<Node::Status> m_PreviousStatus;
	std::vector<int> m_Counter;

private:
	//Black board
	std::unordered_map<std::string, std::vector<bool>*> m_Bools;
	std::unordered_map<std::string, std::vector<int>*> m_Ints;
	std::unordered_map<std::string, std::vector<float>*> m_Floats;
	std::unordered_map<std::string, std::vector<std::string>*> m_Strings;
	std::unordered_map<std::string, std::vector<Vec2f>*> m_Vec2fs;


	Node::ptr m_RootNode = nullptr;
};
}

#endif
