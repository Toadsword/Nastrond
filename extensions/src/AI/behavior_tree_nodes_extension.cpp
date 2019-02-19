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

#include <extensions/AI/behavior_tree_nodes_extension.h>
#include <extensions/AI/behavior_tree.h>

namespace sfge::ext::behavior_tree
{
void HasDwellingLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	const auto hasDwelling = m_BehaviorTree->dwarfManager->GetDwellingEntity(index);
	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	if (hasDwelling == INVALID_ENTITY)
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
#endif

#ifdef BT_SOA
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->GetDwellingEntity(index) == INVALID_ENTITY)
	{
		m_BehaviorTree->previousStatus[index] = Status::FAIL;
	}
	else
	{
		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}
#endif
}

void SetDwellingLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	const auto hasBeenAssigned = m_BehaviorTree->dwarfManager->AssignDwellingToDwarf(index);
	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	if (hasBeenAssigned)
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
#endif

#ifdef BT_SOA
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->AssignDwellingToDwarf(index))
	{
		m_BehaviorTree->previousStatus[index] = Status::FAIL;
	}
	else
	{

		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}
#endif
}

void FindRandomPathLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->AddFindRandomPathBT(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->AddFindRandomPathBT(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void FindPathToDwellingLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->AddFindPathToDestinationBT(index, m_BehaviorTree->dwarfManager->GetDwellingAssociatedPosition(index));

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->AddFindPathToDestinationBT(index, m_BehaviorTree->dwarfManager->GetDwellingAssociatedPosition(index));

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void MoveToLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	const auto isAtDestination = m_BehaviorTree->dwarfManager->IsDwarfAtDestination(index);

	if (isAtDestination)
	{
		m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
		return;
	}

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;

	m_BehaviorTree->dwarfManager->AddPathFollowingBT(index);
#endif

#ifdef BT_SOA
	if (m_BehaviorTree->dwarfManager->IsDwarfAtDestination(index))
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;

		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
		return;
	}

	m_BehaviorTree->dwarfManager->AddPathFollowingBT(index);

	m_BehaviorTree->previousStatus[index] = Status::RUNNING;
#endif
}

void WaitForPathLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	const auto hasPath = m_BehaviorTree->dwarfManager->HasPath(index);

	if (hasPath)
	{
		m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
		return;
	}

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
#endif

#ifdef BT_SOA
	if (m_BehaviorTree->dwarfManager->HasPath(index))
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;

		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
		return;
	}

	m_BehaviorTree->previousStatus[index] = Status::RUNNING;
#endif
}

void FindPathToWorkingPlaceLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->AddFindPathToDestinationBT(index, m_BehaviorTree->dwarfManager->GetWorkingPlaceAssociatedPosition(index));

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->AddFindPathToDestinationBT(index, m_BehaviorTree->dwarfManager->GetWorkingPlaceAssociatedPosition(index));

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void FindPathToGiverLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->AddInventoryTaskPathToGiver(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->AddInventoryTaskPathToGiver(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void FindPathToReceiverLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->AddInventoryTaskPathToReceiver(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->AddInventoryTaskPathToReceiver(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void EnterDwellingLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->DwarfEnterDwelling(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->DwarfEnterDwelling(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void ExitDwellingLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->DwarfExitDwelling(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->DwarfExitDwelling(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void EnterWorkingPlaceLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->DwarfEnterWorkingPlace(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->DwarfEnterWorkingPlace(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void ExitWorkingPlaceLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->DwarfExitWorkingPlace(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->DwarfExitWorkingPlace(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void HasJobLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	const auto hasJob = m_BehaviorTree->dwarfManager->HasJob(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	if (hasJob)
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;

	}
#endif

#ifdef BT_SOA
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->HasJob(index))
	{
		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->previousStatus[index] = Status::FAIL;

	}
#endif
}

void HasStaticJobLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	const auto hasStaticJob = m_BehaviorTree->dwarfManager->HasStaticJob(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	if (hasStaticJob)
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;

	}
#endif

#ifdef BT_SOA
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if(m_BehaviorTree->dwarfManager->HasJob(index))
	{
		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}else
	{
		m_BehaviorTree->previousStatus[index] = Status::FAIL;
		
	}
#endif
}

void AssignJobLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	const auto jobAssigned = m_BehaviorTree->dwarfManager->AssignJob(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	if (jobAssigned)
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;
	}
#endif

#ifdef BT_SOA
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->AssignJob(index))
	{
		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->previousStatus[index] = Status::FAIL;
	}
#endif
}

void IsDayTimeLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->IsDayTime())
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;
	}
#endif

#ifdef BT_SOA
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if(m_BehaviorTree->dwarfManager->IsDayTime())
	{
		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->previousStatus[index] = Status::FAIL;
	}
#endif
}

void IsNightTimeLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	const auto isNightTime = m_BehaviorTree->dwarfManager->IsNightTime();

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	if (isNightTime)
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::FAIL;
	}
#endif

#ifdef BT_SOA
	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	if (m_BehaviorTree->dwarfManager->IsNightTime())
	{
		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->previousStatus[index] = Status::FAIL;
	}
#endif
}

void WaitDayTimeLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	if (m_BehaviorTree->dwarfManager->IsDayTime())
	{
		m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
	}
#endif

#ifdef BT_SOA
	if (m_BehaviorTree->dwarfManager->IsDayTime())
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;
		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->previousStatus[index] = Status::RUNNING;
	}
#endif
}

void WaitNightTimeLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	if (m_BehaviorTree->dwarfManager->IsNightTime())
	{
		m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
	}
#endif

#ifdef BT_SOA
	if (m_BehaviorTree->dwarfManager->IsNightTime())
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;
		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->previousStatus[index] = Status::RUNNING;
	}
#endif
}

void AskInventoryTaskLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	if (m_BehaviorTree->dwarfManager->AddInventoryTaskBT(index))
	{
		m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::RUNNING;
	}
#endif

#ifdef BT_SOA
	if (m_BehaviorTree->dwarfManager->AddInventoryTaskBT(index))
	{
		m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
		m_BehaviorTree->currentNode[index] = m_ParentNode;
		m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
	}
	else
	{
		m_BehaviorTree->previousStatus[index] = Status::RUNNING;
	}
#endif
}

void TakeResourcesLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->TakeResources(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->DwarfTakeResources(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}

void PutResourcesLeaf::Execute(const unsigned int index)
{
#ifdef BT_AOS
	m_BehaviorTree->dwarfManager->PutResources(index);

	m_BehaviorTree->dataBehaviorTree[index].doesFlowGoDown = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->dataBehaviorTree[index].currentNode = m_ParentNode;

	m_BehaviorTree->dataBehaviorTree[index].previousStatus = Status::SUCCESS;
#endif

#ifdef BT_SOA
	m_BehaviorTree->dwarfManager->PutResources(index);

	m_BehaviorTree->doesFlowGoDown[index] = m_BehaviorTree->flowGoUp;
	m_BehaviorTree->currentNode[index] = m_ParentNode;

	m_BehaviorTree->previousStatus[index] = Status::SUCCESS;
#endif
}
}
