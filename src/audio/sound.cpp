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
#include <utility/file_utility.h>


namespace sfge
{
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

SoundBufferId SoundBufferManager::LoadSoundBuffer(std::string filename)
{
}

sf::SoundBuffer* SoundBufferManager::GetSoundBuffer(SoundBufferId soundBufferId)
{
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
