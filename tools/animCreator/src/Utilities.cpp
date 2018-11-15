#include <json.hpp>

#include <iostream>
#include <fstream>
#include <windows.h>

#include <AnimationManager.h>
#include <TextureManager.h>
#include <Utilities.h>

using json = nlohmann::json;

void Utilities::ExportToJson(AnimationManager anim, std::vector<TextureInfos*> textures, std::string animName)
{
	json value;

	if (!CreateDirectory((SAVE_FOLDER + animName + "/").c_str(), NULL))
	{
		if(ERROR_ALREADY_EXISTS)
		{
			std::cout << "FOLDER ALREADY EXISTS. \n";
		}
		else
		{
			std::cout << "COULDN'T CREATE FOLDER. \n";
			return;
		}
	}

	

	// Json construction
	value["frameFps"] = anim.GetFPSSpeed();
	auto frames = anim.GetAnim();
	short index = 0;

	for (auto frame : frames)
	{
		TextureInfos* textToApply = nullptr;
		for(auto textu : textures)
		{
			if (textu->id == frame.second)
			{
				textToApply = textu;
				break;
			}
		}

		if (textToApply == nullptr)
			continue;

		std::string fileName = textToApply->path;

		std::size_t found = fileName.find("/");
		while (found < fileName.length())
		{
			fileName = fileName.substr(found + 1);
			found = fileName.find("/");
		}

		//Copy file for saves
		//TODO VERIF SI LE FICHIER EST PAS DEJA EXISTANT DANS DESTINATION
		
		std::ifstream  src(textToApply->path, std::ios::binary);
		std::ofstream  dst(SAVE_FOLDER + animName + "/" + fileName, std::ios::binary);

		dst << src.rdbuf();
		
		value["frames"][index]["key"] = frame.first;		
		value["frames"][index]["path"] = fileName;
		value["frames"][index]["position"]["x"] = textToApply->position.x;
		value["frames"][index]["position"]["y"] = textToApply->position.y;
		value["frames"][index]["size"]["x"] = textToApply->size.x;
		value["frames"][index]["size"]["y"] = textToApply->size.y;
		
		index++;
	}

	// File write
	std::ofstream myfile;
	myfile.open(SAVE_FOLDER + animName + ".json");
	myfile << std::setw(4) << value << std::endl;
	myfile.close();
	
}

void Utilities::ExportToGif(AnimationManager anim, std::vector<TextureInfos*> textures, std::string animName)
{
}
