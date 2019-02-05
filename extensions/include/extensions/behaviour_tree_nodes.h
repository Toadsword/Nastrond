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
class HasDwelling final : public Leaf
{
public:
	HasDwelling(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode){}

	Status Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class AssignDwelling final : public Leaf
{
public:
	AssignDwelling(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class FindRandomPath final : public Leaf {
public:
	FindRandomPath(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode){}

	Status Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class FindPathToDwelling final : public Leaf
{
public:
	FindPathToDwelling(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class MoveTo final : public Leaf
{
public:
	MoveTo(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};

/**
* author Nicolas Schneider
*/
class WaitForPath final : public Leaf
{
public:
	WaitForPath(BehaviourTree* bt, const ptr& parentNode) : Leaf(bt, parentNode) {}

	Status Execute(unsigned int index) override;
};
}
#endif