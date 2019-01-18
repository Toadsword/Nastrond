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

#include <extensions/behaviour_tree.h>

namespace sfge::ext::behaviour_tree
{
	void BehaviourTree::SetInts(std::string key, std::vector<int>* value) {
		m_Ints[key] = value;
	}

	int BehaviourTree::GetInt(std::string key, Entity entity) {
		
	}

	bool BehaviourTree::HasInt(std::string key) {
		return m_Ints.find(key) != m_Ints.end();
	}



	void BehaviourTree::SetString(std::string key, std::vector<std::string>* value) {
		m_Strings[key] = value;
	}

	std::string BehaviourTree::GetString(std::string key, Entity entity) {
		if (m_Strings.find(key) == m_Strings.end()) {
			m_Strings[key] = nullptr;
		}
		return m_Strings[key]->at(GetInt("entityIndex", entity));
	}

	bool BehaviourTree::HasString(std::string key) const {
		return m_Strings.find(key) != m_Strings.end();
	}

}