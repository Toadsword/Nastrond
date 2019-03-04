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

#include <extensions/dwarf_manager.h>
#include <python/python_engine.h>
#include <extensions/AI/behavior_tree.h>

namespace sfge::ext
{
DwarfManager::DwarfManager(sfge::Engine& engine) :
	System(engine) {}

DwarfManager::~DwarfManager()
{
#ifdef AI_DEBUG_COUNT_TIME
	std::cout << "[DwarfManager]Update: " << m_TimerMilli / m_TimerCounter << "," << m_TimerMicro / m_TimerCounter
		<< "\n";
#endif
#ifdef AI_DEBUG_COUNT_TIME_PRECISE
	std::cout << "		PreBatch: " << m_Prebatch_Ms / m_TimerCounter << "," << m_Prebatch_Mc / m_TimerCounter << "\n";
	std::cout << "		AskPath: " << m_AskPath_Ms / m_TimerCounter << "," << m_AskPath_Mc / m_TimerCounter << "\n";
	std::cout << "		Movement: " << m_Movement_Ms / m_TimerCounter << "," << m_Movement_Mc / m_TimerCounter << "\n";
	std::cout << "		Other: " << m_Other_Ms / m_TimerCounter << "," << m_Other_Mc / m_TimerCounter << "\n";
#endif
}

void DwarfManager::Init()
{
	//Get managers
	m_Transform2DManager = m_Engine.GetTransform2dManager();
	m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
	m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();
	m_NavigationGraphManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<NavigationGraphManager>(
		"NavigationGraphManager");
	m_BuildingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<BuildingManager>(
		"BuildingManager");
	m_Config = m_Engine.GetConfig();

	//Associate behaviour tree
	auto* behaviorTree = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<behavior_tree::BehaviorTree>(
		"BehaviorTree");
	behaviorTree->SetEntities(&m_DwarfsEntities);

	//Read config
	const auto config = m_Engine.GetConfig();
	m_FixedDeltaTime = m_Engine.GetConfig()->fixedDeltaTime;

	//Load texture
	m_TexturePath = "data/sprites/triangle.png";
	m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
	m_Texture = m_TextureManager->GetTexture(m_TextureId);

#ifdef DEBUG_SPAWN_DWARF
	//Create dwarfs
	for (auto i = 0u; i < m_DwarfToSpawn; i++)
	{
		//TODO modifier d�s que Duncan a rajouter un moyen d'obtenir les bounds de la tilemap au niveau des positions
		const Vec2f pos(std::rand() % static_cast<int>(900) - 450, std::rand() % static_cast<int>(450));

		InstantiateDwarf(pos);
	}
#endif

	//Init job 
	//TODO changer la mani�re dont je cr�e la liste des boulots, il serait plus int�ressant de garder le % de chaque job actuellement attribu�
	m_JobBuildingType.push(WAREHOUSE);
	m_JobBuildingType.push(EXCAVATION_POST);
	m_JobBuildingType.push(FORGE);
	m_JobBuildingType.push(MINE);
	m_JobBuildingType.push(WAREHOUSE);
	m_JobBuildingType.push(MUSHROOM_FARM);

	m_ThreadPool = &m_Engine.GetThreadPool();
}

void DwarfManager::InstantiateDwarf(const Vec2f pos)
{
	auto* entityManager = m_Engine.GetEntityManager();

	auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);

	if (newEntity == INVALID_ENTITY)
	{
		entityManager->ResizeEntityNmb(m_Config->currentEntitiesNmb + m_ContainersExtender);
		newEntity = entityManager->CreateEntity(INVALID_ENTITY);
	}

	const auto indexNewDwarf = GetIndexForNewEntity();

	auto& entityInfo = entityManager->GetEntityInfo(newEntity);
	entityInfo.name = "Dwarf " + std::to_string(indexNewDwarf);


	if(indexNewDwarf + 1 > m_IndexDwarfsEntities)
	{
		m_IndexDwarfsEntities = indexNewDwarf + 1;
	}

	//Update data for new dwarf in std::vectors
	m_DwarfsEntities[indexNewDwarf] = newEntity;
	m_Paths[indexNewDwarf] = std::vector<Vec2f>();
	m_AssociatedDwelling[indexNewDwarf] = INVALID_ENTITY;
	m_AssociatedWorkingPlace[indexNewDwarf] = INVALID_ENTITY;
	m_DwarfActivities[indexNewDwarf] = DwarfActivity::IDLE;

