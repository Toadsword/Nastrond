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
class BehaviourTree : public System {
public:
	BehaviourTree(sfge::Engine& engine);

	//Bools
	void SetBools(const std::string& key, std::vector<bool>* value);
	bool GetBool(const std::string& key, const Entity entity);
	bool HasBool(const std::string& key) const;

	//Ints
	void SetInts(const std::string& key, std::vector<int>* value);
	int GetInt(const std::string& key, const Entity entity);
	bool HasInt(const std::string& key);

	//Floats
	void SetFloats(const std::string& key, std::vector<float>* value);
	float GetFloat(const std::string& key, const Entity entity);
	bool HasFloat(const std::string& key) const;

	//Strings
	void SetString(const std::string& key, std::vector<std::string>* value);
	std::string GetString(const std::string& key, const Entity entity);
	bool HasString(const std::string& key) const;

	//Vec2f
	void SetVec2f(const std::string& key,std::vector<Vec2f>* value);
	Vec2f GetVec2f(const std::string& key, const Entity entity);
	bool HasVec2f(const std::string& key) const;

private:
	std::unordered_map<std::string, std::vector<bool>*> m_Bools;
	std::unordered_map<std::string, std::vector<int>*> m_Ints;
	std::unordered_map<std::string, std::vector<float>*> m_Floats;
	std::unordered_map<std::string, std::vector<std::string>*> m_Strings;
	std::unordered_map<std::string, std::vector<Vec2f>*> m_Vec2fs;
};

class Node {
public:
	virtual bool Execute() = 0;

	enum class Status {
		SUCCESS,
		FAIL,
		RUNNING
	};

	using ptr = std::shared_ptr<Node>;
};

class CompositeNode : public Node {
public: 
	void AddChild(Node::ptr child) { children.push_back(child); }
	bool HasChildren() const { return !children.empty(); }
protected:
	std::vector<Node::ptr> children;
};

class Decorator : public Node {
public:

	void SetChild(Node::ptr node) { child = node; }
	bool HasChild() const { return child != nullptr; }

protected:
	Node::ptr child = nullptr;
};

class Leaf : public Node {
	
};
}

#endif
