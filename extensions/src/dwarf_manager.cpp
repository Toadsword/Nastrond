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
#include <extensions/behaviour_tree_nodes.h>
#include <extensions/behaviour_tree_core.h>

namespace sfge::ext
{
DwarfManager::DwarfManager(sfge::Engine& engine) :
	System(engine) {

}

void DwarfManager::Init() 
{
	//Get managers
	m_Transform2DManager = m_Engine.GetTransform2dManager();
	m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
	m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();
	m_NavigationGraphManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<NavigationGraphManager>(
		"NavigationGraphManager");

	//Associate behaviour tree
	auto* BT =  m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<sfge::ext::behaviour_tree::BehaviourTree>("BehaviourTree");
	BT->SetEntities(&m_DwarfsEntities);

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
	auto t1 = std::chrono::high_resolution_clock::now();
	//Create dwarfs
	for (auto i = 0u; i < m_DwarfToSpawn; i++) {
		const Vec2f pos(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y));

		SpawnDwarf(pos);
	}

	//Destroy dwarf
	for (auto i = 0u; i < 5; i++) {
		DestroyDwarfByIndex(i);
	}

	for (auto i = 0u; i < 20; i++) {
		const Vec2f pos(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y));

		SpawnDwarf(pos);
	}

	//Spawn dwarfs
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "Time: " << duration << "\n";
#endif
}

void DwarfManager::SpawnDwarf(const Vec2f pos) 
{

	auto* entityManager = m_Engine.GetEntityManager();
	Configuration* configuration = m_Engine.GetConfig();

	auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);

	if(newEntity == INVALID_ENTITY) {
		entityManager->ResizeEntityNmb(configuration->currentEntitiesNmb + m_ContainersExtender);
		newEntity = entityManager->CreateEntity(INVALID_ENTITY);
	}
	
	const int indexNewDwarf = GetIndexForNewEntity();

	//Update std::vectors
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
	m_VertexArray[4 * indexNewDwarf + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
	m_VertexArray[4 * indexNewDwarf + 2].position = transformPtr->Position + textureSize / 2.0f;
	m_VertexArray[4 * indexNewDwarf + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);
}

void DwarfManager::DestroyDwarfByEntity(Entity entity) {
	auto index = -1;
	for (auto i = 0; i < m_DwarfsEntities.size(); i++) {
		if(m_DwarfsEntities[i] == entity) {
			//Destroy entity
			auto* entityManager = m_Engine.GetEntityManager();
			entityManager->DestroyEntity(entity);

			index = i;
			break;
		}
	}

	if(index != -1) {
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

Entity DwarfManager::GetDwellingEntity(unsigned int index)
{
	return m_AssociatedDwelling[index];
}

void DwarfManager::AssignDwellingToDwarf(unsigned int index, Entity dwellingEntity) {
	m_AssociatedDwelling[index] = dwellingEntity;
}

bool DwarfManager::IsDwarfAtDestination(unsigned int index) {
	if(!m_Paths[index].empty()) {
		const auto position = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[index])->Position;

		auto dir = m_Paths[index][0] - position;
		if( dir.GetMagnitude() < m_StoppingDistance) {
			m_Paths[index].clear();

			return true;
		}else {
			return false;
		}
	}
	return true;
}

bool DwarfManager::HasPath(unsigned index) {
	return !m_Paths[index].empty();
}

void DwarfManager::BTAddPathToDwelling(unsigned int index) {
	m_BT_pathDwarfToDwelling.push_back(index);
}

void DwarfManager::BtFindRandomPath(unsigned int index) {
	m_BT_pathDwarfToRandom.push_back(index);
}

void DwarfManager::BTAddPathFollower(unsigned int index) {
	m_BT_followingPath.push_back(index);
}

void DwarfManager::ResizeContainers() {
	const size_t newSize = m_DwarfsEntities.size() + m_ContainersExtender;

	m_DwarfsEntities.resize(newSize, INVALID_ENTITY);
	m_Paths.resize(newSize);
	m_States.resize(newSize);

	//Associate behaviour tree
	auto* BT = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<behaviour_tree::BehaviourTree>("BehaviourTree");
	BT->SetEntities(&m_DwarfsEntities);
	m_AssociatedDwelling.resize(newSize, INVALID_ENTITY);
	m_AssociatedWorkingPlace.resize(newSize, INVALID_ENTITY);
	m_VertexArray.resize(m_VertexArray.getVertexCount() * 4 + 4 * m_ContainersExtender);
}

int DwarfManager::GetIndexForNewEntity() {
	//Check if a place is free
	for(size_t i = 0; i < m_DwarfsEntities.size(); i++) {
		if(m_DwarfsEntities[i] == INVALID_ENTITY) {
			return i;
		}
	}

	//Otherwise resize all std::vectors
	const int index = m_DwarfsEntities.size();
	ResizeContainers();
	return index;
}

void DwarfManager::Update(float dt) {
#ifdef DEBUG_RANDOM_PATH
	const auto config = m_Engine.GetConfig();
	const Vec2f screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);
#endif
	//Random path
	for (int i = 0; i < m_BT_pathDwarfToRandom.size(); i++) {
		const auto transformPtr = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[m_BT_pathDwarfToRandom[i]]);
		m_NavigationGraphManager->AskForPath(&m_Paths[m_BT_pathDwarfToRandom[i]], transformPtr->Position,
			Vec2f(std::rand() % static_cast<int>(screenSize.x),
				std::rand() % static_cast<int>(screenSize.y
					)));
	}
	m_BT_pathDwarfToRandom.clear();

	//Follow path
	for (int i = 0; i < m_BT_followingPath.size(); i++) {
		const auto transformPtr = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[m_BT_followingPath[i]]);

		auto dir = m_Paths[m_BT_followingPath[i]][m_Paths[m_BT_followingPath[i]].size() - 1] - transformPtr->Position;

		if (dir.GetMagnitude() < m_StoppingDistance && m_Paths[m_BT_followingPath[i]].size() > 1) {
			m_Paths[m_BT_followingPath[i]].pop_back();
		}
		else {
			transformPtr->Position += dir.Normalized() * m_SpeedDwarf * dt;

			sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

			m_VertexArray[4 * m_BT_followingPath[i]].position = transformPtr->Position - textureSize / 2.0f;
			m_VertexArray[4 * m_BT_followingPath[i] + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
			m_VertexArray[4 * m_BT_followingPath[i] + 2].position = transformPtr->Position + textureSize / 2.0f;
			m_VertexArray[4 * m_BT_followingPath[i] + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);
		}
	}
	m_BT_followingPath.clear();
}

void DwarfManager::FixedUpdate() {
}

void DwarfManager::Draw() {
	auto window = m_Engine.GetGraphics2dManager()->GetWindow();
#ifdef DEBUG_DRAW_PATH
	for (auto i = 0u; i < m_Paths.size(); i++) {
		const auto color = m_Colors[i % m_Colors.size()];

		sf::VertexArray lines{sf::LineStrip, m_Paths[i].size()};
		for (size_t j = 0u; j < m_Paths[i].size(); j++) {
			lines[j].position = m_Paths[i][j];

			lines[j].color = color;
		}

		window->draw(lines);
	}
#endif

	window->draw(m_VertexArray, m_Texture);
}
}
