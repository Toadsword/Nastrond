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

	class HasDwelling : public Leaf
	{
	public:

		Status Execute(unsigned index) override
		{
			if (behaviourTree->dwarfManager->GetDwellingEntity(index) == INVALID_ENTITY)
			{
				return Status::FAIL;
			}
			else
			{
				return Status::SUCCESS;
			}
		}
	};

	class AssignDwelling : public Leaf
	{
	public:

		Status Execute(unsigned int index) override
		{
			auto const dwellingEntity = behaviourTree->dwellingManager->GetFreeSlotInBuilding();

			if (dwellingEntity == INVALID_ENTITY) {
				return Status::FAIL;
			}
			else {
				behaviourTree->dwarfManager->AssignDwellingToDwarf(index, dwellingEntity);
				behaviourTree->dwellingManager->AddDwarfToBuilding(dwellingEntity);
				return Status::SUCCESS;
			}
		}
	};

	class FindRandomPath : public Leaf {
	public:
		FindRandomPath(BehaviourTree* BT) { behaviourTree = BT; }

		Status Execute(unsigned index) override
		{
			//std::cout << "Execute: find random path [" << index << "] \n";
			behaviourTree->dwarfManager->BtFindRandomPath(index);
			return Status::SUCCESS;
		}
	};

	class FindPathToDwelling : public Leaf
	{
	public:

		Status Execute(unsigned index) override
		{
			behaviourTree->dwarfManager->BTAddPathToDwelling(index);
			return Status::SUCCESS;
		}
	};

	class MoveTo : public Leaf
	{
	public:
		MoveTo(BehaviourTree* BT) { behaviourTree = BT; }

		Status Execute(unsigned index) override
		{
			//std::cout << "Execute: move to [" << index << "] \n";
			if (behaviourTree->dwarfManager->IsDwarfAtDestination(index)) {
				return Status::SUCCESS;
			}
			else {
				behaviourTree->dwarfManager->BTAddPathFollower(index);
				return Status::RUNNING;
			}
		}
	};

	class WaitForPath : public Leaf
	{
	public:
		WaitForPath(BehaviourTree* BT) { behaviourTree = BT; }

		Status Execute(unsigned index) override
		{
			if (behaviourTree->dwarfManager->HasPath(index)) {
				return Status::SUCCESS;
			}
			else {
				return Status::RUNNING;
			}
		}
	};
}
#endif