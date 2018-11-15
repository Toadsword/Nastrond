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