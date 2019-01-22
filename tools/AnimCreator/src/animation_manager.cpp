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

#include <iostream>
#include <map>

#include <animation_manager.h>
#include <texture_manager.h>

#include <utility/file_utility.h>
#include <utility/json_utility.h>

namespace sfge::tools
{
void AnimationManager::Init()
{
	m_AnimSpeed = DEFAULT_SPEED_VALUE;
	m_Looped = true;
	m_AnimName = "NewAnimation";

	m_Animation.clear();
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
	m_Animation[key] = textureId;
}

bool AnimationManager::AddOrInsertKey()
{
	return AddOrInsertKey(GetHighestKeyNum() + 1, -1);
}

bool AnimationManager::AddOrInsertKey(short key, short textureId)
{
	if (m_Animation.find(key) == m_Animation.end())
	{
		m_Animation.insert(std::make_pair(key, textureId));
		return true;
	}
	m_Animation[key] = textureId;
	return true;
}

bool AnimationManager::RemoveKey()
{
	return RemoveKey(GetHighestKeyNum());
}

bool AnimationManager::RemoveKey(short key)
{
	if (m_Animation.find(key) != m_Animation.end())
	{
		for (auto frame : m_Animation)
		{
			if (frame.first >= key && frame.first < GetHighestKeyNum())
			{
				SwapKeyTextures(frame.first, frame.first + 1);
			}
		}

		m_Animation.erase(GetHighestKeyNum());

		if (m_Animation.empty())
			AddOrInsertKey(0);
		return true;
	}
	return false;
}

bool AnimationManager::SetTextureOnKey(short key, short textureId)
{
	if (m_Animation.find(key) != m_Animation.end())
	{
		m_Animation[key] = textureId;
		return true;
	}
	return false;
}

bool AnimationManager::SwapKeyTextures(short first, short second)
{
	if (m_Animation.find(first) != m_Animation.end() && m_Animation.find(second) != m_Animation.end())
	{
		short tmp = m_Animation[first];
		m_Animation[first] = m_Animation[second];
		m_Animation[second] = tmp;
		return true;
	}
	return false;
}

void AnimationManager::SetSpeed(float newSpeed)
{
	if (newSpeed > 0.0f)
		m_AnimSpeed = newSpeed;
}

float AnimationManager::GetSpeed()
{
	return m_AnimSpeed;
}

void AnimationManager::SetName(std::string newName)
{
	m_AnimName = newName;
}

std::string AnimationManager::GetName()
{
	return m_AnimName;
}

void AnimationManager::SetIsLooped(bool newLoop)
{
	m_Looped = newLoop;
}

bool AnimationManager::GetIsLooped()
{
	return m_Looped;
}

int AnimationManager::GetHighestKeyNum()
{
	int maxKey = 0;
	for (auto element : m_Animation)
	{
		if (maxKey < element.first)
			maxKey = element.first;
	}
	return maxKey;
}

std::map<const short, short>& AnimationManager::GetAnim()
{
	return m_Animation;
}

short AnimationManager::GetTextureIdFromKeyframe(short key)
{
	if (m_Animation.find(key) != m_Animation.end())
	{
		return m_Animation[key];
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
		return SAVE_FAILURE;
	}

	//Check if file already exists. If so, we ask if the user wants to replace it
	if (!confirmedReplacement)
	{
		std::ifstream doAnimExists(SAVE_FOLDER + m_AnimName + ".json");
		confirmedReplacement = !confirmedReplacement && doAnimExists;
	}

	if (confirmedReplacement)
		return SAVE_DO_REPLACE;

	sfge::RemoveDirectory(SAVE_FOLDER + m_AnimName + "/");
	if (!sfge::CreateDirectory(SAVE_FOLDER + m_AnimName + "/"))
	{
		return SAVE_FAILURE;
	}

	// Json construction
	value["name"] = m_AnimName;
	value["isLooped"] = m_Looped;
	value["speed"] = m_AnimSpeed;
	auto frames = m_Animation;
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
		std::ifstream myImage(SAVE_FOLDER + m_AnimName + "/" + textToApply->fileName);
		if (!myImage.good())
		{
			//std::cout << "saving image \n";
			std::ifstream  src(textToApply->path, std::ios::binary);
			std::ofstream  dst(SAVE_FOLDER + m_AnimName + "/" + textToApply->fileName, std::ios::binary);

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
	myfile.open(SAVE_FOLDER + m_AnimName + ".json");
	myfile.flush();
	myfile << std::setw(4) << value << std::endl;
	myfile.close();

	return SAVE_SUCCESS;
}

}