	//Add transform
	auto* transformPtr = m_Transform2DManager->AddComponent(newEntity);
	transformPtr->Position = pos;

	//Add sprite
	auto* sprite = m_SpriteManager->AddComponent(newEntity);
	sprite->SetTexture(m_Texture);

	//Set sprite infos
	auto& spriteInfo = m_SpriteManager->GetComponentInfo(newEntity);
	spriteInfo.name = "Sprite dwarf";
	spriteInfo.sprite = sprite;
	spriteInfo.textureId = m_TextureId;
	spriteInfo.texturePath = m_TexturePath;
}

void DwarfManager::DestroyDwarfByIndex(const unsigned int index)
{
	//Destroy entity
	auto* entityManager = m_Engine.GetEntityManager();
	entityManager->DestroyEntity(m_DwarfsEntities[index]);

	//Clean all values
	m_DwarfsEntities[index] = INVALID_ENTITY;
	m_Paths[index].clear();
	m_AssociatedDwelling[index] = INVALID_ENTITY;
	m_AssociatedWorkingPlace[index] = INVALID_ENTITY;
}

void DwarfManager::DestroyDwarfByEntity(const Entity entity)
{
	auto index = -1;
	for (auto i = 0; i < m_IndexDwarfsEntities; i++)
	{
		if (m_DwarfsEntities[i] == entity)
		{
			//Destroy entity
			auto* entityManager = m_Engine.GetEntityManager();
			entityManager->DestroyEntity(entity);

			index = i;
			break;
		}
	}

	DestroyDwarfByIndex(index);
}

Entity DwarfManager::GetDwellingEntity(const unsigned int index)
{
	return m_AssociatedDwelling[index];
}

Vec2f DwarfManager::GetDwellingAssociatedPosition(const unsigned int index)
{
	return m_Transform2DManager->GetComponentPtr(m_AssociatedDwelling[index])->Position;
}

Vec2f DwarfManager::GetWorkingPlaceAssociatedPosition(const unsigned int index)
{
	return m_Transform2DManager->GetComponentPtr(m_AssociatedWorkingPlace[index])->Position;
}

void DwarfManager::AskAssignDwellingToDwarf(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::ASSIGN_DWELLING;
}

float Sqrt2(float x) {
	auto i = *reinterpret_cast<int*>(&x);
	i = 0x5f3759df - (i >> 1);
	auto r = *reinterpret_cast<float*>(&i);
	r = r * (1.5f - 0.5f*x*r*r);
	return r * x;
}

bool DwarfManager::IsDwarfAtDestination(const unsigned int index)
{
	if (!m_Paths[index].empty()) {
		auto& dwarfPosition = *m_Positions.at(index);

		const auto distance = m_Paths[index].back() - dwarfPosition;

		if (Sqrt2(distance.x*distance.x + distance.y * distance.y) < m_StoppingDistance)
		{
			m_Paths[index].pop_back();

			if (!m_Paths[index].empty()) {
				const auto velocity = m_Paths[index].back() - dwarfPosition;
				m_VelocitiesComponents[index] = Vec2f(velocity.x, velocity.y) / Sqrt2(velocity.x*velocity.x + velocity.y * velocity.y);
				return false;
			}

			m_VelocitiesComponents[index] = Vec2f(0, 0);
			return true;
		}
		return false;
	}
	return true;
}

bool DwarfManager::HasPath(const unsigned int index)
{
	return !m_Paths[index].empty();
}

void DwarfManager::SetPath(const unsigned int index, std::vector<Vec2f> path)
{
	m_Paths[index] = path;
	m_VelocitiesComponents[index] = path.back() - m_Transform2DManager->GetComponentPtr(m_DwarfsEntities[index])->Position;
	m_VelocitiesComponents[index] = m_VelocitiesComponents[index].Normalized();
}

void DwarfManager::AddFindPathToDestinationBT(const unsigned int index, const Vec2f destination)
{
	m_DwarfActivities[index] = DwarfActivity::FIND_PATH;
	m_DestinationForPathFinding[index] = destination;
}

void DwarfManager::AddFindRandomPathBT(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::FIND_PATH;
	const Vec2f pos(std::rand() % static_cast<int>(1600) - 800, std::rand() % static_cast<int>(800));
	m_DestinationForPathFinding[index] = pos;
}

void DwarfManager::AddPathFollowingBT(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::FOLLOW_PATH;
}

