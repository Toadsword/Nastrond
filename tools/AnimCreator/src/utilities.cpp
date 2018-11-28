/*
MIT License

Copyright (c) 2018 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

----------------------------
Author : Duncan Bourquard (bourquardduncan@gmail.com)
Date : November 2018
Project : AnimationTool for SFGE
*/


#include <iostream>
#include <fstream>

#include <utility/file_utility.h>
#include <utility/json_utility.h>

#include <animation_manager.h>
#include <texture_manager.h>
#include <utilities.h>


LogSaveError ExportToJson(AnimationManager* anim, std::vector<TextureInfos*>* textures, bool confirmedReplacement)
{
	json value;
	const std::string animName = anim->GetName();

	//Creating base folder
	if(!sfge::IsDirectory(DATA_FOLDER) && !sfge::CreateDirectory(DATA_FOLDER))
	{
		std::cout << "Couldn't create DATA folder";
		return SAVE_FAILURE;
	}
	if(!sfge::IsDirectory(SAVE_FOLDER) && !sfge::CreateDirectory(SAVE_FOLDER))
	{
		std::cout << "Couldn't create SAVE_FOLDER folder";
		return SAVE_FAILURE;
	}

	//Check if file already exists. If so, we ask if the user wants to replace it
	if (confirmedReplacement && sfge::IsDirectory(SAVE_FOLDER + animName + "/"))
		sfge::RemoveDirectory(SAVE_FOLDER + animName + "/");
	
	{
		std::ifstream doAnimExists(SAVE_FOLDER + animName + ".json");
		confirmedReplacement = !confirmedReplacement && doAnimExists;
	}

	if(confirmedReplacement)
		return SAVE_DO_REPLACE;

	if (!sfge::IsDirectory(SAVE_FOLDER + animName + "/") && !sfge::CreateDirectory(SAVE_FOLDER + animName + "/"))
	{
		std::cout << "Couldn't create ANIMATION folder";
		return SAVE_FAILURE;
	}

	// Json construction
	value["name"] = anim->GetName();
	value["isLooped"] = anim->GetIsLooped();
	value["speed"] = anim->GetSpeed();
	auto frames = anim->GetAnim();
	short arrayIndex = 0;

	for (auto frame : frames)
	{
		TextureInfos* textToApply = nullptr;
		for(auto texture : *textures)
		{
			if (texture->id == frame.second)
			{
				textToApply = texture;
				break;
			}
		}

		if (textToApply == nullptr)
			continue;

		//Get the file name from the path
		std::string fileName = textToApply->fileName;

		//Copy file into save folder
		if (!sfge::FileExists(SAVE_FOLDER + animName + "/" + fileName))
		{
			//std::cout << "saving image \n";
			std::ifstream  src(textToApply->path, std::ios::binary);
			std::ofstream  dst(SAVE_FOLDER + animName + "/" + fileName, std::ios::binary);

			dst << src.rdbuf();
			dst.close();
		}
		else
		{
			//std::cout << "image already copied\n";
		}

		//Registering information of the frame
		value["frames"][arrayIndex]["key"] = frame.first;		
		value["frames"][arrayIndex]["filename"] = fileName;
		value["frames"][arrayIndex]["position"]["x"] = textToApply->position.x;
		value["frames"][arrayIndex]["position"]["y"] = textToApply->position.y;
		value["frames"][arrayIndex]["size"]["x"] = textToApply->size.x;
		value["frames"][arrayIndex]["size"]["y"] = textToApply->size.y;
		
		arrayIndex++;
	}

	// File write
	std::ofstream myfile;
	myfile.open(SAVE_FOLDER + animName + ".json");
	myfile << std::setw(4) << value << std::endl;
	myfile.close();

	std::cout << "Animation saved in "<< SAVE_FOLDER << animName << "/ \n";

	return SAVE_SUCCESS;
}

//This is simply horrible....
// -> WorkAround?
char* ConvertStringToArrayChar(std::string string, size_t size)
{
	const auto result = new char[size];
	strcpy(result, string.c_str());
	return result;
}