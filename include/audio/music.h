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
#ifndef SFGE_MUSIC_H
#define SFGE_MUSIC_H

#include <vector>
#include <string>
#include <SFML/Audio/Music.hpp>

#include <engine/entity.h>
#include <engine/system.h>
namespace sfge
{
using MusicId = unsigned;
const MusicId INVALID_MUSIC_ID = 0U;


class Music
{
public:
	Music() = default;

	Music& operator=(const Music&) = delete;
	void SetPath(std::string path);
	void SetEntity(Entity entity);
	bool Load();
	void Play();
	void Stop();

	Entity GetEntity();
private:
	MusicId musicId = INVALID_MUSIC_ID;
	std::string m_Path = "";
	sf::Music m_Music;
	Entity m_Entity = INVALID_ENTITY;
};

class MusicManager : 
	public System, 
	public ResizeObserver
{
public:
	MusicManager(Engine& engine);
	~MusicManager() = default;

	MusicManager& operator=(const MusicManager&) = delete;
	void Init() override;
	/**
	* \brief open a music from a path file, put it on MusicMap and return the matchin id
	* \param filename The filename of the music file
	*/
	MusicId LoadMusic(const std::string& filename);
	/**
	* \brief return the music attached to the given musicId on MusicMap
	* \param musicId the id key of the music
	*/
	Music* GetMusic(MusicId musicId);

	void OnResize(size_t new_size) override;

protected:
	std::vector<Music> m_Musics{INIT_ENTITY_NMB};
	std::vector<std::string> m_MusicPaths{INIT_ENTITY_NMB};
	EntityManager& m_EntityManager;
	MusicId incrementId = 1U;
};

}

#endif