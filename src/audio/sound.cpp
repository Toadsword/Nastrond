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
#include <string>
#include <list>
#include <set>

#include <audio/sound.h>
#include <audio/audio.h>
#include <engine/log.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <utility/file_utility.h>
#include "imgui.h"


namespace sfge
{

static std::set<std::string> sndExtensionSet
{
	".wav",
	".ogg",
	".flac"
};

Sound* SoundManager::AddComponent(Entity entity)
{
	auto& sound = GetComponentRef(entity);
	auto& soundInfo = GetComponentInfo(entity);

	
	soundInfo.Sound = &sound;
	m_EntityManager.AddComponentType(entity, ComponentType::SPRITE2D);
	return &sound;
}

void SoundManager::CreateComponent(json & componentJson, Entity entity)
{
	auto & newSound = m_Components[entity - 1];
	auto & newSoundInfo = m_ComponentsInfo[entity - 1];
	if (CheckJsonParameter(componentJson, "path", json::value_t::string))
	{
		auto path = componentJson["path"].get<std::string>();
		newSoundInfo.path = path;
		sf::SoundBuffer* soundBuffer = nullptr;
		if (FileExists(path))
		{
			const SoundBufferId soundBufferId = m_SoundBufferManager.LoadSoundBuffer(path);
			if (soundBufferId != INVALID_TEXTURE)
			{
				/*{
					std::ostringstream oss;
					oss << "Loading Sprite with Texture at: " << path << " with texture id: " << textureId;
					sfge::Log::GetInstance()->Msg(oss.str());
				}*/
				soundBuffer = m_SoundBufferManager.GetSoundBuffer(soundBufferId);
				newSound.SetBuffer(soundBuffer);
				newSoundInfo.SoundBufferId = soundBufferId;
			}
			else
			{
				std::ostringstream oss;
				oss << "Sound file " << path << " cannot be loaded";
				Log::GetInstance()->Error(oss.str());
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "Sound file " << path << " does not exist";
			Log::GetInstance()->Error(oss.str());
		}
	}
	else
	{
		Log::GetInstance()->Error("[Error] No Path for Sound");
	}

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
SoundManager::SoundManager(Engine& engine):
System(engine), m_EntityManager(engine.GetEntityManager()), m_SoundBufferManager(engine.GetAudioManager().GetSoundBufferManager())
{

}
void SoundManager::Reset()
{

}

void SoundManager::Collect()
{

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
	if (const auto config = m_Engine.GetConfig())
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
	return sndExtensionSet.find(extension) != sndExtensionSet.end();
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
	std::list<SoundBufferId> unusedBufferIds;
	for (auto i = 0U; i < m_SoundBufferCountRefs.size(); i++)
	{
		if (m_SoundBuffers[i] && m_SoundBufferCountRefs[i] == 0U)
		{
			unusedBufferIds.push_back(i + 1);
		}
	}
	for (auto unusedTextureId : unusedBufferIds)
	{
		m_SoundBuffers[unusedTextureId] = nullptr;
	}
}

SoundBufferId SoundBufferManager::LoadSoundBuffer(std::string filename)
{
	if(!FileExists (filename))
	{
		std::ostringstream oss;
		oss << "[ERROR] Sound buffer path: " << filename << " does not exist";
		Log::GetInstance()->Error(oss.str());
		return INVALID_SOUND_BUFFER;
	}
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
				oss << "[ERROR] Could not load sound file: " << filename;
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
			oss << "[ERROR] Could not load sound file: " << filename;
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
	
}
}

void sfge::editor::SoundInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Sound");
	ImGui::LabelText("Sound Path", path.c_str());
	ImGui::InputInt("Sound Id", reinterpret_cast<int*>(&SoundBufferId));
	
}