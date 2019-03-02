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
#ifndef DWARF_MANAGER_H
#define DWARF_MANAGER_H

#include <engine/system.h>
#include <graphics/graphics2d.h>
#include <extensions/AI/navigation_graph_manager.h>
#include <extensions/Building/building_utilities.h>
#include <extensions/Building/building_manager.h>

namespace sfge::ext
{
#define DEBUG_DRAW_PATH
#define DEBUG_SPAWN_DWARF
#define DEBUG_RANDOM_PATH

#define AI_DEBUG_COUNT_TIME
#define AI_DEBUG_COUNT_TIME_PRECISE

/**
 * \author Nicolas Schneider
 */
class DwarfManager : public System
{
public:
	DwarfManager(Engine& engine);

	~DwarfManager();

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

#pragma region Instantiation / destruction of dwarf
	/**
	 * \brief Spawn a new Dwarf at the given position
	 * \param pos where the dwarf will spawn
	 */
	void InstantiateDwarf(const Vec2f pos);

	/**
	 * \brief Destroy a dwarf using its entity
	 * \param entity to destroy
	 */
	void DestroyDwarfByEntity(Entity entity);

	/**
	 * \brief Destroy a dwarf using its index inside the entites container
	 * \param index to destroy
	 */
	void DestroyDwarfByIndex(unsigned int index);
#pragma endregion 

#pragma region Building associated
	/**
	 * \brief Get the dwelling associated to a dwarf
	 * \param index of the dwarf
	 * \return entity of dwelling, if none return INVALID_ENTIY
	 */
	Entity GetDwellingEntity(unsigned int index);

	/**
	 * \brief Return the position of the dwelling associated to the given dwarf
	 * \param index of the dwarf
	 * \return position of the dwelling
	 */
	Vec2f GetDwellingAssociatedPosition(unsigned int index);

	/**
	 * \brief Return the position of the current working place of the given dwarf
	 * \param index  of the dwarfs
	 * \return position of the working place
	 */
	Vec2f GetWorkingPlaceAssociatedPosition(unsigned int index);

	 /**
	 * \brief Assign a dwelling to a dwarf
	 * \param index of the dwarf
	 * \param dwellingEntity to assign to the dwarf
	 */
	bool AssignDwellingToDwarf(unsigned int index);
#pragma endregion 

#pragma region Path
	/**
	 * \brief test if dwarf has a assigned path
	 * \param index of the dwarf
	 * \return true if has a path
	 */
	bool HasPath(unsigned int index);

	/**
	 * \brief 
	 * \param index 
	 * \param path 
	 */
	void SetPath(unsigned int index, std::vector<Vec2f> path);

	/**
	 * \brief call from the behaviour tree to add a request for a new path
	 * \param index of the dwarf
	 * \param destination position of the destination
	 */
	void AddFindPathToDestinationBT(unsigned int index, Vec2f destination);

	/**
	 * \brief call from the behaviour tree to add a resquest for a new path to a random position
	 * \param index of the dwarf
	 */
	void AddFindRandomPathBT(unsigned int index);

	/**
	 * \brief call from the behaviour tree ta add a dwarf that must folow its path
	 * \param index of the dwarf
	 */
	void AddPathFollowingBT(unsigned int index);

	/**
	 * \brief call from the behavior tree to get a path from the dwarf to the giver
	 * \param index of the dwarf
	 */
	void AddInventoryTaskPathToGiver(unsigned int index);

	/**
	 * \brief call from the behavior tree to get a path from the dwarf to the receiver
	 * \param index of the dwarf
	 */
	void AddInventoryTaskPathToReceiver(unsigned int index);
#pragma endregion 

#pragma region Inventory task
	/**
	 * \brief call from the behavior tree to add a new inventory task to a given dwarf
	 * \param index of the dwarfs
	 * \return true if a inventoryTask has been assigned
	 */
	bool AddInventoryTaskBT(unsigned int index) { 
		BuildingManager::InventoryTask inventoryTask = m_BuildingManager->ConveyorLookForTask();

		if(inventoryTask == m_BuildingManager->INVALID_INVENTORY_TASK)
		{
			return false;
		}

		m_InventoryTaskBT[index] = inventoryTask;
		return true; 
	}

	void TakeResources(unsigned int index)
	{
		m_BuildingManager->DwarfTakesResources(m_InventoryTaskBT[index].giverType, m_InventoryTaskBT[index].giver, m_InventoryTaskBT[index].resourceType);
	}

	void PutResources(unsigned int index)
	{
		m_BuildingManager->DwarfPutsResources(m_InventoryTaskBT[index].receiverType, m_InventoryTaskBT[index].receiver, m_InventoryTaskBT[index].resourceType, m_InventoryTaskBT[index].resourceQuantity);
	}
#pragma endregion 
	
#pragma region Enter / Exit building
	/**
	 * \brief call from the behavior tree when a dwarf enter its dwelling
	 * \param index of the dwarf
	 */
	void DwarfEnterDwelling(unsigned int index);

	/**
	 * \brief call from the behavior tree when a dwarf exit its dwelling
	 * \param index of the dwarf
	 */
	void DwarfExitDwelling(unsigned int index);

	/**
	 * \brief call from the behavior tree when a dwarf enter its working place
	 * \param index of the dwarf
	 */
	void DwarfEnterWorkingPlace(unsigned int index);