void DwarfManager::AddInventoryTaskPathToGiver(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::FIND_PATH;
	m_DestinationForPathFinding[index] = m_Transform2DManager
		->GetComponentPtr(m_InventoryTaskBT[index].giver)->Position;
}

void DwarfManager::AddInventoryTaskPathToReceiver(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::FIND_PATH;
	m_DestinationForPathFinding[index] = m_Transform2DManager
		->GetComponentPtr(m_InventoryTaskBT[index].receiver)->Position;
}

void DwarfManager::AddInventoryTaskBT(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::ASSIGN_INVENTORY_TASK;
}

void DwarfManager::TakeResources(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::TAKE_RESOURCE;
}

void DwarfManager::PutResources(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::PUT_RESOURCE;
}

bool DwarfManager::HasInventoryTask(const unsigned int index)
{
	return m_InventoryTaskBT[index].giverType != NO_BUILDING_TYPE;
}

void DwarfManager::DwarfEnterDwelling(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::ENTER_DWELLING;
}

void DwarfManager::DwarfExitDwelling(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::EXIT_DWELLING;
}

void DwarfManager::DwarfEnterWorkingPlace(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::ENTER_WORKING_PLACE;
}

void DwarfManager::DwarfExitWorkingPlace(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::EXIT_WORKING_PLACE;
}

bool DwarfManager::HasJob(const unsigned int index)
{
	return m_AssociatedWorkingPlaceType[index] != NO_BUILDING_TYPE;
}

bool DwarfManager::HasStaticJob(const unsigned int index)
{
	return m_AssociatedWorkingPlaceType[index] != WAREHOUSE;
}

void DwarfManager::AssignJob(const unsigned int index)
{
	m_DwarfActivities[index] = DwarfActivity::ASSIGN_JOB;
}

bool DwarfManager::IsDayTime() const
{
	return m_DayState == DAY;
}

bool DwarfManager::IsNightTime() const
{
	return m_DayState == NIGHT;
}

void DwarfManager::BatchPathFindingRequest()
{
	for (size_t i = 0; i < m_IndexDwarfsEntities; i++)
	{
		if (m_DwarfActivities[i] == DwarfActivity::FIND_PATH) {
			m_PathFindingDwarfIndexes[m_PathFindBatchSize] = i;
			m_PathFindBatchSize++;

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}
	}
}

void DwarfManager::BatchPathFollowing()
{
	for (size_t i = 0; i < m_IndexDwarfsEntities; i++)
	{
		if (m_DwarfActivities[i] == DwarfActivity::FOLLOW_PATH) {
			m_PathFollowBatch[m_PathFollowBatchSize] = i;
			m_EntitiesActiveInBehaviorTree[i] = false; //TODO enlever quand il y aura un vector unique
			m_PathFollowBatchSize++;
		}
	}
}

void DwarfManager::BatchPosition()
{
	for (size_t i = 0; i < m_IndexDwarfsEntities; i++)
	{
		m_Positions[i] = &m_Transform2DManager->GetComponentPtr(m_DwarfsEntities[i])->Position;
 	}
}

