/*
MIT License

Copyright (c) 2018 SAE Institute Switzerland AG

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

----------------------------
Author : Duncan Bourquard (bourquardduncan@gmail.com)
Date : November 2018
Project : AnimationTool for SFGE
*/

#include <map>

#include <animation_manager.h>
#include <texture_manager.h>

#include <utility/file_utility.h>
#include <utility/json_utility.h>

#include <iostream>
namespace sfge::tools
{
void AnimationManager::Init()
{
	m_animSpeed = DEFAULT_SPEED_VALUE;
	m_looped = true;
	m_animName = "NewAnimation";

	m_animation.clear();
	AddOrInsertKey(0);
}

void AnimationManager::AddNewKey(short key, short textureId)
{
	//Add a key at the end
	AddOrInsertKey();

	//Move to the right every frames until key is reached
	for (int i = GetHighestKeyNum(); i != key; i--)
	{
		SwapKeyTextures(i, i - 1);
	}
	//Assign it
	m_animation[key] = textureId;
}

bool AnimationManager::AddOrInsertKey()
{
	return AddOrInsertKey(GetHighestKeyNum() + 1, -1);
}

bool AnimationManager::AddOrInsertKey(short key, short textureId)
{
	if (m_animation.find(key) == m_animation.end())
	{
		m_animation.insert(std::make_pair(key, textureId));
		return true;
	}
	m_animation[key] = textureId;
	return true;
}

bool AnimationManager::RemoveKey()
{
	return RemoveKey(GetHighestKeyNum());
}

bool AnimationManager::RemoveKey(short key)
{
	if (m_animation.find(key) != m_animation.end())
	{
		for (auto frame : m_animation)
		{
			if (frame.first >= key && frame.first < GetHighestKeyNum())
			{
				SwapKeyTextures(frame.first, frame.first + 1);
			}
		}

		m_animation.erase(GetHighestKeyNum());

		if (m_animation.empty())
			AddOrInsertKey(0);
		return true;
	}
	return false;
}

bool AnimationManager::SetTextureOnKey(short key, short textureId)
{
	if (m_animation.find(key) != m_animation.end())
	{
		m_animation[key] = textureId;
		return true;
	}
	return false;
}

bool AnimationManager::SwapKeyTextures(short first, short second)
{
	if (m_animation.find(first) != m_animation.end() && m_animation.find(second) != m_animation.end())
	{
		short tmp = m_animation[first];
		m_animation[first] = m_animation[second];
		m_animation[second] = tmp;
		return true;
	}
	return false;
}

void AnimationManager::SetSpeed(float newSpeed)
{
	if (newSpeed > 0.0f)
		m_animSpeed = newSpeed;
}

float AnimationManager::GetSpeed()
{
	return m_animSpeed;
}

void AnimationManager::SetName(std::string newName)
{
	m_animName = newName;
}

std::string AnimationManager::GetName()
{
	return m_animName;
}

void AnimationManager::SetIsLooped(bool newLoop)
{
	m_looped = newLoop;
}

bool AnimationManager::GetIsLooped()
{
	return m_looped;
}

int AnimationManager::GetHighestKeyNum()
{
	int maxKey = 0;
	for (auto element : m_animation)
	{
		if (maxKey < element.first)
			maxKey = element.first;
	}
	return maxKey;
}

std::map<const short, short>& AnimationManager::GetAnim()
{
	return m_animation;
}

short AnimationManager::GetTextureIdFromKeyframe(short key)
{
	if (m_animation.find(key) != m_animation.end())
	{
		return m_animation[key];
	}
	return -1;
}

LogSaveError AnimationManager::ExportToJson(std::vector<TextureInfos*>* textures, bool confirmedReplacement)
{
	json value;	

	//Creating base folder
	if ((!sfge::IsDirectory(DATA_FOLDER) && sfge::CreateDirectory(DATA_FOLDER)) || 
		(!sfge::IsDirectory(SAVE_FOLDER) && sfge::CreateDirectory(SAVE_FOLDER)))
	{
		std::cout << sfge::IsDirectory(DATA_FOLDER) << " " << sfge::CreateDirectory(DATA_FOLDER) << " " << sfge::IsDirectory(SAVE_FOLDER) << " " << sfge::CreateDirectory(SAVE_FOLDER) << "\n";
		std::cout << "cannot create base Directory \n";
		return SAVE_FAILURE;
	}

	//Check if file already exists. If so, we ask if the user wants to replace it
	if (!confirmedReplacement)
	{
		std::cout << "confirmedReplacement\n";
		std::ifstream doAnimExists(SAVE_FOLDER + m_animName + ".json");
		confirmedReplacement = !confirmedReplacement && doAnimExists;
	}

	if (confirmedReplacement)
		return SAVE_DO_REPLACE;

	sfge::RemoveDirectory(SAVE_FOLDER + m_animName + "/");
	if (!sfge::CreateDirectory(SAVE_FOLDER + m_animName + "/"))
	{
		std::cout << "cannot create animation to save Directory \n";
		return SAVE_FAILURE;
	}

	// Json construction
	value["name"] = m_animName;
	value["isLooped"] = m_looped;
	value["speed"] = m_animSpeed;
	auto frames = m_animation;
	short index = 0;

	for (auto frame : frames)
	{
		TextureInfos* textToApply = nullptr;
		for (auto texture : *textures)
		{
			if (texture->id == frame.second)
			{
				textToApply = texture;
				break;
			}
		}

		if (textToApply == nullptr)
			continue;

		//Copy file into save folder
		std::ifstream myImage(SAVE_FOLDER + m_animName + "/" + textToApply->fileName);
		if (!myImage.good())
		{
			//std::cout << "saving image \n";
			std::ifstream  src(textToApply->path, std::ios::binary);
			std::ofstream  dst(SAVE_FOLDER + m_animName + "/" + textToApply->fileName, std::ios::binary);

			dst << src.rdbuf();
			dst.close();
		}
		myImage.close();

		//Registering information of the frame
		value["frames"][index]["key"] = frame.first;
		value["frames"][index]["filename"] = textToApply->fileName;
		value["frames"][index]["position"]["x"] = textToApply->position.x;
		value["frames"][index]["position"]["y"] = textToApply->position.y;
		value["frames"][index]["size"]["x"] = textToApply->size.x;
		value["frames"][index]["size"]["y"] = textToApply->size.y;

		index++;
	}

	// File write
	std::ofstream myfile;
	myfile.open(SAVE_FOLDER + m_animName + ".json");
	myfile.flush();
	myfile << std::setw(4) << value << std::endl;
	myfile.close();

	std::cout << "Animation saved in " << SAVE_FOLDER << m_animName << "/ \n";

	return SAVE_SUCCESS;
}

}
