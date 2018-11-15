#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <map>

const std::string SAVE_FOLDER = "../data/animSaves/";

struct TextureInfos;
class AnimationManager;

static enum LogSaveError {
	SUCCESS,
	FAILURE,
	DO_REPLACE,
};

class Utilities
{
public:

	static LogSaveError ExportToJson(AnimationManager* anim, std::vector<TextureInfos*>* textures, bool confirmedReplacement = false);
	static LogSaveError ExportToGif(AnimationManager* anim, std::vector<TextureInfos*>* textures, bool confirmedReplacement = false);
};

#endif // ifndef UTILITIES_H