void DwarfManager::BatchAssignDwelling()
{
	for (size_t i = 0; i < m_IndexDwarfsEntities; i++)
	{
		if (m_DwarfActivities[i] == DwarfActivity::ASSIGN_DWELLING) {
			auto const dwellingEntity = m_BuildingManager->AttributeDwarfToDwelling();
			if(dwellingEntity == INVALID_ENTITY)
			{
				std::cout << "BUG DE TA MERE\n";
			}else
			{
				std::cout << "Set house for" << i << " = " << dwellingEntity << "\n";
			}
			m_AssociatedDwelling[i] = dwellingEntity;

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}

		if (m_DwarfActivities[i] == DwarfActivity::ASSIGN_INVENTORY_TASK) {
			const auto inventoryTask = m_BuildingManager->ConveyorLookForTask();

			m_InventoryTaskBT[i] = inventoryTask;

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}

		if(m_DwarfActivities[i] == DwarfActivity::TAKE_RESOURCE)
		{
			m_BuildingManager->DwarfTakesResources(m_InventoryTaskBT[i].giverType, m_InventoryTaskBT[i].giver,
				m_InventoryTaskBT[i].resourceType);

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}

		if(m_DwarfActivities[i] == DwarfActivity::PUT_RESOURCE)
		{
			m_BuildingManager->DwarfPutsResources(m_InventoryTaskBT[i].receiverType, m_InventoryTaskBT[i].receiver,
				m_InventoryTaskBT[i].resourceType,
				m_InventoryTaskBT[i].resourceQuantity);

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}

		if(m_DwarfActivities[i] == DwarfActivity::ENTER_DWELLING)
		{
			m_BuildingManager->DwarfEnterBuilding(DWELLING, GetDwellingEntity(i));

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}

		if(m_DwarfActivities[i] == DwarfActivity::EXIT_DWELLING)
		{
			m_BuildingManager->DwarfExitBuilding(DWELLING, GetDwellingEntity(i));

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}

		if (m_DwarfActivities[i] == DwarfActivity::ENTER_WORKING_PLACE)
		{
			m_BuildingManager->DwarfEnterBuilding(m_AssociatedWorkingPlaceType[i], m_AssociatedWorkingPlace[i]);

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}

		if (m_DwarfActivities[i] == DwarfActivity::EXIT_WORKING_PLACE)
		{
			m_BuildingManager->DwarfExitBuilding(m_AssociatedWorkingPlaceType[i], m_AssociatedWorkingPlace[i]);

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}

		if (m_DwarfActivities[i] == DwarfActivity::ASSIGN_JOB)
		{
			char nbJob = 0;

			while (nbJob < m_JobBuildingType.size())
			{
				const auto buildingType = m_JobBuildingType.front();
				m_JobBuildingType.pop();

				const auto buildingEntity = m_BuildingManager->AttributeDwarfToWorkingPlace(buildingType);

				m_JobBuildingType.push(buildingType);

				if (buildingEntity != INVALID_ENTITY)
				{
					m_AssociatedWorkingPlace[i] = buildingEntity;
					m_AssociatedWorkingPlaceType[i] = buildingType;

					break;
				}

				nbJob++;
			}

			m_DwarfActivities[i] = DwarfActivity::IDLE;
		}
	}
}

void DwarfManager::ResizeContainers()
{
	const auto newSize = m_DwarfsEntities.size() + m_ContainersExtender;

	m_DwarfsEntities.resize(newSize, INVALID_ENTITY);
	m_Paths.resize(newSize);

	//Associate behaviour tree
	auto* behaviorTree = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<behavior_tree::BehaviorTree>(
		"BehaviorTree");
	behaviorTree->SetEntities(&m_DwarfsEntities);
	m_AssociatedDwelling.resize(newSize, INVALID_ENTITY);
	m_AssociatedWorkingPlace.resize(newSize, INVALID_ENTITY);
	m_AssociatedWorkingPlaceType.resize(newSize, NO_BUILDING_TYPE);


	m_PathFollowBatch.resize(newSize);

	m_PathFindBatch.resize(newSize);

	m_PathFindingDwarfIndexes.resize(newSize);

	m_DestinationForPathFinding.resize(newSize);

	m_InventoryTaskBT.resize(newSize);

	m_EntitiesToWakeUpBatch.resize(newSize);

	m_EntitiesActiveInBehaviorTree.resize(newSize, true);

	m_DwarfActivities.resize(newSize, DwarfActivity::IDLE);

	//Velocities
	m_VelocitiesComponents.resize(newSize);
	m_Positions.resize(newSize);
}

int DwarfManager::GetIndexForNewEntity()
{
	//Check if a place is free
	for (auto i = 0; i < m_DwarfsEntities.size(); i++)
	{
		if (m_DwarfsEntities[i] == INVALID_ENTITY)
		{
			return i;
		}
	}

	//Otherwise resize all std::vectors
	const int index = m_DwarfsEntities.size();
	ResizeContainers();
	return index;
}

void DwarfManager::UpdatePositionRange(const int startIndex, const int endIndex, const float vel)
{
	for (size_t i = startIndex; i <= endIndex; ++i)
	{
		const auto indexDwarf = m_PathFollowBatch[i];
		*m_Positions[indexDwarf] += m_VelocitiesComponents[indexDwarf] * vel;
	}
}

void DwarfManager::CheckIsAtDestinationRange(const int startIndex, const int endIndex)
{
	for (size_t i = startIndex; i <= endIndex; ++i)
	{
		const auto indexDwarf = m_PathFollowBatch[i];

		if (IsDwarfAtDestination(indexDwarf))
		{
			m_EntitiesActiveInBehaviorTree[indexDwarf] = true;
			m_DwarfActivities[indexDwarf] = DwarfActivity::IDLE;
		}
	}
}

