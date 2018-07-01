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


#ifndef SFGE_SOUND_H
#define SFGE_SOUND_H

#include <SFML/Audio.hpp>
#include <map>

namespace sfge
{
class SoundManager;

/**
* \brief Sound class child is a Component
*/
class Sound
{
protected:
	sf::Sound* m_Sound = nullptr;
public:
	Sound();
	~Sound();
	/**
	* \brief initialize the Sound class
	*/
	void Init();

	void SetBuffer(sf::SoundBuffer* buffer);
	void Play();
	void Stop();
};

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	/**
	* \brief load a sf::SoundBuffer, put it on soundBufferMap and return the matchin id
	* \param filename The filename of the buffer file
	*/
	unsigned int LoadSoundBuffer(std::string filename);
	/**
	* \brief return the sf::SoundBuffer attached to the given sound_buffer_id on the soundBufferMap
	* \param sound_buffer_id The id key of the soundBuffer
	*/
	sf::SoundBuffer* GetSoundBuffer(unsigned int sound_buffer_id);


	void Reset();

	void Collect();

protected:
	/**
	* \brief The list where the sounds of LoadSound fuction of SoundManager was placed
	*/
	std::vector<Sound> m_Sounds;
	std::map<std::string, unsigned int> idsPathMap;
	std::map<unsigned int, unsigned int> idsRefCountMap;
	std::map<unsigned int, sf::SoundBuffer*> soundBufferMap;
	unsigned int incrementId = 0;
};
}
#endif