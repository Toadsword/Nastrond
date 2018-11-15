#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <map>

const std::string SAVE_FOLDER = "../data/animSaves/";

struct TextureInfos;
class AnimationManager;

class Utilities
{
public:
	static void ExportToJson(AnimationManager anim, std::vector<TextureInfos*> textures, std::string animName);
	static void ExportToGif(AnimationManager anim, std::vector<TextureInfos*> textures, std::string animName);
};

#endif // ifndef UTILITIES_H