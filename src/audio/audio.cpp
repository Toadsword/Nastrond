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

#include <audio/audio.h>
#include <audio/sound.h>
#include <iostream>
#include <utility/file_utility.h>
#include <engine/log.h>

namespace sfge
{
void AudioManager::Init()
{

};
void AudioManager::Update(sf::Time dt)
{

};

SoundManager&  AudioManager::GetSoundManager()
{
	return m_SoundManager;
}

MusicManager&  AudioManager::GetMusicManager()
{
	return m_MusicManager;
}
void AudioManager::Clear()
{
	m_SoundManager.Reset();
}

void AudioManager::Collect()
{
	m_SoundManager.Collect();
}
void AudioManager::Destroy()
{
	Clear();
	Collect();
}

MusicManager::MusicManager()
{
}
unsigned int MusicManager::LoadMusic(std::string filename)
{
	if (musicPathId.find(filename) != musicPathId.end())
	{
		auto musicId = musicPathId[filename];
		auto checkMusic = musicMap.find(musicId);

		if (checkMusic != musicMap.end())
		{
			std::cout << "music return";
			return musicPathId[filename];
		}
		else
		{
			std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
			if (!music->openFromFile(filename))
			{
				std::cout << "music not open from file";
				return 0U;
			}
			musicPathId[filename] = incrementId;
			musicMap[incrementId] = music;
			return incrementId;
		}
	}
	else
	{
		if (FileExists(filename))
		{
			incrementId++;
			auto music = std::make_shared<sf::Music>();

			if (!music->openFromFile(filename))
			{
				std::cout << "music not open from file but exist";
				return 0U;
			}
			musicPathId[filename] = incrementId;
			musicMap[incrementId] = music;
			return incrementId;
		}
	}
	return 0U;
}

std::shared_ptr<sf::Music> MusicManager::GetMusic(unsigned int musicId)
{
	if (musicMap.find(musicId) != musicMap.end())
	{
		return musicMap[musicId];
	}
	return nullptr;
}




}