void DwarfManager::Update(const float dt)
{
#ifdef AI_DEBUG_COUNT_TIME
	const auto t1 = std::chrono::high_resolution_clock::now();
#endif

#pragma region prebatching data
#ifdef AI_DEBUG_COUNT_TIME_PRECISE
	const auto t3 = std::chrono::high_resolution_clock::now();
#endif
	//Batch data
	{
		std::vector<std::future<void>> joinFutures(4);
		auto updateFunction = std::bind(&DwarfManager::BatchPathFindingRequest, this);
		joinFutures[0] = m_ThreadPool->push(updateFunction);

		auto updateFunction1 = std::bind(&DwarfManager::BatchPathFollowing, this);
		joinFutures[1] = m_ThreadPool->push(updateFunction1);

		auto updateFunction2 = std::bind(&DwarfManager::BatchPosition, this);
		joinFutures[2] = m_ThreadPool->push(updateFunction2);

		auto updateFunction3 = std::bind(&DwarfManager::BatchAssignDwelling, this);
		joinFutures[3] = m_ThreadPool->push(updateFunction3);

		for (auto i = 0; i < 4; i++)
		{
			joinFutures[i].get();
		}
	}

#ifdef AI_DEBUG_COUNT_TIME_PRECISE
	const auto t4 = std::chrono::high_resolution_clock::now();
	{
		const auto timerDuration2 = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
		m_Prebatch_Ms += timerDuration2 / 1000;
		m_Prebatch_Mc += timerDuration2 % 1000;
	}
#endif
#pragma endregion 

#pragma region ask for path
#ifdef AI_DEBUG_COUNT_TIME_PRECISE
	const auto t5 = std::chrono::high_resolution_clock::now();
#endif
	//Random path
	for (size_t i = 0; i < m_PathFindBatchSize; ++i)
	{
		const auto indexDwarf = m_PathFindingDwarfIndexes[i]; 
		
		m_NavigationGraphManager->AskForPath(indexDwarf, *m_Positions[indexDwarf], m_DestinationForPathFinding[indexDwarf]);
	}
	m_PathFindBatchSize = 0;
#ifdef AI_DEBUG_COUNT_TIME_PRECISE
	const auto t6 = std::chrono::high_resolution_clock::now();
	{
		const auto timerDuration2 = std::chrono::duration_cast<std::chrono::microseconds>(t6 - t5).count();
		m_AskPath_Ms += timerDuration2 / 1000;
		m_AskPath_Mc += timerDuration2 % 1000;
	}
#endif
#pragma endregion 

#pragma region movement
#ifdef AI_DEBUG_COUNT_TIME_PRECISE
	const auto t7 = std::chrono::high_resolution_clock::now();
#endif
	const auto vel = m_SpeedDwarf * dt;

	//Update position
	const auto coreNmb = m_ThreadPool->size();
	if (m_PathFollowBatchSize > coreNmb) {

		std::vector<std::future<void>> joinFutures2(coreNmb);
		for (auto threadIndex = 0; threadIndex < coreNmb; threadIndex++)
		{
			auto start = (threadIndex + 1)*m_PathFollowBatchSize / (coreNmb + 1);
			auto end = (threadIndex + 2)*m_PathFollowBatchSize / (coreNmb + 1) - 1;
			auto updateFunction3 = std::bind(&DwarfManager::UpdatePositionRange, this, start, end, vel);
			joinFutures2[threadIndex] = m_ThreadPool->push(updateFunction3);
		}
		UpdatePositionRange(0, m_PathFollowBatchSize / (coreNmb + 1) - 1, vel);
		for (auto i = 0; i < coreNmb; i++)
		{
			joinFutures2[i].get();
		}
	}else
	{
		if (m_PathFollowBatchSize > 0) {
			UpdatePositionRange(0, m_PathFollowBatchSize - 1, vel);
		}
	}

	//Check if is at destination
	if (m_PathFollowBatchSize > coreNmb) {
		std::vector<std::future<void>> joinFutures2(coreNmb);
		for (auto threadIndex = 0; threadIndex < coreNmb; threadIndex++)
		{
			auto start = (threadIndex + 1)*m_PathFollowBatchSize / (coreNmb + 1);
			auto end = (threadIndex + 2)*m_PathFollowBatchSize / (coreNmb + 1) - 1;
			auto updateFunction3 = std::bind(&DwarfManager::CheckIsAtDestinationRange, this, start, end);
			joinFutures2[threadIndex] = m_ThreadPool->push(updateFunction3);
		}
		CheckIsAtDestinationRange(0, m_PathFollowBatchSize / (coreNmb + 1) - 1);
		for (auto i = 0; i < coreNmb; i++)
		{
			joinFutures2[i].get();
		}
	}else
	{
		if (m_PathFollowBatchSize > 0) {
			CheckIsAtDestinationRange(0, m_PathFollowBatchSize - 1);
		}
	}

	for(size_t i = 0; i < m_IndexDwarfsEntities; i++)
	{
		if(m_EntitiesActiveInBehaviorTree[i])
		{
			m_EntitiesToWakeUpBatch[m_EntitiesToWakeUpBatchSize] = i;
			m_EntitiesToWakeUpBatchSize++;
		}
	}

	m_PathFollowBatchSize = 0;
#ifdef AI_DEBUG_COUNT_TIME_PRECISE
	const auto t8 = std::chrono::high_resolution_clock::now();
	{
		const auto timerDuration2 = std::chrono::duration_cast<std::chrono::microseconds>(t8 - t7).count();
		m_Movement_Ms += timerDuration2 / 1000;
		m_Movement_Mc += timerDuration2 % 1000;
	}
#endif
#pragma endregion 

#pragma region other
#ifdef AI_DEBUG_COUNT_TIME_PRECISE
	const auto t9 = std::chrono::high_resolution_clock::now();
#endif
	auto* behaviorTree = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<behavior_tree::BehaviorTree>(
		"BehaviorTree");

	m_PathFollowBatchSize = 0;

	//Wake up
	behaviorTree->WakeUpEntities(m_EntitiesToWakeUpBatch, m_EntitiesToWakeUpBatchSize);

	m_EntitiesToWakeUpBatchSize = 0;

	//Update current time
	m_CurrentTime += dt;
	switch (m_DayState)
	{
	case DAY:
		if(m_CurrentTime > m_DayDuration)
		{
			m_CurrentTime = 0;
			m_DayState = DayState::NIGHT;
			std::cout << "To Night\n";
		}
		break;
	case NIGHT:
		if (m_CurrentTime > m_NightDuration)
		{
			m_CurrentTime = 0;
			m_DayState = DayState::DAY;

			std::cout << "To Day\n";
		}
		break;
	default: ;
	}
#ifdef AI_DEBUG_COUNT_TIME_PRECISE
	const auto t10 = std::chrono::high_resolution_clock::now();
	{
		const auto timerDuration2 = std::chrono::duration_cast<std::chrono::microseconds>(t10 - t9).count();
		m_Other_Ms += timerDuration2 / 1000;
		m_Other_Mc += timerDuration2 % 1000;
	}
#endif
#pragma endregion

#ifdef AI_DEBUG_COUNT_TIME
	const auto t2 = std::chrono::high_resolution_clock::now();
	const auto timerDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	m_TimerMilli += timerDuration / 1000;
	m_TimerMicro += timerDuration % 1000;
	m_TimerCounter++;
#endif
}

