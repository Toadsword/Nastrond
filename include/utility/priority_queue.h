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

#ifndef SFGE_PRIORITY_QUEUE_H_
#define SFGE_PRIORITY_QUEUE_H
#include <queue>

namespace sfge
{
	/**
	 * \brief Priority queue order by a priority
	 * \tparam T item to store
	 * \tparam priority_t priority of the item
	 * \author Nicolas Schneider
	 */
	template<typename  T, typename priority_t> struct PriorityQueue {
	private:
		typedef std::pair<priority_t, T> PQElement;
		std::priority_queue<PQElement, std::vector<PQElement>, std::greater<>> m_elements;

	public:
		/**
		 * \brief check if the queue is empty
		 * \return 
		 */
		bool Empty() const {
			return m_elements.empty();
		}

		/**
		 * \brief Put an item in the queue
		 * \param item 
		 * \param priority 
		 */
		void Insert(T item, priority_t priority) {
			m_elements.emplace(priority, item);
		}

		/**
		 * \brief Pop and return the first element by priority
		 * \return 
		 */
		T Get() {
			T bestItem = m_elements.top().second;
			m_elements.pop();
			return bestItem;
		}
	};
}

/**
	 * \brief Priority queue order by a priority
	 * \tparam E item to store
	 * \author Nicolas Schneider
	 */
template <typename E>
class VectorCompleteTree {
public:
	typedef typename std::vector<E>::iterator Position;
private:
	std::vector<E> m_Vector;
protected:
	Position GetPosition(int i) { return m_Vector.begin() + i; }
	int GetIndex(const Position& p) const { return p - m_Vector.begin(); }
public:
	VectorCompleteTree() : m_Vector(1) { m_Vector.reserve(128); }
	size_t Size() const
	{
		return  m_Vector.size() - 1;
	}
	Position Left(const Position& p)
	{
		return GetPosition(2 * GetIndex(p));
	}
	Position Right(const Position& p) { return GetPosition(2 * GetIndex(p) + 1); }
	Position Parent(const Position& p) { return GetPosition(GetIndex(p) / 2); }
	bool HasLeft(const Position& p) const { return 2 * GetIndex(p) <= Size(); }
	bool HasRight(const Position& p) const { return 2 * GetIndex(p) + 1 <= Size(); }
	bool IsRoot(const Position &p) const { return GetIndex(p) == 1; }
	Position Root() { return GetPosition(1); }
	Position Last() { return GetPosition(Size()); }
	void AddLast(const E& e) { m_Vector.push_back(e); }
	void RemoveLast() { m_Vector.pop_back(); }
	void Swap(const Position& p, const Position& q) { E e = *q; *q = *p; *p = e; }
};


/**
 * \brief Priority queue
 * \author Nicolas Schneider
 */
class HeapPriorityQueue
{
public:
	bool Empty() const { return m_Priority.Size() == 0; }
	void Insert(const unsigned int& index, const float& priority)
	{
		m_Priority.AddLast(priority);
		m_Index.AddLast(index);
		auto v = m_Priority.Last();
		auto v2 = m_Index.Last();
		while (!m_Priority.IsRoot(v))
		{
			auto u = m_Priority.Parent(v);
			auto u2 = m_Index.Parent(v2);
			if (!(*v < *u)) break;
			m_Priority.Swap(v, u);
			v = u;
			m_Index.Swap(v2, u2);
			v2 = u2;
		}

		if (m_Priority.Size() > 128)
		{
			m_Priority.RemoveLast();
			m_Index.RemoveLast();
		}
	}
	const unsigned int& Min() { return *(m_Index.Root()); }
	void RemoveMin()
	{
		if (m_Priority.Size() == 1)
		{
			m_Priority.RemoveLast();
		}
		else
		{
			auto u = m_Priority.Root();
			auto u2 = m_Index.Root();
			m_Priority.Swap(u, m_Priority.Last());
			m_Priority.RemoveLast();
			m_Index.Swap(u2, m_Index.Last());
			m_Index.RemoveLast();
			while (m_Priority.HasLeft(u))
			{
				auto v = m_Priority.Left(u);
				auto v2 = m_Index.Left(u2);
				if (m_Priority.HasRight(u) && *(m_Priority.Right(u)) < *v)
				{
					v = m_Priority.Right(u);
					v2 = m_Index.Right(u2);
				}
				if (*v < *u)
				{
					m_Priority.Swap(v, u);
					u = v;

					m_Index.Swap(v2, u2);
					u2 = v2;
				}
				else break;
			}
		}
	}
private:
	VectorCompleteTree<float> m_Priority;
	VectorCompleteTree<unsigned int> m_Index;

	typedef VectorCompleteTree<unsigned int>::Position PositionIndex;
	typedef VectorCompleteTree<float>::Position PositionPriority;
};

#endif /* INCLUDE_UTILITY_PRIORITY_QUEUE_H_ */
