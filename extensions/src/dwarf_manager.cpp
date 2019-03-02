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

	m_VertexArray = sf::VertexArray(sf::Quads, 0);

#ifdef DEBUG_SPAWN_DWARF
	m_Config = m_Engine.GetConfig();
	m_ScreenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);
	//Create dwarfs
	for (auto i = 0u; i < m_DwarfToSpawn; i++)
	{
		const Vec2f pos(std::rand() % static_cast<int>(m_ScreenSize.x), std::rand() % static_cast<int>(m_ScreenSize.y));

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

	auto indexNewDwarf = GetIndexForNewEntity();

	if(indexNewDwarf + 1 > m_IndexDwarfsEntities)
	{
		m_IndexDwarfsEntities = indexNewDwarf + 1;
	}

	//Update data for new dwarf in std::vectors
	m_DwarfsEntities[indexNewDwarf] = newEntity;
	m_States[indexNewDwarf] = State::IDLE;
	m_Paths[indexNewDwarf] = std::vector<Vec2f>();
	m_AssociatedDwelling[indexNewDwarf] = INVALID_ENTITY;
	m_AssociatedWorkingPlace[indexNewDwarf] = INVALID_ENTITY;
	m_DwarfActivities[indexNewDwarf] = DwarfActivity::IDLE;

	const auto textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

	indexNewDwarf *= 4;

	m_VertexArray[indexNewDwarf].texCoords = sf::Vector2f(0, 0);
	m_VertexArray[indexNewDwarf + 1].texCoords = sf::Vector2f(textureSize.x, 0);
	m_VertexArray[indexNewDwarf + 2].texCoords = textureSize;
	m_VertexArray[indexNewDwarf + 3].texCoords = sf::Vector2f(0, textureSize.y);

	//Add transform
	auto* transformPtr = m_Transform2DManager->AddComponent(newEntity);
	transformPtr->Position = pos;

	m_VertexArray[indexNewDwarf].position = transformPtr->Position - textureSize / 2.0f;
	m_VertexArray[indexNewDwarf + 1].position = transformPtr->Position + sf::Vector2f(
		textureSize.x / 2.0f, -textureSize.y / 2.0f);
	m_VertexArray[indexNewDwarf + 2].position = transformPtr->Position + textureSize / 2.0f;
	m_VertexArray[indexNewDwarf + 3].position = transformPtr->Position + sf::Vector2f(
		-textureSize.x / 2.0f, textureSize.y / 2.0f);
}

