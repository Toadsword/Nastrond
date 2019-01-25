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

namespace sfge::ext
{
DwarfManager::DwarfManager(sfge::Engine& engine) :
	System(engine) {

}

void DwarfManager::Init() {
	//Get managers
	m_Transform2DManager = m_Engine.GetTransform2dManager();
	m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
	m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();
	m_NavigationGraphManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<NavigationGraphManager>(
		"NavigationGraphManager");

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
	for (auto i = 0u; i < m_DwarfToSpawn; i++) {
		const Vec2f pos(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y));

		SpawnDwarf(pos);
	}
#endif
}

void DwarfManager::SpawnDwarf(const Vec2f pos) {

	auto* entityManager = m_Engine.GetEntityManager();
	Configuration* configuration = m_Engine.GetConfig();
	//TODO ajouter check pour resizer uniquement s'il le faut
	entityManager->ResizeEntityNmb(configuration->currentEntitiesNmb + 1);

	const auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);

	//TODO ajouter un check automatique pour mettre le nain à la première place disponible dans les tableaux

	//Check if vectors are big enough, otherwise resize them
	if (m_DwarfsEntities.size() < m_IndexNewDwarf + 1)
		ResizeContainers(m_DwarfsEntities.size() + m_ContainersExtender);

	//Update std::vectors
	m_DwarfsEntities[m_IndexNewDwarf] = newEntity;
	m_States[m_IndexNewDwarf] = State::IDLE;
	m_Paths[m_IndexNewDwarf] = std::vector<Vec2f>();
	m_AssociatedDwelling[m_IndexNewDwarf] = INVALID_ENTITY;
	m_AssociatedWorkingPlace[m_IndexNewDwarf] = INVALID_ENTITY;

	m_VertexArray.resize(m_IndexNewDwarf * 4 + 4);

	sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

	m_VertexArray[4 * m_IndexNewDwarf].texCoords = sf::Vector2f(0, 0);
	m_VertexArray[4 * m_IndexNewDwarf + 1].texCoords = sf::Vector2f(textureSize.x, 0);
	m_VertexArray[4 * m_IndexNewDwarf + 2].texCoords = textureSize;
	m_VertexArray[4 * m_IndexNewDwarf + 3].texCoords = sf::Vector2f(0, textureSize.y);

	//Add transform
	auto* transformPtr = m_Transform2DManager->AddComponent(newEntity);
	transformPtr->Position = pos;

	m_VertexArray[4 * m_IndexNewDwarf].position = transformPtr->Position - textureSize / 2.0f;
	m_VertexArray[4 * m_IndexNewDwarf + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
	m_VertexArray[4 * m_IndexNewDwarf + 2].position = transformPtr->Position + textureSize / 2.0f;
	m_VertexArray[4 * m_IndexNewDwarf + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);

	m_IndexNewDwarf++;
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
		m_States[index] = State::IDLE;
		m_AssociatedDwelling[index] = INVALID_ENTITY;
		m_AssociatedWorkingPlace[index] = INVALID_ENTITY;

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

void DwarfManager::DestroyDwarfByIndex(unsigned index) {
	//Destroy entity
	auto* entityManager = m_Engine.GetEntityManager();
	entityManager->DestroyEntity(m_DwarfsEntities[index]);

	//Clean all values
	m_Paths[index] = std::vector<Vec2f>(0);
	m_States[index] = State::IDLE;
	m_AssociatedDwelling[index] = INVALID_ENTITY;
	m_AssociatedWorkingPlace[index] = INVALID_ENTITY;

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

void DwarfManager::ResizeContainers(const size_t newSize) {
	m_DwarfsEntities.resize(newSize);
	m_Paths.resize(newSize);
	m_States.resize(newSize);
	m_AssociatedDwelling.resize(newSize);
	m_AssociatedWorkingPlace.resize(newSize);
}

void DwarfManager::Update(float dt) {
#ifdef DEBUG_RANDOM_PATH
	const auto config = m_Engine.GetConfig();
	const Vec2f screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);
#endif
	
	for (auto i = 0u; i < m_IndexNewDwarf; i++) {
		switch (m_States[i]) { 
		case IDLE: {
#ifdef DEBUG_RANDOM_PATH
			const auto transformPtr = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[i]);
			m_NavigationGraphManager->AskForPath(&m_Paths[i], transformPtr->Position,
				Vec2f(std::rand() % static_cast<int>(screenSize.x),
					std::rand() % static_cast<int>(screenSize.y
						)));
			m_States[i] = State::WAITING_NEW_PATH;
#endif
			break;
		}

			case WALKING: 
			break;

			case WAITING_NEW_PATH: 
				if (!m_Paths[i].empty()) {
					m_States[i] = State::WALKING;
				}
			break;
			default: ;
		}
	}
}

void DwarfManager::FixedUpdate() {
	for (auto i = 0u; i < m_IndexNewDwarf; i++) {
		switch (m_States[i]) { 
			case IDLE: break;
			case WALKING: {
				auto transformPtr = m_Engine.GetTransform2dManager()->GetComponentPtr(m_DwarfsEntities[i]);

				auto dir = m_Paths[i][m_Paths[i].size() - 1] - transformPtr->Position;

				if (dir.GetMagnitude() < m_StoppingDistance) {
					m_Paths[i].pop_back();

					if (m_Paths[i].empty()) {
						m_States[i] = State::IDLE;
					}
				}
				else {
					//TODO ajouter un manager pour les velocités. L'idées est de créer un système qui ne comporte que ça comme données et fait un traitement uniquement dessus. Il faut rajouter des fonctions comme AddComponent() en lui passant directement l'entité
					transformPtr->Position += dir.Normalized() * m_SpeedDwarf;

					sf::Vector2f textureSize = sf::Vector2f(m_Texture->getSize().x, m_Texture->getSize().y);

					m_VertexArray[4 * i].position = transformPtr->Position - textureSize / 2.0f;
					m_VertexArray[4 * i + 1].position = transformPtr->Position + sf::Vector2f(textureSize.x / 2.0f, -textureSize.y / 2.0f);
					m_VertexArray[4 * i + 2].position = transformPtr->Position + textureSize / 2.0f;
					m_VertexArray[4 * i + 3].position = transformPtr->Position + sf::Vector2f(-textureSize.x / 2.0f, textureSize.y / 2.0f);
				}
				break;
			}
			case WAITING_NEW_PATH: break;
			default: ;
		}
	}
}

void DwarfManager::Draw() {
	auto window = m_Engine.GetGraphics2dManager()->GetWindow();
#ifdef DEBUG_DRAW_PATH
	for (auto i = 0u; i < m_IndexNewDwarf; i++) {
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
