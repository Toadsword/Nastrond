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

	std::map<const short, short> getAnim();

private:
	std::map<const short, short> m_animation;
};

#endif // ifndef ANIMATION_MANAGER_H