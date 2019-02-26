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

#include <extensions/Building/road_manager.h>

namespace sfge::ext
{
	RoadManager::RoadManager(Engine& engine) : System(engine) {}

	void RoadManager::Init()
	{
		m_Transform2DManager = m_Engine.GetTransform2dManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
		m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();
		m_EntityManager = m_Engine.GetEntityManager();
		m_Configuration = m_Engine.GetConfig();

		//Load Texture
		m_CrossTexturePath = "data/sprites/SP_path_4ways.png";
		m_CrossTextureId = m_TextureManager->LoadTexture(m_CrossTexturePath);
		m_CrossTexture = m_TextureManager->GetTexture(m_CrossTextureId);

		//Load Texture
		m_ThreeWayTexturePath = "data/sprites/SP_path_3ways.png";
		m_ThreeWayTextureId = m_TextureManager->LoadTexture(m_ThreeWayTexturePath);
		m_ThreeWayTexture = m_TextureManager->GetTexture(m_ThreeWayTextureId);

		//Load Texture
		m_OneWayTexturePath = "data/sprites/SP_path_1way.png";
		m_OneWayTextureId = m_TextureManager->LoadTexture(m_OneWayTexturePath);
		m_OneWayTexture = m_TextureManager->GetTexture(m_OneWayTextureId);

		//Load Texture
		m_EndTexturePath = "data/sprites/SP_path_end.png";
		m_EndTextureId = m_TextureManager->LoadTexture(m_EndTexturePath);
		m_EndTexture = m_TextureManager->GetTexture(m_EndTextureId);

		//Load Texture
		m_TurnNSTexturePath = "data/sprites/SP_path_turnNS.png";
		m_TurnNSTextureId = m_TextureManager->LoadTexture(m_TurnNSTexturePath);
		m_TurnNSTexture = m_TextureManager->GetTexture(m_TurnNSTextureId);

		//Load Texture
		m_TurnWETexturePath = "data/sprites/SP_path_turnWE.png";
		m_TurnWETextureId = m_TextureManager->LoadTexture(m_TurnWETexturePath);
		m_TurnWETexture = m_TextureManager->GetTexture(m_TurnWETextureId);

		//Load Texture
		m_SoloTexturePath = "data/sprites/SP_path_solo.png";
		m_SoloTextureId = m_TextureManager->LoadTexture(m_SoloTexturePath);
		m_SoloTexture = m_TextureManager->GetTexture(m_SoloTextureId);

		//Load Texture
		m_GroundTexturePath = "data/sprites/SP_ground_clear.png";
		m_GroundTextureId = m_TextureManager->LoadTexture(m_GroundTexturePath);
		m_GroundTexture = m_TextureManager->GetTexture(m_GroundTextureId);

		Log::GetInstance()->Msg("Road Manager initialized");
	}

	void RoadManager::Update(float dt)
	{
	}

	void RoadManager::FixedUpdate()
	{
	}

	void RoadManager::Draw()
	{
	}

	void RoadManager::SpawnRoad(Vec2f position, std::vector<bool> roadAround)
	{
		m_EntityManager = m_Engine.GetEntityManager();


		auto newEntity = m_EntityManager->CreateEntity(INVALID_ENTITY);

		if (newEntity == INVALID_ENTITY)
		{
			m_EntityManager->ResizeEntityNmb(m_Configuration->currentEntitiesNmb + CONTAINER_RESERVATION);
			newEntity = m_EntityManager->CreateEntity(INVALID_ENTITY);
		}

		//add transform
		auto transformPtr = m_Transform2DManager->AddComponent(newEntity);
		transformPtr->Position = Vec2f(position.x, position.y);

		if (CheckEmptySlot(newEntity, transformPtr, roadAround))
		{
			return;
		}

		m_BuildingIndexCount++;


		if (m_BuildingIndexCount >= CONTAINER_RESERVATION * m_NmbReservation)
		{
			m_EntityIndex.reserve(m_BuildingIndexCount + CONTAINER_RESERVATION);
			m_NmbReservation++;
		}

		m_EntityIndex.emplace_back(newEntity);

		const size_t newWarehouse = m_BuildingIndexCount - 1;

		m_EntityIndex[newWarehouse] = newEntity;

		RoadTextureSelection(newEntity, transformPtr, roadAround);
	}

