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
#include <extensions/navigation_graph_manager.h>
#include <extensions/building_utilities.h>
#include <extensions/building_manager.h>

namespace sfge::ext
{
#define DEBUG_DRAW_PATH
#define DEBUG_SPAWN_DWARF
#define DEBUG_RANDOM_PATH

/**
 * \author Nicolas Schneider
 */
class DwarfManager : public System
{
public:
	DwarfManager(Engine& engine);

	void Init() override;

	void Update(float dt) override;

	void FixedUpdate() override;

	void Draw() override;

	/**
	 * \brief Spawn a new Dwarf at the given position
	 * \param pos where the dwarf will spawn
	 */
	void SpawnDwarf(const Vec2f pos);

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

	//Dwelling 
	/**
	 * \brief Get the dwelling associated to a dwarf
	 * \param index of the dwarf
	 * \return entity of dwelling, if none return INVALID_ENTIY
	 */
	Entity GetDwellingEntity(unsigned int index);

	Vec2f GetDwellingAssociatedPosition(unsigned int index);

	Vec2f GetWorkingPlaceAssociatedPosition(unsigned int index);
	 /**
	 * \brief Assign a dwelling to a dwarf
	 * \param index of the dwarf
	 * \param dwellingEntity to assign to the dwarf
	 */
	bool AssignDwellingToDwarf(unsigned int index);

	//Path related function
	bool IsDwarfAtDestination(unsigned int index);

	bool HasPath(unsigned int index);

	void AddFindPathToDestinationBT(unsigned int index, Vec2f destination);

	void AddFindRandomPathBT(unsigned int index);

	void AddPathFollowingBT(unsigned int index);

	//Inventory task related function
	void AddInventoryTaskPathToGiver(unsigned int index);

	void AddInventoryTaskPathToReceiver(unsigned int index);

	struct InventoryTask final
	{
		Entity giver = INVALID_ENTITY;
		Entity receiver = INVALID_ENTITY;
		BuildingManager::BuildingType giverType = BuildingManager::BuildingType::LENGTH;
		BuildingManager::BuildingType receiverType = BuildingManager::BuildingType::LENGTH;
		ResourceType resourceType = NONE;
		char resourceQuantity = 0;
	};

	void AddInventoryTaskBT(unsigned int index, InventoryTask inventoryTask);

	//enter/exiting building

	void DwarfEnterDwelling(unsigned int index);

	void DwarfExitDwelling(unsigned int index);

	void DwarfEnterWorkingPlace(unsigned int index);

	void DwarfExitWorkingPlace(unsigned int index);

	//job related function

	bool HasJob(unsigned int index);

	bool HasStaticJob(unsigned int index);

	bool AssignJob(unsigned int index);

	//time related function
	bool IsDayTime() const;

	bool IsNightTime() const;

private:
	void ResizeContainers();
	int GetIndexForNewEntity();

	void AddDwarfToDraw(unsigned int index);

	//System
	Transform2dManager* m_Transform2DManager;
	TextureManager* m_TextureManager;
	SpriteManager* m_SpriteManager;
	NavigationGraphManager* m_NavigationGraphManager;
	BuildingManager* m_BuildingManager;

	//Dwarfs Holder
	const size_t m_ContainersExtender = 100;
	std::vector<Entity> m_DwarfsEntities;

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
	const float m_StoppingDistance = 20;

	//Forces
	float m_FixedDeltaTime = 0.0f;
	const float m_SpeedDwarf = 20;

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
	const size_t m_DwarfToSpawn = 50;
#endif

	//Dwarfs texture
	std::string m_TexturePath;
	TextureId m_TextureId;
	sf::Texture* m_Texture;

	//Dwelling
	std::vector<Entity> m_AssociatedDwelling;

	//Jobs
	std::vector<Entity> m_AssociatedWorkingPlace;
	std::vector<BuildingManager::BuildingType> m_AssociatedWorkingPlaceType;
	std::queue<BuildingManager::BuildingType> m_JobBuildingType;

	//Vertex array
	sf::VertexArray m_VertexArray;
	std::vector<unsigned int> m_IndexesToDraw;
	unsigned int m_IndexToDraw = 0;

	//Data filed by the behaviourTree
	std::vector<int> m_PathToIndexDwarfBT;
	std::vector<Vec2f> m_PathToDestinationBT;
	unsigned int m_IndexPathToDestinationBT = 0;

	std::vector<int> m_PathToRandomBT;
	unsigned int m_IndexPathToRandomBT = 0;

	std::vector<int> m_PathFollowingBT;
	unsigned int m_IndexPathFollowingBT = 0;

	//Inventory task
	std::vector<InventoryTask> m_InventoryTaskBT;

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
};
}

#endif
