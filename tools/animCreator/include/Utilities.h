#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <map>

struct TextureInfos;

class Utilities
{
public:
	static void ExportToJson(std::map<short, short> anim, std::vector<TextureInfos*> textures);
	static void ExportToGif(std::map<short, short> anim, std::vector<TextureInfos*> textures);
};

#endif // ifndef UTILITIES_H