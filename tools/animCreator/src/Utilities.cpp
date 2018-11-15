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

	//Creating base folder
	if(!CreateDirectory("../data/", NULL) && !CreateDirectory(SAVE_FOLDER.c_str(), NULL))
	{
		if (ERROR_ALREADY_EXISTS != GetLastError())
		{
			std::cout << "COULDN'T CREATE BASE FOLDER : error(" << GetLastError() << ")\n";
			return;
		}
	}

	if (!CreateDirectory((SAVE_FOLDER + animName + "/").c_str(), NULL))
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			std::cout << "FOLDER '"+ SAVE_FOLDER + animName +"/' ALREADY EXISTS. \n";
		}
		else
		{
			std::cout << "COULDN'T CREATE FOLDER '" + SAVE_FOLDER + animName + "/' : error(" << GetLastError()  << ")\n";
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
		std::ifstream my_file(SAVE_FOLDER + animName + "/" + fileName);
		if (!my_file.good())
		{
			std::cout << "saving image \n";
			std::ifstream  src(textToApply->path, std::ios::binary);
			std::ofstream  dst(SAVE_FOLDER + animName + "/" + fileName, std::ios::binary);

			dst << src.rdbuf();
			dst.close();
		}
		else
		{
			std::cout << "image already copied\n";
		}
		my_file.close();
		
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
