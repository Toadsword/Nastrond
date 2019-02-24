/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

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

#ifndef SFGE_UI_H
#define SFGE_UI_H

#include <engine/entity.h>
#include <engine/system.h>
#include <engine/rect_transform.h>
#include <graphics/graphics2d.h>
#include <graphics/text.h>
#include <graphics/button.h>
#include <graphics/image.h>

namespace sfge
{	
	/*
	 * Author: Cédric Chambaz
	 */
	class UIManager : public System {
	public:
		using System::System;

		void Init() override;
		void Update(float dt) override;
		void Draw() override;

		ButtonManager* GetButtonManager();
		TextManager* GetTextManager();
		ImageManager* GetImageManager();
	private:
		// Managers
		EntityManager* m_EntityManager;
		RectTransformManager* m_RectTransformManager;
		Graphics2dManager* m_Graphics2dManager;
		ButtonManager m_ButtonManager{ m_Engine };
		TextManager m_TextManager{ m_Engine };
		ImageManager m_ImageManager{ m_Engine };

		sf::RenderWindow* m_Window;
	};
}
#endif
