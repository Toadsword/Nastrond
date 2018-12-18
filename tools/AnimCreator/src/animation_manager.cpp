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
#include <iostream>
namespace sfge::tools
{
void AnimationManager::Init()
{
	m_animSpeed = 100;
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

void AnimationManager::SetSpeed(int newSpeed)
{
	if (newSpeed > 0.0f)
		m_animSpeed = newSpeed;
}

int AnimationManager::GetSpeed()
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
}
