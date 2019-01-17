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
#include <engine/tile_asset.h>
#include <graphics/texture.h>
#include <graphics/graphics2d.h>

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
		oss << "Creating Configuration from " << filename;
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

	if (CheckJsonExists(jsonData, "id") && CheckJsonParameter(jsonData, "id", nlohmann::detail::value_t::number_integer))
	{
		if(jsonData["id"] > INVALID_TILE_TYPE)
			tiletypeId = jsonData["id"];
	}
	else
		return INVALID_TILE_TYPE;

	if (CheckJsonExists(jsonData, "texturePath") && CheckJsonParameter(jsonData, "texturePath", nlohmann::detail::value_t::string))
	{
		TextureId textId = m_TextureManager->LoadTexture(jsonData["texturePath"]);
		if(textId == INVALID_TEXTURE)
		{
			std::ostringstream oss;
			oss << "[Error] Couldn't load texture for tileType " << tiletypeId << ".\n";
			Log::GetInstance()->Error(oss.str());
			return INVALID_TILE_TYPE;
		}
		m_TexturesId[tiletypeId - 1] = textId;
	}

	return tiletypeId;
}

bool TileTypeManager::SetTileTexture(Entity tileId, TileTypeId tileTypeId)
{
	if (tileTypeId == INVALID_TILE_TYPE)
		return false;

	if (!m_Engine.GetEntityManager()->HasComponent(tileId, ComponentType::SPRITE2D))
		m_SpriteManager->AddComponent(tileId);
	
	m_SpriteManager->GetComponentPtr(tileId)->SetTexture(m_TextureManager->GetTexture(m_TexturesId[tileTypeId]));
	return true;
}

void TileTypeManager::Clear()
{
	for (auto& tiletypeId : m_TileTypeId)
	{
		tiletypeId = 0U;
	}
}

void TileTypeManager::Collect()
{

}
}
