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

#ifndef ANIM_TOOL_UTILITIES_H
#define ANIM_TOOL_UTILITIES_H

#include <vector>
#include <map>

const std::string DATA_FOLDER = "../data/";
const std::string SAVE_FOLDER = "../data/animSaves/";

struct TextureInfos;
class AnimationManager;

/**
 * \brief Enum used to reference the state of last saved animation in the application.
 */
enum LogSaveError {
	SAVE_SUCCESS,
	SAVE_FAILURE,
	SAVE_DO_REPLACE,
};


/**
 * \brief Export the current animation to Json, ready for SFGE.
 * \param anim AnimationManager that stores all the informations
 * \param textures All the textures loaded in the application.
 * \param confirmedReplacement True if the application ignores the existent files, false otherwise.
 * \return The state of the exportation.
 */
LogSaveError ExportToJson(AnimationManager* anim, std::vector<TextureInfos*>* textures, bool confirmedReplacement = false);

/**
 * \brief Convert current string into an array of char.
 * \param string String to convert.
 * \param size Size of the array of char.
 * \return Pointer to the array of char.
 */
char* ConvertStringToArrayChar(std::string string, size_t size);


#endif // ifndef UTILITIES_H