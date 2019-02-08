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

#include <extensions/behaviour_tree_nodes.h>

namespace sfge::ext::behaviour_tree
{
void HasDwellingLeaf::Execute(const unsigned int index)
{
	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	if (m_BehaviourTree->dwarfManager->GetDwellingEntity(index) == INVALID_ENTITY)
	{
		m_BehaviourTree->previousStatus[index] = Status::FAIL;
	}
	else
	{
		m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
	}
}

void SetDwellingLeaf::Execute(const unsigned int index)
{
	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	if (m_BehaviourTree->dwarfManager->AssignDwellingToDwarf(index))
	{
		m_BehaviourTree->previousStatus[index] = Status::FAIL;
	}
	else
	{

		m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
	}
}

void FindRandomPathLeaf::Execute(const unsigned int index)
{
	m_BehaviourTree->dwarfManager->AddFindRandomPathBT(index);

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
}

void FindPathToDwellingLeaf::Execute(const unsigned int index)
{
	m_BehaviourTree->dwarfManager->AddFindPathToDestinationBT(index, m_BehaviourTree->dwarfManager->GetDwellingAssociatedPosition(index));

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
}

void MoveToLeaf::Execute(const unsigned int index)
{
	if (m_BehaviourTree->dwarfManager->IsDwarfAtDestination(index))
	{
		m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
		m_BehaviourTree->currentNode[index] = m_ParentNode;

		m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
		return;
	}

	m_BehaviourTree->dwarfManager->AddPathFollowingBT(index);

	m_BehaviourTree->previousStatus[index] = Status::RUNNING;
}

void WaitForPathLeaf::Execute(const unsigned int index)
{
	if (m_BehaviourTree->dwarfManager->HasPath(index))
	{
		m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
		m_BehaviourTree->currentNode[index] = m_ParentNode;

		m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
		return;
	}

	m_BehaviourTree->previousStatus[index] = Status::RUNNING;
}

void FindPathToWorkingPlaceLeaf::Execute(const unsigned int index)
{
	m_BehaviourTree->dwarfManager->AddFindPathToDestinationBT(index, m_BehaviourTree->dwarfManager->GetWorkingPlaceAssociatedPosition(index));

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
}

void FindPathToGiverLeaf::Execute(const unsigned int index)
{
	m_BehaviourTree->dwarfManager->AddInventoryTaskPathToGiver(index);

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
}

void FindPathToReceiverLeaf::Execute(const unsigned int index)
{
	m_BehaviourTree->dwarfManager->AddInventoryTaskPathToReceiver(index);

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
}

void EnterDwellingLeaf::Execute(const unsigned int index)
{
	m_BehaviourTree->dwarfManager->DwarfEnterDwelling(index);

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
}

void ExitDwellingLeaf::Execute(const unsigned int index)
{
	m_BehaviourTree->dwarfManager->DwarfExitDwelling(index);

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
}

void EnterWorkingPlaceLeaf::Execute(unsigned int index)
{
	m_BehaviourTree->dwarfManager->DwarfEnterWorkingPlace(index);

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
}

void ExitWorkingPlaceLeaf::Execute(unsigned int index)
{
	m_BehaviourTree->dwarfManager->DwarfExitWorkingPlace(index);

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	m_BehaviourTree->previousStatus[index] = Status::SUCCESS;
}
}
