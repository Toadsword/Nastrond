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

#ifndef SFGE_EXT_BEHAVIOR_TREE_NODE_EXTENSION_H
#define SFGE_EXT_BEHAVIOR_TREE_NODE_EXTENSION_H

#include <extensions/AI/behavior_tree_nodes_core.h>

namespace sfge::ext::behavior_tree
{

/**
 * author Nicolas Schneider
 */
class HasDwellingLeaf final : public Leaf
{
public:
	HasDwellingLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(HasDwellingLeaf)

/**
* author Nicolas Schneider
*/
class SetDwellingLeaf final : public Leaf
{
public:
	SetDwellingLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(SetDwellingLeaf)

/**
* author Nicolas Schneider
*/
class FindRandomPathLeaf final : public Leaf
{
public:
	FindRandomPathLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(FindRandomPathLeaf)

/**
* author Nicolas Schneider
*/
class FindPathToDwellingLeaf final : public Leaf
{
public:
	FindPathToDwellingLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(FindPathToDwellingLeaf)

/**
* author Nicolas Schneider
*/
class MoveToLeaf final : public Leaf
{
public:
	MoveToLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(MoveToLeaf)

/**
* author Nicolas Schneider
*/
class WaitForPathLeaf final : public Leaf
{
public:
	WaitForPathLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(WaitForPathLeaf)

/**
* author Nicolas Schneider
*/
class FindPathToWorkingPlaceLeaf final : public Leaf
{
public:
	FindPathToWorkingPlaceLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(FindPathToWorkingPlaceLeaf)

/**
* author Nicolas Schneider
*/
class FindPathToGiverLeaf final : public Leaf
{
public:
	FindPathToGiverLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(FindPathToGiverLeaf)

/**
* author Nicolas Schneider
*/
class FindPathToReceiverLeaf final : public Leaf
{
public:
	FindPathToReceiverLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(FindPathToReceiverLeaf)

/**
* author Nicolas Schneider
*/
class EnterDwellingLeaf final : public Leaf
{
public:
	EnterDwellingLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(EnterDwellingLeaf)

/**
* author Nicolas Schneider
*/
class ExitDwellingLeaf final : public Leaf
{
public:
	ExitDwellingLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(ExitDwellingLeaf)

/**
* author Nicolas Schneider
*/
class EnterWorkingPlaceLeaf final : public Leaf
{
public:
	EnterWorkingPlaceLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(EnterWorkingPlaceLeaf)

/**
* author Nicolas Schneider
*/
class ExitWorkingPlaceLeaf final : public Leaf
{
public:
	ExitWorkingPlaceLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(ExitWorkingPlaceLeaf)

/**
* author Nicolas Schneider
*/
class HasJobLeaf final : public Leaf
{
public:
	HasJobLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(HasJobLeaf)

/**
* author Nicolas Schneider
*/
class HasStaticJobLeaf final : public Leaf
{
public:
	HasStaticJobLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(HasStaticJobLeaf)

/**
* author Nicolas Schneider
*/
class AssignJobLeaf final : public Leaf
{
public:
	AssignJobLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(AssignJobLeaf)

/**
* author Nicolas Schneider
*/
class IsDayTimeLeaf final : public Leaf
{
public:
	IsDayTimeLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(IsDayTimeLeaf)

/**
* author Nicolas Schneider
*/
class IsNightTimeLeaf final : public Leaf
{
public:
	IsNightTimeLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(IsNightTimeLeaf)

/**
* author Nicolas Schneider
*/
class WaitDayTimeLeaf final : public Leaf
{
public:
	WaitDayTimeLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(WaitDayTimeLeaf)

/**
* author Nicolas Schneider
*/
class WaitNightTimeLeaf final : public Leaf
{
public:
	WaitNightTimeLeaf(BehaviorTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	void Execute(unsigned int index) override;
};
REGISTER_NODE_TYPE(WaitNightTimeLeaf)

}
#endif
