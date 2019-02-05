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
Node::Status HasDwelling::Execute(const unsigned int index) {
	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	if (m_BehaviourTree->dwarfManager->GetDwellingEntity(index) == INVALID_ENTITY)
	{
		return Status::FAIL;
	}
	else
	{
		return Status::SUCCESS;
	}
}

Node::Status AssignDwelling::Execute(const unsigned int index) {
	auto const dwellingEntity = m_BehaviourTree->dwellingManager->GetFreeSlotInBuilding();

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;

	if (dwellingEntity == INVALID_ENTITY) {
		return Status::FAIL;
	}
	else {
		m_BehaviourTree->dwarfManager->AssignDwellingToDwarf(index, dwellingEntity);
		m_BehaviourTree->dwellingManager->AddDwarfToBuilding(dwellingEntity);

		return Status::SUCCESS;
	}
}

Node::Status FindRandomPath::Execute(const unsigned int index) {
	m_BehaviourTree->dwarfManager->BtFindRandomPath(index);

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;
	return Status::SUCCESS;
}

Node::Status FindPathToDwelling::Execute(const unsigned int index) {
	m_BehaviourTree->dwarfManager->BTAddPathToDwelling(index);

	m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
	m_BehaviourTree->currentNode[index] = m_ParentNode;
	return Status::SUCCESS;
}

Node::Status MoveTo::Execute(const unsigned int index) {
	if (m_BehaviourTree->dwarfManager->IsDwarfAtDestination(index)) {
		m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
		m_BehaviourTree->currentNode[index] = m_ParentNode;
		return Status::SUCCESS;
	}

	m_BehaviourTree->dwarfManager->BTAddPathFollower(index);
	return Status::RUNNING;
}

Node::Status WaitForPath::Execute(const unsigned int index) {
	if (m_BehaviourTree->dwarfManager->HasPath(index)) {
		m_BehaviourTree->doesFlowGoDown[index] = m_BehaviourTree->flowGoUp;
		m_BehaviourTree->currentNode[index] = m_ParentNode;
		return Status::SUCCESS;
	}

	return Status::RUNNING;
}
}
