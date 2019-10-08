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

namespace sfge
{
void AudioManager::Init()
{
	m_SoundManager.Init();
	m_SoundBufferManager.Init();
	
};
void AudioManager::Update(float dt)
{
	(void)dt;
};

SoundManager* AudioManager::GetSoundManager()
{
	return &m_SoundManager;
}

MusicManager* AudioManager::GetMusicManager()
{
	return &m_MusicManager;
}

SoundBufferManager* AudioManager::GetSoundBufferManager()
{
	return &m_SoundBufferManager;
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





}


