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
#include <map>
#include <vector>

#include <SFML/Audio.hpp>
#include <engine/component.h>
#include <editor/editor.h>

namespace sfge
{
class SoundManager;

using SoundId = unsigned int;
const SoundId INVALID_SOUND_ID = 0U;

using SoundBufferId = unsigned;
const SoundBufferId INVALID_SOUND_BUFFER = 0U;

const auto MAX_SOUND_BUFFER_SIZE = 1'000'000ll;

namespace editor
{
class SoundInfo : public ComponentInfo, PathEditorComponent
{
	void DrawOnInspector() override;
};
}
/**
* \brief Sound class child is a Component
*/
class Sound
{
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

protected:
	sf::Sound m_Sound;
};

class SoundBufferManager : public System
{
public:

	using System::System;

	~SoundBufferManager();

	void Init() override;

	void LoadSoundBuffers(std::string filename);

	void Clear() override;
	
	void Collect() override;

	SoundBufferId LoadSoundBuffer(std::string filename);
	sf::SoundBuffer* GetSoundBuffer(SoundBufferId soundBufferId);
private:

  	bool HasValidExtension(std::string filename);
	std::vector<std::string> m_SoundBufferPaths{ INIT_ENTITY_NMB };
	std::vector<unsigned int> m_SoundBufferCountRefs{ INIT_ENTITY_NMB };
	std::vector<std::unique_ptr<sf::SoundBuffer>> m_SoundBuffers{INIT_ENTITY_NMB};
	SoundBufferId m_IncrementId = 0U;

};

class SoundManager : public ComponentManager<Sound, editor::SoundInfo>
{
public:
	SoundManager();
	~SoundManager();


	Sound* AddComponent(Entity entity) override;
	void CreateComponent(json& componentJson, Entity entity) override;
	void DestroyComponent(Entity entity) override;
	
	void Reset();

	void Collect();

protected:

	std::map<std::string, SoundId> idsPathMap;
	std::map<SoundId, unsigned int> idsRefCountMap;
	std::map<SoundId, sf::SoundBuffer*> soundBufferMap;
	SoundId incrementId = INVALID_SOUND_ID;
};
}
#endif