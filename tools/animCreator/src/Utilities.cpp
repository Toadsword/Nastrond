#include <json.hpp>

#include <iostream>
#include <fstream>
#include <windows.h>

#include <TextureManager.h>
#include <Utilities.h>

using json = nlohmann::json;

void Utilities::ExportToJson(std::map<short, short> anim, std::vector<TextureInfos*> textures)
{
	json value;

	value["frameFps"] = 60;
	if (!CreateDirectory("/data/saves/", NULL))
	{
		std::cout << "LE FOLDER N'A PAS ETE CREE \n";
		return;
	}
	std::ofstream test1("/data/saves/coucou.json");
	test1 << std::setw(4) << value << std::endl;

	test1.close();
}

void Utilities::ExportToGif(std::map<short, short> anim, std::vector<TextureInfos*> textures)
{
}
