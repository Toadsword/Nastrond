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
#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <map>

class AnimationManager
{
	
public:

	AnimationManager::AnimationManager() {};

	void Init();

	bool AddKey(short key);
	bool AddKey(short key, short TextureId);
	bool RemoveKey(short key);
	bool SetTextureOnKey(short key, short textureId);

	void SetFPSSpeed(short newSpeed);
	short GetFPSSpeed();
	void SetName(std::string newName);
	std::string GetName();
	void SetLooped(bool newLoop);
	bool GetLooped();

	std::map<const short, short>& GetAnim();

private:
	bool m_looped = false;
	short m_animSpeed = 100;
	std::string m_animName = "TestAnimMaggle";
	std::map<const short, short> m_animation;
};

#endif // ifndef ANIMATION_MANAGER_H