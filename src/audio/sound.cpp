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
#include <sstream>
#include <list>
#include <audio/sound.h>
#include <audio/audio.h>
#include <engine/log.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <utility/file_utility.h>


namespace sfge
{

static std::set<std::string> sndExtensionSet
{
	".wav",
	".ogg",
	".flac"
};
unsigned int SoundManager::LoadSoundBuffer(std::string filename)
{
	//If already loaded
	if (idsPathMap.find(filename) != idsPathMap.end())
	{
		const unsigned int soundBufferId = idsPathMap[filename];

		if (soundBufferMap.find(soundBufferId) != soundBufferMap.end())
		{
			return idsPathMap[filename];
		}
		else
		{


			sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
			if (!soundBuffer->loadFromFile(filename))
			{
				{
					std::ostringstream oss;
					oss << "Error with loading sound: " << filename;
					sfge::Log::GetInstance()->Error(oss.str());
				}
				return 0U;
			}

			incrementId++;
			idsPathMap[filename] = incrementId;
			idsRefCountMap[incrementId] = 1U;
			soundBufferMap[incrementId] = soundBuffer;
			return incrementId;
		}
	}
	else if (FileExists(filename))
	{
		sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();

		if (!soundBuffer->loadFromFile(filename))
		{
			{
				std::ostringstream oss;
				oss << "Error with loading sound: " << filename;
				sfge::Log::GetInstance()->Error(oss.str());
			}
			return 0U;
		}
		{
			std::ostringstream oss;
			oss << "Loading Sound Buffer for: " << filename;
			sfge::Log::GetInstance()->Msg(oss.str());
		}
		incrementId++;
		idsPathMap[filename] = incrementId;

		idsRefCountMap[incrementId] = 1U;
		soundBufferMap[incrementId] = soundBuffer;
		return incrementId;
	}

	return 0U;
}


sf::SoundBuffer* SoundManager::GetSoundBuffer(unsigned int sound_buffer_id)
{
	if (soundBufferMap.find(sound_buffer_id) != soundBufferMap.end())
	{
		return soundBufferMap[sound_buffer_id];
	}
	return nullptr;

}

Sound* SoundManager::AddComponent(Entity entity)
{
	return nullptr;
}

void SoundManager::CreateComponent(json & componentJson, Entity entity)
{
}

void SoundManager::DestroyComponent(Entity entity)
{
}

sfge::Sound::Sound()
{
}

sfge::Sound::~Sound()
{
}
void Sound::Init()
{
}
void Sound::SetBuffer(sf::SoundBuffer* buffer)
{
	m_Sound.setBuffer(*buffer);
}
SoundManager::SoundManager(): ComponentManager()
{

}
void SoundManager::Reset()
{
	for (auto idRefCountPair : idsRefCountMap)
	{
		idsRefCountMap[idRefCountPair.first] = 0U;
	}
}

void SoundManager::Collect()
{
	std::list<unsigned int> unusedIds;
	for (auto idRefCountPair : idsRefCountMap)
	{
		const SoundId& soundId = idRefCountPair.first;
		const int& soundIdCount = idRefCountPair.second;
		if (soundIdCount == 0U)
		{
			unusedIds.push_back(soundId);
		}
	}
	for (unsigned int soundBufferId : unusedIds)
	{
		idsRefCountMap.erase(soundBufferId);
		soundBufferMap.erase(soundBufferId);
	}
	for (auto sound : m_Components)
	{
	}
	m_Components.clear();
}

void Sound::Play()
{
	m_Sound.play();

}

void Sound::Stop()
{
	m_Sound.stop();
}

SoundBufferManager::~SoundBufferManager()
{
	m_SoundBuffers.clear();
}

void SoundBufferManager::Init()
{
	if (const auto config = m_Engine.GetConfig().lock())
	{
		if (config->devMode)
		{
			LoadSoundBuffers(config->dataDirname);
		}
	}
}

void SoundBufferManager::LoadSoundBuffers(std::string dataDirname)
{
	std::function<void(std::string)> LoadAllSoundBuffers;
	LoadAllSoundBuffers = [&LoadAllSoundBuffers, this](std::string entry)
	{
		if (IsRegularFile(entry) && HasValidExtension(entry))
		{
			if(CalculateFileSize(entry) > MAX_SOUND_BUFFER_SIZE)
			{
				const auto newSoundBufferId = LoadSoundBuffer(entry);
				if (newSoundBufferId != INVALID_SOUND_BUFFER)
				{
					std::ostringstream oss;
					oss << "Loading soundbuffers: " << entry << "\n";
					Log::GetInstance()->Msg(oss.str());
				}
			}
		}

		if (IsDirectory(entry))
		{
			IterateDirectory(entry, LoadAllSoundBuffers);
		}
	};
	IterateDirectory(dataDirname, LoadAllSoundBuffers);
}


bool SoundBufferManager::HasValidExtension(std::string filename)
{
	const auto folderLastIndex = filename.find_last_of('/');
	const std::string::size_type filenameExtensionIndex = filename.find_last_of('.');
	if (filenameExtensionIndex >= filename.size())
	{
		return false;
	}

	//Check extension first
	const std::string extension = filename.substr(filenameExtensionIndex);
	if (sndExtensionSet.find(extension) == sndExtensionSet.end())
	{
		return false;
	}
	return true;
}

void SoundBufferManager::Clear()
{
	for (auto& soundBufferRefCount : m_SoundBufferCountRefs)
	{
		soundBufferRefCount = 0U;
	}
}

void SoundBufferManager::Collect()
{
	std::list<SoundBufferId> unusedTextureIds;
	for (auto i = 0U; i < m_SoundBufferCountRefs.size(); i++)
	{
		if (m_SoundBuffers[i] && m_SoundBufferCountRefs[i] == 0U)
		{
			unusedTextureIds.push_back(i + 1);
		}
	}
	for (auto unusedTextureId : unusedTextureIds)
	{
		m_SoundBuffers[unusedTextureId] = nullptr;
	}
}

SoundBufferId SoundBufferManager::LoadSoundBuffer(std::string filename)
{

	if (!HasValidExtension(filename))
	{
		std::ostringstream oss;
		oss << "[ERROR] Sound buffer path: " << filename << " has invalid extension";
		Log::GetInstance()->Error(oss.str());
		return INVALID_SOUND_BUFFER;
	}



	auto soundBufferId = INVALID_SOUND_ID;
	for (SoundBufferId checkedId = 1U; checkedId <= m_IncrementId; checkedId++)
	{
		if (filename == m_SoundBufferPaths[checkedId])
		{
			soundBufferId = checkedId;
		}
	}
	//Was or still is loaded
	if (soundBufferId != INVALID_SOUND_BUFFER)
	{

		//Check if the texture was destroyed
		if (m_SoundBuffers[soundBufferId - 1] != nullptr)
		{
			m_SoundBufferCountRefs[soundBufferId - 1]++;
			return soundBufferId;
		}
		else
		{
			auto soundBuffer = std::make_unique<sf::SoundBuffer>();
			if (!soundBuffer->loadFromFile(filename))
			{
				std::ostringstream oss;
				oss << "[ERROR] Could not load sound buffer file: " << filename;
				Log::GetInstance()->Error(oss.str());
				return INVALID_SOUND_BUFFER;
			}
			m_SoundBufferCountRefs[soundBufferId - 1] = 1U;
			m_SoundBuffers[soundBufferId - 1] = std::move(soundBuffer);
			return m_IncrementId;
		}
	}
	else
	{
		//Texture was never loaded
		if (FileExists(filename))
		{

			auto soundBuffer = std::make_unique<sf::SoundBuffer>();
			if (!soundBuffer->loadFromFile(filename))
			{
				std::ostringstream oss;
				oss << "[ERROR] Could not load texture file: " << filename;
				Log::GetInstance()->Error(oss.str());
				return INVALID_TEXTURE;
			}

			m_IncrementId++;
			m_SoundBufferPaths[m_IncrementId - 1] = filename;
			m_SoundBufferCountRefs[m_IncrementId - 1] = 1U;
			m_SoundBuffers[m_IncrementId - 1] = std::move(soundBuffer);
			return m_IncrementId;
		}
		else
		{
			std::ostringstream oss;
			oss << "[ERROR] Could not load texture file: " << filename;
			Log::GetInstance()->Error(oss.str());
		}
	}
	return INVALID_TEXTURE;
}

sf::SoundBuffer* SoundBufferManager::GetSoundBuffer(SoundBufferId soundBufferId)
{
	return m_SoundBuffers[soundBufferId - 1].get();
}

SoundManager::~SoundManager()
{
	for (auto sound : m_Components)
	{
	}
	for (auto soundBuffer : soundBufferMap)
	{
		delete(soundBuffer.second);
	}
}
}

void sfge::editor::SoundInfo::DrawOnInspector()
{
}