void DwarfManager::FixedUpdate()
{
}

void DwarfManager::Draw()
{
	auto window = m_Engine.GetGraphics2dManager()->GetWindow();
#ifdef DEBUG_DRAW_PATH
	auto sizeVertexArrayPath = 0;

	for (auto i = 0u; i < m_IndexDwarfsEntities; i++)
	{
		if (!m_Paths[i].empty()) {
			sizeVertexArrayPath += m_Paths[i].size();
			sizeVertexArrayPath += 2;
		}
	}
	if(sizeVertexArrayPath >= 2)
	sizeVertexArrayPath -= 2;

	auto index = 0;
	sf::VertexArray lines{ sf::LineStrip, static_cast<size_t>(sizeVertexArrayPath) };
	Vec2f lastPos;

	for (auto i = 0u; i < m_IndexDwarfsEntities; i++)
	{
		auto path = m_Paths[i];
		if (i > 0)
		{
			if (!path.empty()) {
				lines[index].position = lastPos;

				lines[index].color = sf::Color(0, 0, 0, 0);
				index++;

				lines[index].position = path[0];

				lines[index].color = sf::Color(0, 0, 0, 0);
				index++;
			}
		}

		const auto color = m_Colors[i % m_Colors.size()];

		for (auto & p : path)
		{
			lines[index].position = p;
			lines[index].color = color;

			index++;
		}

		if(index > 0)
		lastPos = lines[index - 1].position;
	}
	
	window->draw(lines);
#endif
}
}
