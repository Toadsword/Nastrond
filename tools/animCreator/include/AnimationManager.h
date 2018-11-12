#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <map>

class AnimationManager
{
	
private:
	std::map<const short, short> textures;
public:
	bool AddKey();
	bool AddKey(short TextureId);
	bool RemoveKey(short key);
	bool SetTextureOnKey(short key, short textureId);

	std::map<const short, short> getAnim();
};

#endif // ifndef ANIMATION_MANAGER_H