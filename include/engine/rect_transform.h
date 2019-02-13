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

#ifndef SFGE_RECT_TRANSFORM_H
#define SFGE_RECT_TRANSFORM_H

#include <editor/editor_info.h>
#include <utility/json_utility.h>
#include <engine/component.h>
#include <engine/transform2d.h>
#include <graphics/graphics2d.h>

namespace sfge
{
	struct RectTransform : Transform2d {
		void Update(Camera* camera);

		Vec2f basePosition = {0, 0};
	};

	struct RectTransformInfo : editor::ComponentInfo
	{
		void DrawOnInspector() override;
		RectTransform* rectTransform = nullptr;
	};

	class RectTransformManager : public SingleComponentManager<RectTransform, RectTransformInfo, ComponentType::RECTTRANSFORM> {
	public:
		using SingleComponentManager::SingleComponentManager;
		void CreateComponent(json& componentJson, Entity entity) override;
		RectTransform* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void Init();
		void Update(float dt) override;

	protected:
		CameraManager* m_CameraManager;
	};
}
#endif