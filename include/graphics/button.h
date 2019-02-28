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

#ifndef SFGE_BUTTON_H
#define SFGE_BUTTON_H

#include <engine/component.h>
#include <graphics/rect_transform.h>
#include <python/pycomponent.h>

namespace sfge
{
	/*
	 * Author: Cédric Chambaz
	 */
	class Button final
	{
	public:
		Button& operator=(const Button&) = delete;

		Button();
		~Button();

		void Init();
	};

	namespace editor
	{
		struct ButtonInfo : ComponentInfo
		{
			void DrawOnInspector() override;
			Button* button = nullptr;
		};
	}

	class ButtonManager final : public SingleComponentManager<Button, editor::ButtonInfo, ComponentType::BUTTON> {
	public:
		using SingleComponentManager::SingleComponentManager;

		ButtonManager(Engine& engine);
		~ButtonManager();

		ButtonManager& operator=(const Button&) = delete;

		void CreateComponent(json& componentJson, Entity entity) override;
		Button* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void Init() override;

		void OnResize(size_t newSize) override;
	protected:
		RectTransformManager* m_RectTransformManager;
	};
}
#endif
