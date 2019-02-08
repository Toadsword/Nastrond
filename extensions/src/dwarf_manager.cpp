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
#include <extensions/behaviour_tree_core.h>

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
	auto* behaviourTree = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behaviour_tree::BehaviourTree>(
		"BehaviourTree");
	behaviourTree->SetEntities(&m_DwarfsEntities);

	//Read config
	const auto config = m_Engine.GetConfig();
	m_FixedDeltaTime = m_Engine.GetConfig()->fixedDeltaTime;

	//Load texture
	m_TexturePath = "data/sprites/triangle.png";
	m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
	m_Texture = m_TextureManager->GetTexture(m_TextureId);

	m_VertexArray = sf::VertexArray(sf::Quads, 0);

#ifdef DEBUG_SPAWN_DWARF
	const Vec2f screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);
	//Create dwarfs
	for (auto i = 0u; i < m_DwarfToSpawn; i++)
	{
		const Vec2f pos(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y));

		SpawnDwarf(pos);
	}
#endif
}

void DwarfManager::SpawnDwarf(const Vec2f pos)
{
	auto* entityManager = m_Engine.GetEntityManager();
	Configuration* configuration = m_Engine.GetConfig();

	auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);

	if (newEntity == INVALID_ENTITY)
	{
		entityManager->ResizeEntityNmb(configuration->currentEntitiesNmb + m_ContainersExtender);
		newEntity = entityManager->CreateEntity(INVALID_ENTITY);
	}

	const int indexNewDwarf = GetIndexForNewEntity();

	//Update data for new dwarf in std::vectors
	m_DwarfsEntities[indexNewDwarf] = newEntity;
	m_States[indexNewDwarf] = State::IDLE;
	m_Paths[indexNewDwarf] = std::vector<Vec2f>();
	m_AssociatedDwelling[indexNewDwarf] = INVALID_ENTITY;
	m_AssociatedWorkingPlace[indexNewDwarf] = INVALID_ENTITY;

	sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

	m_VertexArray[4 * indexNewDwarf].texCoords = sf::Vector2f(0, 0);
	m_VertexArray[4 * indexNewDwarf + 1].texCoords = sf::Vector2f(textureSize.x, 0);
	m_VertexArray[4 * indexNewDwarf + 2].texCoords = textureSize;
	m_VertexArray[4 * indexNewDwarf + 3].texCoords = sf::Vector2f(0, textureSize.y);

	//Add transform
	auto* transformPtr = m_Transform2DManager->AddComponent(newEntity);
	transformPtr->Position = pos;

	m_VertexArray[4 * indexNewDwarf].position = transformPtr->Position - textureSize / 2.0f;
	m_VertexArray[4 * indexNewDwarf + 1].position = transformPtr->Position + sf::Vector2f(
		textureSize.x / 2.0f, -textureSize.y / 2.0f);
	m_VertexArray[4 * indexNewDwarf + 2].position = transformPtr->Position + textureSize / 2.0f;
	m_VertexArray[4 * indexNewDwarf + 3].position = transformPtr->Position + sf::Vector2f(
		-textureSize.x / 2.0f, textureSize.y / 2.0f);
}

void DwarfManager::DestroyDwarfByIndex(unsigned int index)
{
	//Destroy entity
	auto* entityManager = m_Engine.GetEntityManager();
	entityManager->DestroyEntity(m_DwarfsEntities[index]);

	//Clean all values
	m_DwarfsEntities[index] = INVALID_ENTITY;
	m_Paths[index] = std::vector<Vec2f>(0);
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
}

