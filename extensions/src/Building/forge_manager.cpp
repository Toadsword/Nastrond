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

#include <extensions/Building/forge_manager.h>
#include <extensions/Building/building_manager.h>


namespace sfge::ext
{
	ForgeManager::ForgeManager(Engine& engine) : System(engine) {}

	void ForgeManager::Init()
	{
		m_EntityManager = m_Engine.GetEntityManager();

		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
		m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();
		m_Configuration = m_Engine.GetConfig();
		m_BuildingManager = m_Engine.GetPythonEngine()->GetPySystemManager().GetPySystem<BuildingManager>(
			"BuildingManager");

		m_Window = m_Engine.GetGraphics2dManager()->GetWindow();


		//Load Texture
		m_TexturePath = "data/sprites/forge.png";
		m_TextureId = m_TextureManager->LoadTexture(m_TexturePath);
		m_Texture = m_TextureManager->GetTexture(m_TextureId);

		m_Init = true;

		Log::GetInstance()->Msg("Forge Manager initialized");
	}

	void ForgeManager::Update(float dt)
	{
		if (!m_Init)
		{
			Init();
		}
	}

	void ForgeManager::FixedUpdate()
	{
		ProduceTools();
	}

	void ForgeManager::Draw()
	{
	}

	void ForgeManager::SpawnBuilding(Vec2f pos)
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
		entityInfo.name = "Forge " + std::to_string(m_BuildingIndexCount);

		if(CheckFreeSlot(newEntity))
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

		const size_t newForge = m_BuildingIndexCount - 1;

		m_EntityIndex[newForge] = newEntity;

	}

	void ForgeManager::DestroyBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_EntityIndex[i] = INVALID_ENTITY;
				EntityManager* entityManager = m_Engine.GetEntityManager();
				entityManager->DestroyEntity(entity);
				return;
			}
		}
	}

	void ForgeManager::AttributeDwarfToBuilding(Entity entity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_DwarfSlots[i].dwarfAttributed++;
				return;
			}
		}
	}

	void ForgeManager::DeallocateDwarfToBuilding(Entity entity)
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

	void ForgeManager::DwarfEnterBuilding(Entity entity)
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

	void ForgeManager::DwarfExitBuilding(Entity entity)
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

	Entity ForgeManager::GetBuildingWithFreePlace()
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

	void ForgeManager::DwarfTakesResources(Entity entity)
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_ResourcesInventoriesGiver[i] -= GetStackSizeByResourceType(m_ResourceTypeProduced);
				m_ReservedExportStackNumber[i]--;
			}
		}
	}

	void ForgeManager::DwarfPutsResources(Entity entity)
	{
		for (unsigned int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == entity)
			{
				m_ResourcesInventoriesReceiver[i] += GetStackSizeByResourceType(m_ResourceTypeNeeded);
				m_ReservedImportStackNumber[i]--;
			}
		}
	}

	void ForgeManager::ReserveContainer(const size_t newSize)
	{
		m_EntityIndex.reserve(newSize);

		m_DwarfSlots.reserve(newSize);

		m_ResourcesInventoriesGiver.reserve(newSize);
		m_ResourcesInventoriesReceiver.reserve(newSize);

		m_ProgressionConsumption.reserve(newSize);
		m_ProgressionCoolDown.reserve(newSize);

		m_ReservedExportStackNumber.reserve(newSize);
		m_ReservedImportStackNumber.reserve(newSize);
	}

	void ForgeManager::ProduceTools()
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY || m_ResourcesInventoriesReceiver[i] <= 0 || m_ResourcesInventoriesGiver[i] >= m_MaxCapacityGiver)
			{
				continue;
			}

			if ((m_ProgressionCoolDown[i] += m_DwarfSlots[i].dwarfIn) < m_CoolDownGoal)
			{
				continue;
			}

			m_ProgressionCoolDown[i] = 0;
			m_ResourcesInventoriesReceiver[i]--;

			const unsigned short stackSizeNeeded = GetStackSizeByResourceType(m_ResourceTypeNeeded);

			if(m_ResourcesInventoriesReceiver[i] <= m_MaxCapacityReceiver - (m_ReservedImportStackNumber[i] * stackSizeNeeded + stackSizeNeeded))
			{
				m_BuildingManager->RegistrationBuildingToBeFill(m_EntityIndex[i], BuildingType::FORGE, m_ResourceTypeNeeded);
			}

			m_ProgressionConsumption[i]++;

			if (m_ProgressionConsumption[i] < m_ConsumptionGoal)
			{
				continue;
			}

			m_ProgressionConsumption[i] = 0;
			m_ResourcesInventoriesGiver[i]++;

			const unsigned short stackSizeProduce = GetStackSizeByResourceType(m_ResourceTypeProduced);

			if(m_ResourcesInventoriesGiver[i] >= m_ReservedExportStackNumber[i] * stackSizeProduce + stackSizeProduce)
			{
				m_BuildingManager->RegistrationBuildingToBeEmptied(m_EntityIndex[i], BuildingType::FORGE, m_ResourceTypeProduced);
			}
		}
	}

	bool ForgeManager::CheckFreeSlot(Entity newEntity)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if (m_EntityIndex[i] == INVALID_ENTITY)
			{
				m_EntityIndex[i] = newEntity;

				m_DwarfSlots[i] = DwarfSlots();

				m_ResourcesInventoriesReceiver[i] = 0;
				m_ResourcesInventoriesGiver[i] = 0;

				m_ProgressionCoolDown[i] = 0;
				m_ProgressionConsumption[i] = 0;

				m_ReservedExportStackNumber[i] = 0;
				m_ReservedImportStackNumber[i] = 0;
				return true;
			}
		}
		return false;
	}

	void ForgeManager::AttributeContainer()
	{
		m_EntityIndex.emplace_back(INVALID_ENTITY);

		m_DwarfSlots.emplace_back(DwarfSlots());

		m_ResourcesInventoriesGiver.emplace_back(0);
		m_ResourcesInventoriesReceiver.emplace_back(0);

		m_ProgressionConsumption.emplace_back(0);
		m_ProgressionCoolDown.emplace_back(0);

		m_ReservedExportStackNumber.emplace_back(0);
		m_ReservedImportStackNumber.emplace_back(0);
	}

	void ForgeManager::SetupTexture(const Entity entity)
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
}