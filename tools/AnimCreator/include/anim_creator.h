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

*/

#ifndef SFGE_ANIMCREATOR_H
#define SFGE_ANIMCREATOR_H

#include <engine/system.h>
#include <graphics_manager.h>
#include <animation_manager.h>

namespace sfge::tools
{

//This will allow to call the tools directly from a central executable called SFGE_EDITOR
class AnimCreator : public System
{
public:
	using System::System;
	void Init() override;
	void Update(float dt) override;
	void Draw() override;

	GraphicsManager* GetGraphicsManager();
	TextureManager* GetTextureManager();
	AnimationManager* GetAnimationManager();
protected:
	/**
	 * \brief Pointer to the Graphics Manager of the Engine.
	 */
	GraphicsManager m_GraphicsManager;
	/**
	 * \brief Pointer to the Texture Manager of the Engine.
	 */
	TextureManager m_TextureManager;
	/**
	 * \brief Pointer to the Animation Manager of the Engine.
	 */
	AnimationManager m_AnimationManager;

	bool m_isInit = false;
};
}

#endif //SFGE_ANIMCREATOR_H