void DwarfManager::DestroyDwarfByEntity(Entity entity)
{
	auto index = -1;
	for (auto i = 0; i < m_DwarfsEntities.size(); i++)
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

	if (index != -1)
	{
		//Clean all values
		m_Paths[index] = std::vector<Vec2f>(0);
		m_States[index] = State::INVALID;
		m_AssociatedDwelling[index] = INVALID_ENTITY;
		m_AssociatedWorkingPlace[index] = INVALID_ENTITY;
		m_DwarfsEntities[index] = INVALID_ENTITY;

		//Reset vertexArray
		m_VertexArray[4 * index].texCoords = sf::Vector2f(0, 0);
		m_VertexArray[4 * index + 1].texCoords = sf::Vector2f(0, 0);
		m_VertexArray[4 * index + 2].texCoords = sf::Vector2f(0, 0);
		m_VertexArray[4 * index + 3].texCoords = sf::Vector2f(0, 0);

		m_VertexArray[4 * index].position = sf::Vector2f(0, 0);
		m_VertexArray[4 * index + 1].position = sf::Vector2f(0, 0);
		m_VertexArray[4 * index + 2].position = sf::Vector2f(0, 0);
		m_VertexArray[4 * index + 3].position = sf::Vector2f(0, 0);
	}
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
	auto const dwellingEntity = m_BuildingManager->GetDwellingFreeSlots(); //TODO changer

	if (dwellingEntity == INVALID_ENTITY)
	{
		return false;
	}
	else
	{
		m_AssociatedDwelling[index] = dwellingEntity;
		m_BuildingManager->AddDwarfToBuilding(dwellingEntity);

		return true;
	}
}

bool DwarfManager::IsDwarfAtDestination(const unsigned int index)
{
	if (!m_Paths[index].empty())
	{
		const auto position = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[index])->Position;

		auto dir = m_Paths[index][0] - position;
		if (dir.GetMagnitude() < m_StoppingDistance)
		{
			m_Paths[index].clear();
			return true;
		}

		if (m_Paths[index].size() > 1) {

			dir = m_Paths[index][m_Paths[index].size() - 1] - position;

			if (dir.GetMagnitude() < m_StoppingDistance)
			{
				m_Paths[index].pop_back();
			}
		}

		return false;
	}
	return true;
}

bool DwarfManager::HasPath(unsigned int index)
{
	return !m_Paths[index].empty();
}

void DwarfManager::AddFindPathToDestinationBT(const unsigned int index, const Vec2f destination)
{
	m_PathToIndexDwarfBT[m_IndexPathToDestinationBT] = index;
	m_PathToDestinationBT[m_IndexPathToDestinationBT] = destination;
	m_IndexPathToDestinationBT++;
}

void DwarfManager::AddFindRandomPathBT(unsigned int index)
{
	m_PathToRandomBT[m_IndexPathToRandomBT] = index;
	m_IndexPathToRandomBT++;
}

void DwarfManager::AddPathFollowingBT(unsigned int index)
{
	m_PathFollowingBT[m_IndexPathFollowingBT] = index;
	m_IndexPathFollowingBT++;
}

void DwarfManager::AddInventoryTaskPathToGiver(const unsigned int index)
{
	m_PathToIndexDwarfBT[m_IndexPathToDestinationBT] = index;
	m_PathToDestinationBT[m_IndexPathToDestinationBT] = m_Transform2DManager
	                                                    ->GetComponentPtr(m_InventoryTaskBT[index].giver)->Position;
	m_IndexPathToDestinationBT++;
}

void DwarfManager::AddInventoryTaskPathToReceiver(const unsigned int index)
{
	m_PathToIndexDwarfBT[m_IndexPathToDestinationBT] = index;
	m_PathToDestinationBT[m_IndexPathToDestinationBT] = m_Transform2DManager
	                                                    ->GetComponentPtr(m_InventoryTaskBT[index].receiver)->Position;
	m_IndexPathToDestinationBT++;
}

void DwarfManager::AddInventoryTaskBT(const unsigned int index, const InventoryTask inventoryTask)
{
	m_InventoryTaskBT[index] = inventoryTask;
}

void DwarfManager::DwarfEnterDwelling(const unsigned int index)
{
	m_BuildingManager->DwarfEnterBuilding(BuildingManager::BuildingType::DWELLING, GetDwellingEntity(index));
}

void DwarfManager::DwarfExitDwelling(const unsigned int index)
{
	m_BuildingManager->DwarfExitBuilding(BuildingManager::BuildingType::DWELLING, GetDwellingEntity(index));
}

