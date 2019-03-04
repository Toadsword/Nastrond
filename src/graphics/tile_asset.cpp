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
 
/*******************************
 * Author : Duncan Bourquard
 * Date : 16.01.2019
 */

#include <engine/engine.h>
#include <graphics/tile_asset.h>
#include <graphics/texture.h>
#include <graphics/graphics2d.h>
#include <utility/file_utility.h>

namespace sfge
{
	void TileTypeManager::Init()
	{
		System::Init();
		m_SpriteManager = m_Engine.GetGraphics2dManager()->GetSpriteManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
	}

	TileTypeId TileTypeManager::LoadTileType(std::string filename)
	{
		{
			std::ostringstream oss;
			oss << "Creating TileType Configuration from " << filename;
			Log::GetInstance()->Msg(oss.str());
		}

		auto jsonConfigPtr = LoadJson(filename);
		if (jsonConfigPtr == nullptr)
		{
			std::ostringstream oss;
			oss << "[Error] TileType JSON file: " << filename << " failed to open or did not parse as JSON";
			Log::GetInstance()->Error(oss.str());
			return INVALID_TILE_TYPE;
		}
		return LoadTileType(*jsonConfigPtr);
	}

	TileTypeId TileTypeManager::LoadTileType(json & jsonData)
	{
		auto tiletypeId = INVALID_TILE_TYPE;

		for (auto& tileTypeObj : jsonData)
		{
			if (CheckJsonExists(tileTypeObj, "id") && CheckJsonParameter(tileTypeObj, "id", nlohmann::detail::value_t::number_unsigned))
			{
				if (tileTypeObj["id"] > INVALID_TILE_TYPE)
					tiletypeId = tileTypeObj["id"];
			}
			else
				continue;

			m_TileTypeId[tiletypeId - 1] = tiletypeId;

			if (CheckJsonExists(tileTypeObj, "texturePath") && CheckJsonParameter(tileTypeObj, "texturePath", nlohmann::detail::value_t::string))
			{
				TextureId textId = m_TextureManager->LoadTexture(tileTypeObj["texturePath"]);
				if(textId == INVALID_TEXTURE)
				{
					std::ostringstream oss;
					oss << "[Error] Couldn't load texture for tileType " << tiletypeId << ".\n";
					Log::GetInstance()->Error(oss.str());
					return INVALID_TILE_TYPE;
				}

				m_TexturesId[tiletypeId - 1] = textId;
			}

			if(m_Incremental < tiletypeId)
				m_Incremental = tiletypeId;
			
		}
		return tiletypeId;
	}

	bool TileTypeManager::SetTileTexture(Entity tilemapId, TileId tileId, TileTypeId tileTypeId)
	{
		if (tileTypeId == INVALID_TILE_TYPE)
			return false;
		auto* tilemap = m_TilemapManager->GetComponentPtr(tilemapId);
		auto* sprite = tilemap->GetSprite(tileId);

		/*
		auto* sprite = m_SpriteManager->GetComponentPtr(tileId);
		if (!m_Engine.GetEntityManager()->HasComponent(tileId, ComponentType::SPRITE2D))
			sprite = m_SpriteManager->AddComponent(tileId);
		*/

		sprite->setTexture(*m_TextureManager->GetTexture(m_TexturesId[tileTypeId - 1]));
		sprite->setOrigin(sf::Vector2f(sprite->getLocalBounds().width, sprite->getLocalBounds().height) / 2.0f);

		//m_SpriteManager->GetComponentInfo(tileId).textureId = m_TexturesId[tileTypeId - 1];

		return true;
	}

	TextureId TileTypeManager::GetTextureFromTileType(TileTypeId tileTypeId)
	{
		for(int index = 0; index < m_TileTypeId.size(); index++)
		{
			if (m_TileTypeId[index] == tileTypeId)
				return m_TexturesId[index];
		}
		return INVALID_TEXTURE;
	}

	std::vector<size_t> TileTypeManager::GetAllTileTypeIds() const
	{
		return m_TileTypeId;
	}

	void TileTypeManager::AddNewTileType(std::string filename)
	{
		TextureId textId = m_TextureManager->LoadTexture(filename);
		if(textId != INVALID_TEXTURE)
		{
			for(int index = 0; index < m_TileTypeId.size(); index++)
			{
				if(m_TileTypeId[index] == INVALID_TILE_TYPE)
				{
					m_Incremental++;
					m_TileTypeId[index] = m_Incremental;
					m_TexturesId[index] = textId;
					break;
				}
			}
		}
	}

	void TileTypeManager::Clear()
	{
		for (auto& tiletypeId : m_TileTypeId)
			tiletypeId = INVALID_TILE_TYPE;
		
		for (auto& textureId : m_TexturesId)
			textureId = INVALID_TEXTURE;
	}

	void TileTypeManager::Collect()
	{

	}

	json TileTypeManager::Save()
	{
		json j;

		for(int i = 0; i < m_TileTypeId.size(); i++)
		{
			if(m_TileTypeId[i] != INVALID_TILE_TYPE)
			{
				j[i]["id"] = m_TileTypeId[i];
				j[i]["texturePath"] = m_TextureManager->GetTexturePath(m_TexturesId[i]);
			}
		}

		return j;
	}
}