	void RoadManager::SpawnRoad(const std::vector<int> tilesTypeVector, const int Length, const Vec2f positionFirstTile, const Vec2f size, const int targetType, const int groundType)
	{
		int count = 0;
		int row = 0;


		for (int i = 0; i < tilesTypeVector.size(); i++)
		{
			std::vector<bool> roadAround;

			roadAround.resize(5, false);

			if (tilesTypeVector[i] != targetType)
			{
				roadAround[0] = false;
				if (row % 2 == 0)
					SpawnRoad(Vec2f(size.x * count, row * size.y / 2), roadAround);
				else
					SpawnRoad(Vec2f(size.x * count - size.x / 2, row * size.y / 2), roadAround);

				count++;
				if (count >= Length)
				{
					count = 0;
					row++;
				}
				continue;
			}

			roadAround[0] = true;
			if(row % 2 == 0)
			{
				if(i - Length >= 0)
					roadAround[1] = tilesTypeVector[i - Length] == targetType;
				if (i - Length + 1 >= 0 && count < Length - 1)
					roadAround[2] = tilesTypeVector[i - Length + 1] == targetType;
				if (i + Length < tilesTypeVector.size())
					roadAround[3] = tilesTypeVector[i + Length] == targetType;
				if (i + Length + 1 < tilesTypeVector.size() && count < Length - 1)
					roadAround[4] = tilesTypeVector[i + Length + 1] == targetType;

				SpawnRoad(Vec2f(size.x * count, row * size.y / 2), roadAround);
			}
			else
			{
				if (i - Length - 1 >= 0 && count > 0)
					roadAround[1] = tilesTypeVector[i - Length - 1] == targetType;
				if (i - Length >= 0)
					roadAround[2] = tilesTypeVector[i - Length] == targetType;
				if (i + Length - 1 < tilesTypeVector.size() && count > 0)
					roadAround[3] = tilesTypeVector[i + Length - 1] == targetType;
				if (i + Length < tilesTypeVector.size())
					roadAround[4] = tilesTypeVector[i + Length] == targetType;

				SpawnRoad(Vec2f(size.x * count - size.x / 2, row * size.y / 2), roadAround);
			}

			count++;

			if(count >= Length)
			{
				count = 0;
				row++;
			}
		}
	}


	void RoadManager::GetNeighbors()
	{

	}

	bool RoadManager::CheckEmptySlot(const Entity entity, Transform2d* transform2d, std::vector<bool> roadAround)
	{
		for (int i = 0; i < m_BuildingIndexCount; i++)
		{
			if(m_EntityIndex[i] == INVALID_ENTITY)
			{
				m_EntityIndex[i] = entity;
				RoadTextureSelection(entity, transform2d, roadAround);
				return true;
			}
		}
		return false;
	}

