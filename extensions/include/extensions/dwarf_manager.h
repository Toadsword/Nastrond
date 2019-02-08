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

	/**
	 * \brief Assign a dwelling to a dwarf
	 * \param index of the dwarf
	 * \param dwellingEntity to assign to the dwarf
	 */
	void AssignDwellingToDwarf(unsigned int index, Entity dwellingEntity);

	bool IsDwarfAtDestination(unsigned int index);

	bool HasPath(unsigned int index);

	//Behaviour tree communication function
	void AddFindPathToDwellingBT(unsigned int index);

	void AddFindRandomPathBT(unsigned int index);

	void AddPathFollowingBT(unsigned int index);

private:
	void ResizeContainers();
	int GetIndexForNewEntity();

	void AddDwarfToDraw(unsigned int index);

	//System
	Transform2dManager* m_Transform2DManager;
	TextureManager* m_TextureManager;
	SpriteManager* m_SpriteManager;
	NavigationGraphManager* m_NavigationGraphManager;

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

	//Buildings
	std::vector<Entity> m_AssociatedDwelling;
	std::vector<Entity> m_AssociatedWorkingPlace;

	//Vertex array
	sf::VertexArray m_VertexArray;
	std::vector<unsigned int> m_IndexesToDraw;
	unsigned int m_IndexToDraw = 0;

	//Data filed by the behaviourTree
	std::vector<int> m_PathToDwellingBT;
	unsigned int m_IndexPathToDwellingBT = 0;

	std::vector<int> m_PathToRandomBT;
	unsigned int m_IndexPathToRandomBT = 0;

	std::vector<int> m_PathFollowingBT;
	unsigned int m_IndexPathFollowingBT = 0;
};
}

#endif