void DwarfManager::ResizeContainers()
{
	const auto newSize = m_DwarfsEntities.size() + m_ContainersExtender;

	m_DwarfsEntities.resize(newSize, INVALID_ENTITY);
	m_Paths.resize(newSize);
	m_States.resize(newSize);

	m_IndexesToDraw.resize(newSize);

	//Associate behaviour tree
	auto* behaviourTree = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<behaviour_tree::BehaviourTree>(
		"BehaviourTree");
	behaviourTree->SetEntities(&m_DwarfsEntities);
	m_AssociatedDwelling.resize(newSize, INVALID_ENTITY);
	m_AssociatedWorkingPlace.resize(newSize, INVALID_ENTITY);
	m_VertexArray.resize(m_VertexArray.getVertexCount() * 4 + 4 * m_ContainersExtender);

	m_PathFollowingBT.resize(newSize);
	m_PathToIndexDwarfBT.resize(newSize);
	m_PathToDestinationBT.resize(newSize);
	m_PathToRandomBT.resize(newSize);

	m_InventoryTaskBT.resize(newSize);
}

int DwarfManager::GetIndexForNewEntity()
{
	//Check if a place is free
	for (size_t i = 0; i < m_DwarfsEntities.size(); i++)
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

void DwarfManager::Update(float dt)
{
#ifdef DEBUG_RANDOM_PATH
	const auto config = m_Engine.GetConfig();
	const Vec2f screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);
#endif
	//Random path
	if (m_IndexPathToRandomBT != 0) {
		for (size_t i = 0; i < m_IndexPathToRandomBT; ++i)
		{
			const auto indexDwarf = m_PathToRandomBT[i];

			const auto transformPtr = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[indexDwarf]);
			m_NavigationGraphManager->AskForPath(&m_Paths[indexDwarf], transformPtr->Position,
				Vec2f(std::rand() % static_cast<int>(screenSize.x),
					std::rand() % static_cast<int>(screenSize.y
						)));

			AddDwarfToDraw(indexDwarf);
		}
		m_IndexPathToRandomBT = 0;
	}

	//Follow path
	if (m_IndexPathFollowingBT != 0) {
		for (size_t i = 0; i < m_IndexPathFollowingBT; ++i)
		{
			const auto indexDwarf = m_PathFollowingBT[i];

			const auto transformPtr = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[indexDwarf]);

			auto dir = m_Paths[indexDwarf][m_Paths[indexDwarf].size() - 1] - transformPtr->Position;

			transformPtr->Position += dir.Normalized() * m_SpeedDwarf * dt;

			AddDwarfToDraw(indexDwarf);

		}
		m_IndexPathFollowingBT = 0;
	}

}

void DwarfManager::FixedUpdate() {}

void DwarfManager::Draw()
{
	auto window = m_Engine.GetGraphics2dManager()->GetWindow();
#ifdef DEBUG_DRAW_PATH
	for (auto i = 0u; i < m_Paths.size(); i++)
	{
		const auto color = m_Colors[i % m_Colors.size()];

		sf::VertexArray lines{sf::LineStrip, m_Paths[i].size()};
		for (size_t j = 0u; j < m_Paths[i].size(); j++)
		{
			lines[j].position = m_Paths[i][j];

			lines[j].color = color;
		}

		window->draw(lines);
	}
#endif

	//Draw dwarf
	if (m_IndexToDraw != 0) {
		const auto halfTextureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y) / 2.0f;
		for (size_t i = 0; i < m_IndexToDraw; i++)
		{
			const auto indexDwarf = m_IndexesToDraw[i];
			const auto position = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[indexDwarf])->Position;
			const auto indexVertex = 4 * i;

			m_VertexArray[indexVertex].position = position - halfTextureSize;
			m_VertexArray[indexVertex + 1].position = position + sf::Vector2f(halfTextureSize.x, -halfTextureSize.y);
			m_VertexArray[indexVertex + 2].position = position + halfTextureSize;
			m_VertexArray[indexVertex + 3].position = position + sf::Vector2f(-halfTextureSize.x, halfTextureSize.y);
		}
		m_IndexToDraw = 0;
	}
	window->draw(m_VertexArray, m_Texture);
}
}
