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

sfge::Sound::Sound(GameObject * gameObject) : Component(gameObject)
{
	m_Sound = new sf::Sound();
}

sfge::Sound::~Sound()
{
	if (m_Sound != nullptr)
	{
		delete(m_Sound);
		m_Sound = nullptr;
	}
}
void Sound::Init()
{
}
void Sound::Update(float dt)
{

}
Sound* Sound::LoadSound(Engine& engine, json & componentJson, GameObject* gameObject)
{
	auto audioManager = engine.GetAudioManager();
	auto soundManager = audioManager.GetSoundManager();
	if (&soundManager != nullptr)
	{
		Sound* newSound = new Sound(gameObject);
		soundManager->LoadSound(componentJson, newSound);
		return newSound;
	}
	return nullptr;
}
void Sound::SetBuffer(sf::SoundBuffer* buffer)
{
	m_Sound->setBuffer(*buffer);
}
SoundManager::SoundManager()
{

}
void SoundManager::LoadSound(json & componentJson, Sound* newSound)
{
	if (newSound == nullptr)
	{
		sfge::Log::GetInstance()->Error("Sound Component arg is null");
		return;
	}
	if (CheckJsonParameter(componentJson, "path", json::value_t::string))
	{
		std::string path = componentJson["path"].get<std::string>();
		if (FileExists(path))
		{
			unsigned int soundBufferId = LoadSoundBuffer(path);
			if (soundBufferId != 0U)
			{

				sf::SoundBuffer* buffer = GetSoundBuffer(soundBufferId);
				newSound->SetBuffer(buffer);
			}
			else
			{
				std::ostringstream oss;
				oss << "Could not find SoundBuffer for:" << path;
				Log::GetInstance()->Error(oss.str());
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "SoundBuffer file " << path << " does not exist";
			Log::GetInstance()->Error(oss.str());
		}
	}
	else
	{
		Log::GetInstance()->Error("[Error] No Path for Sound");
	}
	m_Sounds.push_back(newSound);
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
		if (idRefCountPair.second == 0U)
		{
			unusedIds.push_back(idRefCountPair.first);
		}
	}
	for (unsigned int soundBufferId : unusedIds)
	{
		idsRefCountMap.erase(soundBufferId);
		delete(soundBufferMap[soundBufferId]);
		soundBufferMap.erase(soundBufferId);
	}
	for (auto sound : m_Sounds)
	{
		delete(sound);
	}
	m_Sounds.clear();
}

void Sound::Play()
{
	m_Sound->play();

}

void Sound::Stop()
{
	m_Sound->stop();
}

SoundManager::~SoundManager()
{
	for (auto sound : m_Sounds)
	{
		delete(sound);
	}
	for (auto soundBuffer : soundBufferMap)
	{
		delete(soundBuffer.second);
	}
}
}