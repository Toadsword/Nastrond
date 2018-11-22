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
#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include <map>

/**
 * \brief AnimationManager : Store and manager the animation of the application.
 */
class AnimationManager
{	
public:

	/**
	 * \brief Constructor of Animation Manager.
	 */
	AnimationManager() {};

	/**
	 * \brief Initializes the Animations manager, creating the required variables needed to function.
	 */
	void Init();

	/**
	 * \brief Add a new frame of animation at the end of it.
	 * \return true if created, false otherwise.
	 */
	bool AddOrInsertKey();

	/**
	 * \brief Add a new frame at specified key location. If already created, assigns the specified texture.
	 * \param key Id of the keyframe.
	 * \param textureId Id of the texture to apply.
	 * \return true if created, false otherwise.
	 */
	bool AddOrInsertKey(short key, short textureId = -1);

	/**
	 * \brief Add a new keyframe at specified key location. If already created or assigned, create a new keyframe at the specified location and assigns the specified texture.
	 * \param key Id of the new keyframe.
	 * \param textureId Id of the texture to apply.
	 */
	void AddNewKey(short key, short textureId = -1);

	/**
	 * \brief Remove the last keyframe of the current m_animation.
	 * \return true if removed, false otherwise.
	 */
	bool RemoveKey();

	/**
	 * \brief Remove the keyframe specified by key of the current m_animation.
	 * \param key Id of the keyframe to remove.
	 * \return true if removed, false otherwise.
	 */
	bool RemoveKey(short key);
	/**
	 * \brief Change the texture Id of given keyframe Id.
	 * \param key Id of the keyframe to apply the new texture.
	 * \param textureId Id of the texture to apply.
	 * \return 
	 */
	bool SetTextureOnKey(short key, short textureId);
	/**
	 * \brief Exchange the position of two textures among the two given keyframes.
	 * \param first keyframe Id.
	 * \param second keyframe Id.
	 * \return true if exchanged, false otherwise.
	 */
	bool SwapKeyTextures(short first, short second);

	/**
	 * \brief Setter of the speed of the animation.
	 * \param newSpeed New speed of the animation.
	 */
	void SetSpeed(int newSpeed);

	/**
	 * \brief Getter of the speed of the animation.
	 * \return Current speed of the animation.
	 */
	int GetSpeed();

	/**
	 * \brief Setter of the name of the animation.
	 * \param newName New name of the animation.
	 */
	void SetName(std::string newName);

	/**
	 * \brief Getter of the name of the animation.
	 * \return Current name of the animation.
	 */
	std::string GetName();

	/**
	 * \brief Setter of the loop state of animation.
	 * \param newIsLoop New loop state of the animation.
	 */
	void SetIsLooped(bool newIsLoop);

	/**
	 * \brief Getter of the loop state of the animation.
	 * \return Current loop state of the animation.
	 */
	bool GetIsLooped();

	/**
	 * \brief Getter of the last id among all keyframes of the animation.
	 * \return Highest Id found among the keyframes.
	 */
	int GetHighestKeyNum();
	
	/**
	 * \brief Getter of the animation.
	 * \return the reference on the whole animation.
	 */
	std::map<const short, short>& GetAnim();

	/**
	 * \brief Getter of the Texture Id of the given keyframe Id given.
	 * \param key Id of the keyframes.
	 * \return Id of the Texture stored in keyframe given, -1 if not found.
	 */
	short GetTextureIdFromKeyframe(short key);

private:
	/**
	 * \brief Loop state of the animation; Default value : false
	 */
	bool m_looped = true;

	/**
	 * \brief Speed of the animation in miliseconds(ms) per frame; Default value : 100ms per frame
	 */
	int m_animSpeed = 100;

	/**
	 * \brief Name of the animation; Default value : "NewAnimation"
	 */
	std::string m_animName = "NewAnimation";

	/**
	 * \brief Where every informations about the keyframes and textures Id are stored.
	 */
	std::map<const short, short> m_animation;
};

#endif // ifndef ANIMATION_MANAGER_H