void DwarfManager::DestroyDwarfByIndex(const unsigned int index)
{
	//Destroy entity
	auto* entityManager = m_Engine.GetEntityManager();
	entityManager->DestroyEntity(m_DwarfsEntities[index]);

	//Clean all values
	m_DwarfsEntities[index] = INVALID_ENTITY;
	m_Paths[index].clear();
	m_States[index] = State::INVALID;
	m_AssociatedDwelling[index] = INVALID_ENTITY;
	m_AssociatedWorkingPlace[index] = INVALID_ENTITY;

	//Reset vertexArray to non-visible
	m_VertexArray[4 * index + 0].texCoords = sf::Vector2f(0, 0);
	m_VertexArray[4 * index + 1].texCoords = sf::Vector2f(0, 0);
	m_VertexArray[4 * index + 2].texCoords = sf::Vector2f(0, 0);
	m_VertexArray[4 * index + 3].texCoords = sf::Vector2f(0, 0);

	m_VertexArray[4 * index + 0].position = sf::Vector2f(0, 0);
	m_VertexArray[4 * index + 1].position = sf::Vector2f(0, 0);
	m_VertexArray[4 * index + 2].position = sf::Vector2f(0, 0);
	m_VertexArray[4 * index + 3].position = sf::Vector2f(0, 0);

	//Batching data
	m_DwarfActivities[index] = DwarfActivity::IDLE;
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

bool DwarfManager::AssignDwellingToDwarf(const unsigned int index)
{
	auto const dwellingEntity = m_BuildingManager->AttributeDwarfToDwelling();

	if (dwellingEntity == INVALID_ENTITY)
	{
		return false;
	}

	m_AssociatedDwelling[index] = dwellingEntity;

	return true;
}

bool DwarfManager::IsDwarfAtDestination(const unsigned int index)
{
	auto& dwarfPosition = *m_Positions.at(index);

	const auto distance = m_Paths[index].back() - dwarfPosition;

	if (sqrtf(distance.x*distance.x + distance.y * distance.y) < m_StoppingDistance)
	{
		m_Paths[index].pop_back();
		
		if (!m_Paths[index].empty()) {
			const auto velocity = m_Paths[index].back() - dwarfPosition;
			m_VelocitiesComponents[index] = Vec2f(velocity.x, velocity.y) / sqrtf(velocity.x*velocity.x + velocity.y * velocity.y);
			return false;
		}

		m_VelocitiesComponents[index] = Vec2f(0, 0);
		return true;
	}
	return false;
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
	m_DestinationForPathFinding[index] = Vec2f(std::rand() % static_cast<int>(m_ScreenSize.x), std::rand() % static_cast<int>(m_ScreenSize.y));
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

void DwarfManager::DwarfEnterDwelling(const unsigned int index)
{
	m_BuildingManager->DwarfEnterBuilding(DWELLING, GetDwellingEntity(index));
}

void DwarfManager::DwarfExitDwelling(const unsigned int index)
{
	m_BuildingManager->DwarfExitBuilding(DWELLING, GetDwellingEntity(index));
}

void DwarfManager::DwarfEnterWorkingPlace(const unsigned int index)
{
	m_BuildingManager->DwarfEnterBuilding(m_AssociatedWorkingPlaceType[index], m_AssociatedWorkingPlace[index]);
}

void DwarfManager::DwarfExitWorkingPlace(const unsigned int index)
{
	m_BuildingManager->DwarfExitBuilding(m_AssociatedWorkingPlaceType[index], m_AssociatedWorkingPlace[index]);
}

bool DwarfManager::HasJob(const unsigned int index)
{
	return m_AssociatedWorkingPlaceType[index] != NO_BUILDING_TYPE;
}

bool DwarfManager::HasStaticJob(const unsigned int index)
{
	return m_AssociatedWorkingPlaceType[index] != WAREHOUSE;
}

std::mutex AssignJobMutex;
bool DwarfManager::AssignJob(const unsigned int index)
{
	//TODO enlever le lock pour que le tout soit plus rapide
	std::lock_guard<std::mutex> lock(AssignJobMutex);
	char nbJob = 0;

	while (nbJob < m_JobBuildingType.size())
	{
		const auto buildingType = m_JobBuildingType.front();
		m_JobBuildingType.pop();

		const auto buildingEntity = m_BuildingManager->AttributeDwarfToWorkingPlace(buildingType);

		m_JobBuildingType.push(buildingType);

		if (buildingEntity != INVALID_ENTITY)
		{
			m_AssociatedWorkingPlace[index] = buildingEntity;
			m_AssociatedWorkingPlaceType[index] = buildingType;
			return true;
		}

		nbJob++;
	}

	return false;
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

void DwarfManager::ResizeContainers()
{
	const auto newSize = m_DwarfsEntities.size() + m_ContainersExtender;

	m_DwarfsEntities.resize(newSize, INVALID_ENTITY);
	m_Paths.resize(newSize);
	m_States.resize(newSize);

	m_IndexesToDraw.resize(newSize);

	//Associate behaviour tree
	auto* behaviorTree = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<behavior_tree::BehaviorTree>(
		"BehaviorTree");
	behaviorTree->SetEntities(&m_DwarfsEntities);
	m_AssociatedDwelling.resize(newSize, INVALID_ENTITY);
	m_AssociatedWorkingPlace.resize(newSize, INVALID_ENTITY);
	m_AssociatedWorkingPlaceType.resize(newSize, NO_BUILDING_TYPE);
	m_VertexArray.resize(m_VertexArray.getVertexCount() + 4 * m_ContainersExtender);


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

void DwarfManager::AddDwarfToDraw(const unsigned int index)
{
	m_IndexesToDraw[m_IndexToDraw] = index;
	m_IndexToDraw++;
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
		std::vector<std::future<void>> joinFutures(3);
		auto updateFunction = std::bind(&DwarfManager::BatchPathFindingRequest, this);
		joinFutures[0] = m_ThreadPool->push(updateFunction);

		auto updateFunction1 = std::bind(&DwarfManager::BatchPathFollowing, this);
		joinFutures[1] = m_ThreadPool->push(updateFunction1);

		auto updateFunction2 = std::bind(&DwarfManager::BatchPosition, this);
		joinFutures[2] = m_ThreadPool->push(updateFunction2);

		for (auto i = 0; i < 3; i++)
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
		for (size_t i = 0; i < m_PathFollowBatchSize; ++i)
		{
			const auto indexDwarf = m_PathFollowBatch[i];

			*m_Positions[indexDwarf] += m_VelocitiesComponents[indexDwarf] * vel;
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
		for (size_t i = 0; i < m_PathFollowBatchSize; ++i)
		{
			const auto indexDwarf = m_PathFollowBatch[i];

			if (IsDwarfAtDestination(indexDwarf))
			{
				m_EntitiesActiveInBehaviorTree[indexDwarf] = true;
				m_DwarfActivities[i] = DwarfActivity::IDLE;
			}
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
		}
		break;
	case NIGHT:
		if (m_CurrentTime > m_NightDuration)
		{
			m_CurrentTime = 0;
			m_DayState = DayState::DAY;
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
		sizeVertexArrayPath += m_Paths[i].size();
		sizeVertexArrayPath += 2;
	}

	sizeVertexArrayPath -= 2;

	auto index = 0;
	sf::VertexArray lines{ sf::LineStrip, static_cast<size_t>(sizeVertexArrayPath) };
	Vec2f lastPos;

	for (auto i = 0u; i < m_IndexDwarfsEntities; i++)
	{
		auto path = m_Paths[i];
		if(i > 0)
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

		for (auto & p: path)
		{
			lines[index].position = p;
			lines[index].color = color;

			index++;
		}

		lastPos = lines[index - 1].position;
	}

	window->draw(lines);
#endif
	//Draw dwarf
	const auto halfTextureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y) / 2.0f;
	for (size_t i = 0; i < m_IndexDwarfsEntities; i++)
	{
		const auto position = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[i])->Position;
		const auto indexVertex = 4 * i;
		
		m_VertexArray[indexVertex].position = position - halfTextureSize;
		m_VertexArray[indexVertex + 1].position = position + sf::Vector2f(halfTextureSize.x, -halfTextureSize.y);
		m_VertexArray[indexVertex + 2].position = position + halfTextureSize;
		m_VertexArray[indexVertex + 3].position = position + sf::Vector2f(-halfTextureSize.x, halfTextureSize.y);
	}
	m_IndexToDraw = 0;

	window->draw(m_VertexArray, m_Texture);
}
}