	/**
	 * \brief call from the behavior tree when a dwarf exit its working place
	 * \param index of the dwarf
	 */
	void DwarfExitWorkingPlace(unsigned int index);
#pragma endregion 
	
#pragma region Job
	/**
	 * \brief Test if a given dwarf has a job
	 * \param index of the dwarf
	 * \return true if the dwarf has a job
	 */
	bool HasJob(unsigned int index);

	/**
	 * \brief Test if a given dwarf has a static job inside a building
	 * \param index of the dwarf
	 * \return true if has a static job
	 */
	bool HasStaticJob(unsigned int index);

	/**
	 * \brief Assign a job to a dwarf
	 * \param index of the dwarf
	 * \return true if has been able to assign a new job
	 */
	bool AssignJob(unsigned int index);
#pragma endregion 

#pragma region Time of the day
	/**
	 * \brief Test if it's day time
	 * \return true if it's day time
	 */
	bool IsDayTime() const;

	/**
	* \brief Test if it's night time
	* \return true if it's night time
	*/
	bool IsNightTime() const;
#pragma endregion 

private:
	void BatchPathFindingRequest();
	void BatchPathFollowing();
	void BatchPosition();

	void ResizeContainers();
	int GetIndexForNewEntity();

	void UpdatePositionRange(int startIndex, int endIndex, float vel);

	void CheckIsAtDestinationRange(int startIndex, int endIndex);
	bool IsDwarfAtDestination(unsigned int index);

	//System
	Transform2dManager* m_Transform2DManager;
	TextureManager* m_TextureManager;
	SpriteManager* m_SpriteManager;
	NavigationGraphManager* m_NavigationGraphManager;
	BuildingManager* m_BuildingManager;

	//Dwarfs Holder
	const size_t m_ContainersExtender = 100'000;
	std::vector<Entity> m_DwarfsEntities;
	int m_IndexDwarfsEntities = 0;

	//State management
	enum State
	{
		INVALID,
		IDLE,
		WALKING,
		WAITING_NEW_PATH
	};

	std::vector<State> m_States;

	//Path management
	std::vector<std::vector<Vec2f>> m_Paths;
	const float m_StoppingDistance = 10;
	std::vector<Vec2f> m_DestinationForPathFinding;
	std::vector<Vec2f> m_VelocitiesComponents;

	std::vector<Vec2f*> m_Positions;

	//Forces
	float m_FixedDeltaTime = 0.0f;
	const float m_SpeedDwarf = 200;

#ifdef DEBUG_DRAW_PATH
	std::vector<sf::Color> m_Colors{
		sf::Color::Black,
		sf::Color::Blue,
		sf::Color::Cyan,
		sf::Color::Green,
		sf::Color::Magenta,
		sf::Color::Red,
		sf::Color::Yellow
	};
#endif

#ifdef DEBUG_SPAWN_DWARF
	const size_t m_DwarfToSpawn = 1;
#endif

	enum class DwarfActivity : char {
		IDLE,
		FIND_PATH,
		FOLLOW_PATH
	};

	std::vector<DwarfActivity> m_DwarfActivities;

	//Dwarfs texture
	std::string m_TexturePath;
	TextureId m_TextureId;
	sf::Texture* m_Texture;

	//Dwelling
	std::vector<Entity> m_AssociatedDwelling;

	//Jobs
	std::vector<Entity> m_AssociatedWorkingPlace;
	std::vector<BuildingType> m_AssociatedWorkingPlaceType;
	std::queue<BuildingType> m_JobBuildingType;

	//Data filed by the behaviourTree
	std::vector<int> m_PathFindBatch;
	unsigned int m_PathFindBatchSize = 0;

	std::vector<int> m_PathFindingDwarfIndexes;


	std::vector<int> m_PathFollowBatch;
	unsigned int m_PathFollowBatchSize = 0;

	std::vector<bool> m_EntitiesActiveInBehaviorTree;
	std::vector<int> m_EntitiesToWakeUpBatch;
	int m_EntitiesToWakeUpBatchSize = 0;

	//Inventory task
	std::vector<BuildingManager::InventoryTask> m_InventoryTaskBT;

	//Time of Day
	const float m_DayDuration = 10;
	const float m_NightDuration = 5;
	float m_CurrentTime = 0;

	enum DayState
	{
		DAY,
		NIGHT
	};
	DayState m_DayState = DAY;

#ifdef AI_DEBUG_COUNT_TIME
	unsigned __int64 m_TimerMilli = 0u;
	unsigned __int64 m_TimerMicro = 0u;
	int m_TimerCounter = 0;

	unsigned __int64 m_Prebatch_Ms = 0u;
	unsigned __int64 m_Prebatch_Mc = 0u;

	unsigned __int64 m_AskPath_Ms = 0u;
	unsigned __int64 m_AskPath_Mc = 0u;

	unsigned __int64 m_Movement_Ms = 0u;
	unsigned __int64 m_Movement_Mc = 0u;

	unsigned __int64 m_Other_Ms = 0u;
	unsigned __int64 m_Other_Mc = 0u;
#endif

#ifdef DEBUG_RANDOM_PATH
	Configuration* m_Config;
	Vec2f m_ScreenSize;
#endif

	ctpl::thread_pool* m_ThreadPool;
};
}

#endif
