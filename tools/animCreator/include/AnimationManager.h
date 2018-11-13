#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <map>

class AnimationManager
{
	
public:

	AnimationManager::AnimationManager() {};

	bool AddKey();
	bool AddKey(short TextureId);
	bool RemoveKey(short key);
	bool SetTextureOnKey(short key, short textureId);

	std::map<const short, short> getAnim();

private:
	std::map<const short, short> textures;
};

#endif // ifndef ANIMATION_MANAGER_H