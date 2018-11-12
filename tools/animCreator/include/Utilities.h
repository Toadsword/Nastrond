#ifndef UTILITIES_H
#define UTILITIES_H

#include <map>

class Utilities
{
public:
	void ExportToJson(std::map<short, short> anim);
	void ExportToGif(std::map<short, short> anim);
};

#endif // ifndef UTILITIES_H