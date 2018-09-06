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

#include <audio/music.h>
#include <utility/file_utility.h>
#include <engine/log.h>
#include <engine/engine.h>

namespace  sfge
{

void Music::SetPath(const std::string path)
{
	m_Path = path;
}

void Music::SetEntity(Entity entity)
{
	m_Entity = entity;
}

bool Music::Load()
{
	return m_Music.openFromFile(m_Path);
}

void Music::Play()
{
	m_Music.play();
}

void Music::Stop()
{
	m_Music.stop();
}

Entity Music::GetEntity()
{
	return m_Entity;
}



MusicId MusicManager::LoadMusic(const std::string& filename)
{
	const auto pathIterator = std::find(m_MusicPaths.cbegin(), m_MusicPaths.cend(), filename);
	if (pathIterator != m_MusicPaths.end())
	{
		const MusicId musicId = pathIterator - m_MusicPaths.cbegin() + 1;
		return musicId;
		
	}
	else
	{
		if (FileExists(filename))
		{
			MusicId musicId = incrementId;
			auto& music = m_Musics[musicId-1];
			music.SetPath(filename);
			if(music.Load())
			{
				std::ostringstream oss;
				oss << "[Error] Music at path: " << filename << " could not be loaded";
				Log::GetInstance()->Error(oss.str());
				return INVALID_MUSIC_ID;
			}

			incrementId++;
			return musicId;
		}
	}
	return INVALID_MUSIC_ID;
}

Music * sfge::MusicManager::GetMusic(MusicId musicId)
{
	if (musicId == INVALID_MUSIC_ID)
		return nullptr;
	return &m_Musics[musicId-1];
}

}