	void RoadManager::RoadTextureSelection(const Entity entity, Transform2d* transform2d, std::vector<bool> roadAround)
	{
		if(!roadAround[0])
		{
			SetupTexture(entity, m_GroundTexture, m_GroundTextureId, m_GroundTexturePath);
		}

		else if(!roadAround[1] && !roadAround[2] && !roadAround[3] && !roadAround[4])
		{
			SetupTexture(entity, m_SoloTexture, m_SoloTextureId, m_SoloTexturePath);
		}

		else if(roadAround[1] && !roadAround[2] && !roadAround[3] && !roadAround[4])
		{
			transform2d->Scale = Vec2f(1, -1);
			SetupTexture(entity, m_EndTexture, m_EndTextureId, m_EndTexturePath);
		}
		else if (!roadAround[1] && roadAround[2] && !roadAround[3] && !roadAround[4])
		{
			transform2d->Scale = Vec2f(-1, -1);
			SetupTexture(entity, m_EndTexture, m_EndTextureId, m_EndTexturePath);
		}
		else if (!roadAround[1] && !roadAround[2] && roadAround[3] && !roadAround[4])
		{
			SetupTexture(entity, m_EndTexture, m_EndTextureId, m_EndTexturePath);
		}
		else if (!roadAround[1] && !roadAround[2] && !roadAround[3] && roadAround[4])
		{
			transform2d->Scale = Vec2f(-1, 1);
			SetupTexture(entity, m_EndTexture, m_EndTextureId, m_EndTexturePath);
		}

		else if (roadAround[1] && !roadAround[2] && roadAround[3] && !roadAround[4])
		{
			SetupTexture(entity, m_TurnWETexture, m_TurnWETextureId, m_TurnWETexturePath);
		}
		else if (!roadAround[1] && roadAround[2] && !roadAround[3] && roadAround[4])
		{
			transform2d->Scale = Vec2f(-1, 1);
			SetupTexture(entity, m_TurnWETexture, m_TurnWETextureId, m_TurnWETexturePath);
		}

		else if (roadAround[1] && roadAround[2] && !roadAround[3] && !roadAround[4])
		{
			SetupTexture(entity, m_TurnNSTexture, m_TurnNSTextureId, m_TurnNSTexturePath);
		}
		else if (!roadAround[1] && !roadAround[2] && roadAround[3] && roadAround[4])
		{
			transform2d->Scale = Vec2f(1, -1);
			SetupTexture(entity, m_TurnNSTexture, m_TurnNSTextureId, m_TurnNSTexturePath);
		}

		else if (!roadAround[1] && roadAround[2] && roadAround[3] && roadAround[4])
		{
			transform2d->Scale = Vec2f(-1, 1);
			SetupTexture(entity, m_ThreeWayTexture, m_ThreeWayTextureId, m_ThreeWayTexturePath);
		}
		else if (roadAround[1] && !roadAround[2] && roadAround[3] && roadAround[4])
		{
			SetupTexture(entity, m_ThreeWayTexture, m_ThreeWayTextureId, m_ThreeWayTexturePath);
		}
		else if (roadAround[1] && roadAround[2] && !roadAround[3] && roadAround[4])
		{
			transform2d->Scale = Vec2f(-1, -1);
			SetupTexture(entity, m_ThreeWayTexture, m_ThreeWayTextureId, m_ThreeWayTexturePath);
		}
		else if (roadAround[1] && roadAround[2] && roadAround[3] && !roadAround[4])
		{
			transform2d->Scale = Vec2f(1, -1);
			SetupTexture(entity, m_ThreeWayTexture, m_ThreeWayTextureId, m_ThreeWayTexturePath);
		}

		else if (roadAround[1] && !roadAround[2] && !roadAround[3] && roadAround[4])
		{
			transform2d->Scale = Vec2f(-1, 1);
			SetupTexture(entity, m_OneWayTexture, m_OneWayTextureId, m_OneWayTexturePath);
		}
		else if (!roadAround[1] && roadAround[2] && roadAround[3] && !roadAround[4])
		{
			SetupTexture(entity, m_OneWayTexture, m_OneWayTextureId, m_OneWayTexturePath);
		}

		else if (roadAround[1] && roadAround[2] && roadAround[3] && roadAround[4])
		{
			SetupTexture(entity, m_CrossTexture, m_CrossTextureId, m_CrossTexturePath);
		}

	}

	void RoadManager::SetupTexture(const Entity entity, sf::Texture* texture, const TextureId textureId, const std::string texturePath)
	{
		//Sprite Component part
		Sprite* sprite = m_SpriteManager->AddComponent(entity);
		sprite->SetTexture(texture);

		auto& spriteInfo = m_SpriteManager->GetComponentInfo(entity);
		spriteInfo.name = "Sprite";
		spriteInfo.sprite = sprite;
		spriteInfo.textureId = textureId;
		spriteInfo.texturePath = texturePath;
	}

	void RoadManager::SetupTextureGround(const Entity entity)
	{
		Sprite* sprite = m_SpriteManager->AddComponent(entity);
		sprite->SetTexture(m_GroundTexture);

		auto& spriteInfo = m_SpriteManager->GetComponentInfo(entity);
		spriteInfo.name = "Sprite";
		spriteInfo.sprite = sprite;
		spriteInfo.textureId = m_GroundTextureId;
		spriteInfo.texturePath = m_GroundTexturePath;
	}

}