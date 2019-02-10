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

#ifndef SFGE_EXT_BEHAVIOUR_TREE_NODE_H
#define SFGE_EXT_BEHAVIOUR_TREE_NODE_H

#include <extensions/behaviour_tree_core.h>

namespace sfge::ext::behaviour_tree
{
/**
 * author Nicolas Schneider
 */
class HasDwellingLeaf final : public Leaf
{
public:
	HasDwellingLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class SetDwellingLeaf final : public Leaf
{
public:
	SetDwellingLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class FindRandomPathLeaf final : public Leaf
{
public:
	FindRandomPathLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class FindPathToDwellingLeaf final : public Leaf
{
public:
	FindPathToDwellingLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class MoveToLeaf final : public Leaf
{
public:
	MoveToLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class WaitForPathLeaf final : public Leaf
{
public:
	WaitForPathLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class FindPathToWorkingPlaceLeaf final : public Leaf
{
public:
	FindPathToWorkingPlaceLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class FindPathToGiverLeaf final : public Leaf
{
public:
	FindPathToGiverLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class FindPathToReceiverLeaf final : public Leaf
{
public:
	FindPathToReceiverLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class EnterDwellingLeaf final : public Leaf
{
public:
	EnterDwellingLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class ExitDwellingLeaf final : public Leaf
{
public:
	ExitDwellingLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class EnterWorkingPlaceLeaf final : public Leaf
{
public:
	EnterWorkingPlaceLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class ExitWorkingPlaceLeaf final : public Leaf
{
public:
	ExitWorkingPlaceLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class HasJobLeaf final : public Leaf
{
public:
	HasJobLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class HasStaticJobLeaf final : public Leaf
{
public:
	HasStaticJobLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class AssignJobLeaf final : public Leaf
{
public:
	AssignJobLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class IsDayTimeLeaf final : public Leaf
{
public:
	IsDayTimeLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class IsNightTimeLeaf final : public Leaf
{
public:
	IsNightTimeLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class WaitDayTimeLeaf final : public Leaf
{
public:
	WaitDayTimeLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class WaitNightTimeLeaf final : public Leaf
{
public:
	WaitNightTimeLeaf(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
}
#endif
