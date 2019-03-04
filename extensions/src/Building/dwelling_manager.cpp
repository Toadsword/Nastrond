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

#include <extensions/Building/dwelling_manager.h>
#include <extensions/Building/building_manager.h>


namespace sfge::ext
{
	DwellingManager::DwellingManager(Engine & engine) : System(engine) {}

	void DwellingManager::Init()
	{
		m_EntityManager = m_Engine.GetEntityManager();
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
		m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();
		m_Configuration = m_Engine.GetConfig();
		m_BuildingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<BuildingManager>(
			"BuildingManager");

		m_Window = m_Engine.GetGraphics2dManager()->GetWindow();

		//Load texture
		m_TexturePath = "data/sprites/dwelling.png";
		m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
		m_Texture = m_TextureManager->GetTexture(m_TextureId);

		m_Init = true;

		Log::GetInstance()->Msg("Dwelling Manager initialized");
	}

	void DwellingManager::Update(float dt)
	{
		if (!m_Init)
		{
			Init();
		}
	}

	void DwellingManager::FixedUpdate()
	{
		Consume();
	}

	void DwellingManager::Draw()
	{
	}

	void DwellingManager::SpawnBuilding(Vec2f pos)
	{
		Entity newEntity = m_EntityManager->CreateEntity(INVALID_ENTITY);

		if(newEntity == INVALID_ENTITY)
		{
			m_EntityManager->ResizeEntityNmb(m_Configuration->currentEntitiesNmb + CONTAINER_RESERVATION);
			newEntity = m_EntityManager->CreateEntity(INVALID_ENTITY);
		}

		//add transform
		Transform2d* transformPtr = m_Transform2DManager->AddComponent(newEntity);
		transformPtr->Position = Vec2f(pos.x, pos.y);

		SetupTexture(newEntity);

		editor::EntityInfo& entityInfo = m_EntityManager->GetEntityInfo(newEntity);
		entityInfo.name = "Dwelling " + std::to_string(m_BuildingIndexCount);

		if (CheckFreeSlot(newEntity))
		{
			return;
		}

		m_BuildingIndexCount++;


		if (m_BuildingIndexCount >= CONTAINER_RESERVATION * m_NmbReservation)
		{
			ReserveContainer(m_BuildingIndexCount + CONTAINER_RESERVATION);
			m_NmbReservation++;
		}

		AttributeContainer();

		const size_t newDwelling = m_BuildingIndexCount - 1;

		m_EntityIndex[newDwelling] = newEntity;
	}

	void DwellingManager::DestroyBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_EntityIndex[i] = INVALID_ENTITY;
				m_EntityManager->DestroyEntity(entity);
				return;
			}
		}
	}

	void DwellingManager::AttributeDwarfToBuilding(Entity dwellingEntity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == dwellingEntity)
			{
				m_DwarfSlots[i].dwarfAttributed++;
				return;
			}
		}
	}

	void DwellingManager::DeallocateDwarfToBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_DwarfSlots[i].dwarfAttributed--;
				return;
			}
		}
	}

	Entity DwellingManager::GetBuildingWithFreePlace()
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_DwarfSlots[i].dwarfAttributed < m_DwarfSlots[i].maxDwarfCapacity)
			{
				return m_EntityIndex[i];
			}
		}
		return INVALID_ENTITY;
	}

	void DwellingManager::DwarfEnterBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_DwarfSlots[i].dwarfIn++;
				return;
			}
		}
	}

	void DwellingManager::DwarfExitBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_DwarfSlots[i].dwarfIn--;
				return;
			}
		}
	}

	void DwellingManager::DwarfPutsResources(Entity entity)
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_ResourcesInventories[i] += GetStackSizeByResourceType(m_ResourceTypeNeeded);
				m_ReservedImportStackNumber[i]--;
				return;
			}
		}
	}

	void DwellingManager::Consume()
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				continue;
			}
			
			if (m_ResourcesInventories[i] <= 0)
			{
				DecreaseHappiness();
				continue;
			}
			
			if(m_DwarfSlots[i].dwarfIn <= 0)
			{
				continue;
			}

			m_ProgressionCoolDown[i] += m_DwarfSlots[i].dwarfIn;

			if (m_ProgressionCoolDown[i] < m_CoolDownGoal)
			{
				continue;
			}

			m_ProgressionCoolDown[i] = 0;
			m_ResourcesInventories[i]--;

			const unsigned short stackSizeNeeded = GetStackSizeByResourceType(m_ResourceTypeNeeded);

			if(m_ResourcesInventories[i] <= m_MaxCapacity - (m_ReservedImportStackNumber[i] * m_ResourceTypeNeeded + stackSizeNeeded))
			{
				m_ReservedImportStackNumber[i]++;
				m_BuildingManager->RegistrationBuildingToBeFill(m_EntityIndex[i], BuildingType::DWELLING, m_ResourceTypeNeeded);
			}
		}
	}

	void DwellingManager::ReserveContainer(const size_t newSize)
	{
		m_EntityIndex.reserve(newSize);
		m_DwarfSlots.reserve(newSize);

		m_ResourcesInventories.reserve(newSize);
		m_ReservedImportStackNumber.reserve(newSize);

		m_ProgressionCoolDown.reserve(newSize);
	}

	void DwellingManager::AttributeContainer()
	{
		m_EntityIndex.emplace_back(INVALID_ENTITY);
		m_DwarfSlots.emplace_back(DwarfSlots());

		m_ResourcesInventories.emplace_back(m_MaxCapacity);
		m_ReservedImportStackNumber.emplace_back(0);

		m_ProgressionCoolDown.emplace_back(0);
	}

	bool DwellingManager::CheckFreeSlot(Entity newEntity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] != INVALID_ENTITY)
			{
				continue;
			}

			m_EntityIndex[i] = newEntity;


			m_DwarfSlots[i] = DwarfSlots();

			m_ResourcesInventories[i] = 0;
			m_ProgressionCoolDown[i] = 0;
			m_ReservedImportStackNumber[i] = 0;

			return true;
		}
		return false;
	}

	void DwellingManager::SetupTexture(const Entity entity)
	{
		//Sprite Component part
		Sprite* sprite = m_SpriteManager->AddComponent(entity);
		sprite->SetTexture(m_Texture);

		auto& spriteInfo = m_SpriteManager->GetComponentInfo(entity);
		spriteInfo.name = "Sprite";
		spriteInfo.sprite = sprite;
		spriteInfo.textureId = m_TextureId;
		spriteInfo.texturePath = m_TexturePath;
	}

	void DwellingManager::DecreaseHappiness()
	{
		// TODO : Setup this function to decrease Happiness
	}
}
