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


//STL
#include <sstream>
#include <list>
#include <set>

#include <graphics/texture.h>
#include <engine/log.h>
#include <engine/config.h>
#include <engine/engine.h>
#include <utility/file_utility.h>



namespace sfge
{

static std::set<std::string> imgExtensionSet
{
	".png",
	".jpg",
	".jpeg",
	".bmp",
	".tga",
	".gif",
	".psd",
	".hdr",
	".pic"
};


void TextureManager::Init()
{
	if(auto config = Engine::GetInstance()->GetConfig().lock())
	{
		if(config->devMode)
		{
			LoadTextures(config->dataDirname);
		}
	}
}

void TextureManager::LoadTextures(std::string dataDirname)
{
	std::function<void(std::string)> LoadAllTextures;
	LoadAllTextures = [&LoadAllTextures, this](std::string entry)
	{
		if (IsRegularFile(entry))
		{
			const TextureId newTextureId = LoadTexture(entry);
			if (newTextureId)
			{
				std::ostringstream oss;
				oss << "Loading texture: " << entry << "\n";
				Log::GetInstance()->Msg(oss.str());
			}
		}

		if (IsDirectory(entry))
		{
			{
				std::ostringstream oss;
				oss << "Opening folder: " << entry << "\n";
				Log::GetInstance()->Msg(oss.str());
			}
			IterateDirectory(entry, LoadAllTextures);
		}
	};
	IterateDirectory(dataDirname, LoadAllTextures);
}

TextureId TextureManager::LoadTexture(std::string filename)
{
	const auto folderLastIndex = filename.find_last_of('/');
	const std::string::size_type filenameExtensionIndex = filename.find_last_of('.');
	if (filenameExtensionIndex >= filename.size())
	{
		std::ostringstream oss;
		oss << "[ERROR] Texture path: " << filename << " has invalid extension";
		Log::GetInstance()->Error(oss.str());
		return INVALID_TEXTURE;
	}
	const std::string extension = filename.substr(filenameExtensionIndex);
	if(imgExtensionSet.find(extension) == imgExtensionSet.end())
	{
		std::ostringstream oss;
		oss << "[ERROR] Texture path: " << filename << " has invalid extension";
		Log::GetInstance()->Error(oss.str());
		return INVALID_TEXTURE;
	}
	//Check extension first
	
	if (m_NameIdsMap.find(filename) != m_NameIdsMap.end())
	{
		auto textureId = m_NameIdsMap[filename];
		//Check if the texture was destroyed
		const auto checkTexture = m_TexturesMap.find(textureId);
		if (checkTexture != m_TexturesMap.end())
		{
			m_IdsRefCountMap[textureId]++;
			return textureId;
		}
		else
		{
			auto* texture = new sf::Texture();
			if (!texture->loadFromFile(filename))
			{
				std::ostringstream oss;
				oss << "[ERROR] Could not load texture file: " << filename;
				Log::GetInstance()->Error(oss.str());
				return INVALID_TEXTURE;
			}
			m_IncrementId++;
			m_NameIdsMap[filename] = m_IncrementId;
			m_IdsRefCountMap[m_IncrementId] = 1U;
			m_TexturesMap[m_IncrementId] = texture;
			return m_IncrementId;
		}
	}
	else
	{
		if (FileExists(filename))
		{
			m_IncrementId++;
			auto texture = new sf::Texture();
			if (!texture->loadFromFile(filename))
				return INVALID_TEXTURE;
			m_NameIdsMap[filename] = m_IncrementId;
			m_IdsRefCountMap[m_IncrementId] = 1U;
			m_TexturesMap[m_IncrementId] = texture;
			return m_IncrementId;
		}
	}
	return INVALID_TEXTURE;
}






sf::Texture* TextureManager::GetTexture(unsigned int text_id)
{
	if (m_TexturesMap.find(text_id) != m_TexturesMap.end())
	{
		return m_TexturesMap[text_id];
	}
	return nullptr;
}

void TextureManager::Clear()
{
	for (auto idRefCountPair : m_IdsRefCountMap)
	{
		m_IdsRefCountMap[idRefCountPair.first] = 0U;
	}
}

void TextureManager::Collect()
{
	std::list<TextureId> unusedTextureIds;
	for (auto idRefCountPair : m_IdsRefCountMap)
	{
		auto textureId = idRefCountPair.first;
		auto refCount = idRefCountPair.second;
		if (refCount == 0U)
		{
			unusedTextureIds.push_back(textureId);
		}
	}
	for (auto unusedTextureId : unusedTextureIds)
	{
		delete(m_TexturesMap[unusedTextureId]);
		m_TexturesMap.erase(unusedTextureId);
	}
}

}
