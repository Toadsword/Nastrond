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
Node::Status HasDwelling::Execute(unsigned int index) { //TODO Demander à Elias si ça devrait être en const ou non
	behaviourTree->m_FlowGoesDown[index] = behaviourTree->FLOW_GOES_UP;
	behaviourTree->m_CurrentNode[index] = m_parentNode;

	if (behaviourTree->dwarfManager->GetDwellingEntity(index) == INVALID_ENTITY)
	{
		return Status::FAIL;
	}
	else
	{
		return Status::SUCCESS;
	}
}

Node::Status AssignDwelling::Execute(unsigned int index) {
	auto const dwellingEntity = behaviourTree->dwellingManager->GetFreeSlotInBuilding();

	behaviourTree->m_FlowGoesDown[index] = behaviourTree->FLOW_GOES_UP;
	behaviourTree->m_CurrentNode[index] = m_parentNode;

	if (dwellingEntity == INVALID_ENTITY) {
		return Status::FAIL;
	}
	else {
		behaviourTree->dwarfManager->AssignDwellingToDwarf(index, dwellingEntity);
		behaviourTree->dwellingManager->AddDwarfToBuilding(dwellingEntity);

		return Status::SUCCESS;
	}
}

Node::Status FindRandomPath::Execute(unsigned int index) {
	behaviourTree->dwarfManager->BtFindRandomPath(index);

	behaviourTree->m_FlowGoesDown[index] = behaviourTree->FLOW_GOES_UP;
	behaviourTree->m_CurrentNode[index] = m_parentNode;
	return Status::SUCCESS;
}

Node::Status FindPathToDwelling::Execute(unsigned int index) {
	behaviourTree->dwarfManager->BTAddPathToDwelling(index);

	behaviourTree->m_FlowGoesDown[index] = behaviourTree->FLOW_GOES_UP;
	behaviourTree->m_CurrentNode[index] = m_parentNode;
	return Status::SUCCESS;
}

Node::Status MoveTo::Execute(unsigned int index) {
	if (behaviourTree->dwarfManager->IsDwarfAtDestination(index)) {
		behaviourTree->m_FlowGoesDown[index] = behaviourTree->FLOW_GOES_UP;
		behaviourTree->m_CurrentNode[index] = m_parentNode;
		return Status::SUCCESS;
	}

	behaviourTree->dwarfManager->BTAddPathFollower(index);
	return Status::RUNNING;
}

Node::Status WaitForPath::Execute(unsigned int index) {
	if (behaviourTree->dwarfManager->HasPath(index)) {
		behaviourTree->m_FlowGoesDown[index] = behaviourTree->FLOW_GOES_UP;
		behaviourTree->m_CurrentNode[index] = m_parentNode;
		return Status::SUCCESS;
	}

	return Status::RUNNING;
}
}
