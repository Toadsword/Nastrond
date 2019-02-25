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

		Log::GetInstance()->Msg("Dwelling Manager initialized");

	}

	void DwellingManager::Update(float dt)
	{
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
		auto* entityManager = m_Engine.GetEntityManager();

		auto newEntity = entityManager->CreateEntity(INVALID_ENTITY);

		if(newEntity == INVALID_ENTITY)
		{
			entityManager->ResizeEntityNmb(m_Configuration->currentEntitiesNmb + 1);
			newEntity = entityManager->CreateEntity(INVALID_ENTITY);
		}

		//add transform
		auto* transformPtr = m_Transform2DManager->AddComponent(newEntity);
		transformPtr->Position = Vec2f(pos.x, pos.y);

		if (CheckEmptySlot(newEntity))
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

		SetupTexture(newEntity);
	}

	bool DwellingManager::DestroyBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_EntityIndex[i] = INVALID_ENTITY;
				EntityManager* entityManager = m_Engine.GetEntityManager();
				entityManager->DestroyEntity(entity);
				return true;
			}
		}
		return false;
	}

	bool DwellingManager::AddDwarfToBuilding(Entity dwellingEntity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == dwellingEntity)
			{
				if (m_DwarfSlots[i].dwarfAttributed < m_DwarfSlots[i].maxDwarfCapacity)
				{
					m_DwarfSlots[i].dwarfAttributed++;
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}

	bool DwellingManager::RemoveDwarfToBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_DwarfSlots[i].dwarfAttributed--;
				return true;
			}
		}
		return false;
	}

	Entity DwellingManager::GetFreeSlotInBuilding()
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
				if (m_DwarfSlots[i].dwarfIn > m_DwarfSlots[i].maxDwarfCapacity)
					m_DwarfSlots[i].dwarfIn = m_DwarfSlots[i].maxDwarfCapacity;
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
				if (m_DwarfSlots[i].dwarfIn < 0)
					m_DwarfSlots[i].dwarfIn = 0;
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
			else if(m_DwarfSlots[i].dwarfIn <= 0)
			{
				continue;
			}

			m_ProgressionCoolDown[i] += m_DwarfSlots[i].dwarfIn;

			if(m_ProgressionCoolDown[i] >= m_CoolDown)
			{
				m_ProgressionCoolDown[i] = 0;
				m_ResourcesInventories[i]--;

				if(m_ResourcesInventories[i] < 0)
				{
					DecreaseHappiness();
				}

				if(m_ResourcesInventories[i] <= m_MaxCapacity - (m_ReservedImportStackNumber[i] * GetStackSizeByResourceType(m_ResourceTypeNeeded) + GetStackSizeByResourceType(m_ResourceTypeNeeded)))
				{
					m_ReservedImportStackNumber[i]++;
					m_BuildingManager->RegistrationBuildingToBeFill(m_EntityIndex[i], BuildingType::DWELLING, m_ResourceTypeNeeded, 0);
				}
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

		m_ResourcesInventories.emplace_back(0);
		m_ReservedImportStackNumber.emplace_back(0);

		m_ProgressionCoolDown.emplace_back(0);
	}

	bool DwellingManager::CheckEmptySlot(Entity newEntity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				m_EntityIndex[i] = newEntity;
				m_DwarfSlots[i] = DwarfSlots();

				m_ResourcesInventories[i] = 0;
				m_ProgressionCoolDown[i] = 0;
				m_ReservedImportStackNumber[i] = 0;

				SetupTexture(newEntity);

				return true;
			}
		}
		return false;
	}

	void DwellingManager::SetupTexture(const unsigned int dwellingIndex)
	{
		//Sprite Component part
		Sprite* sprite = m_SpriteManager->AddComponent(dwellingIndex);
		sprite->SetTexture(m_Texture);
	}

	void DwellingManager::DecreaseHappiness()
	{
		// TODO : Setup this function to decrease Happiness
	}
}
