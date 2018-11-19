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

#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <map>

const std::string DATA_FOLDER = "../data/";
const std::string SAVE_FOLDER = "../data/animSaves/";

struct TextureInfos;
class AnimationManager;

enum LogSaveError {
	SAVE_SUCCESS,
	SAVE_FAILURE,
	SAVE_DO_REPLACE,
};

class Utilities
{
public:

	static LogSaveError ExportToJson(AnimationManager* anim, std::vector<TextureInfos*>* textures, bool confirmedReplacement = false);
	static LogSaveError ExportToGif(AnimationManager* anim, std::vector<TextureInfos*>* textures, bool confirmedReplacement = false);

	static char* Utilities::ConvertStringToArrayChar(std::string string, size_t size);

	// Works only on Windows
	// Function taken on by Mr.C64 : https://stackoverflow.com/questions/14762456/getclipboarddatacf-text
	static std::string GetClipboardText();
};

#endif // ifndef UTILITIES_H