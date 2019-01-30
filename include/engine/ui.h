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

#include <queue>
#include <vector>
#include <any>

#include <engine/globals.h>
#include <utility/log.h>
#include <engine/entity.h>
#include <engine/system.h>
#include <engine/engine.h>
#include <engine/scene.h>
#include <engine/transform2d.h>
#include <editor/editor_info.h>
#include <editor/editor.h>

#include <utility/json_utility.h>
#include <engine/vector.h>
#include "graphics/sprite2d.h"
#include <engine/transform2d.h>

namespace sfge
{	
	struct RectTransform : Transform2d {
		Vec2f size;
		Vec2f halfSize;
		sf::Sprite sprite;
		sf::Texture texture;
		sf::Color color;
		void* action = nullptr;
		void* data = nullptr;
	};

	struct RectTransformInfo : editor::ComponentInfo
	{
		void DrawOnInspector() override;
		RectTransform* rectTransform = nullptr;
	};

	struct Text {
		sf::Text text;
		sf::Font font;
		sf::Color color;
	};

	struct TextInfo : editor::ComponentInfo
	{
		void DrawOnInspector() override;
		Text* text = nullptr;
	};

	/*struct Image {
		sf::Sprite sprite;
	};

	struct ImageInfo : editor::ComponentInfo
	{
		void DrawOnInspector() override;
		ImageInfo* image;
	};

	struct Button {
		void* action;
		void* data;
	};

	struct ButtonInfo : editor::ComponentInfo
	{
		void DrawOnInspector() override;
		Button* button = nullptr;
	};*/

	class RectTransformManager : public SingleComponentManager<RectTransform, RectTransformInfo, ComponentType::RECTTRANSFORM> {
		using SingleComponentManager::SingleComponentManager;
		void CreateComponent(json& componentJson, Entity entity) override;
		RectTransform* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void SetPosition(Entity entity, const Vec2f& newPosition);
		void SetScale(Entity entity, const Vec2f& newScale);
		void SetAngle(Entity entity, const float& newAngle);
		void SetSize(Entity entity, const Vec2f& newSize);
		void SetSprite(Entity entity, const std::string& newSpritePath);
		void SetAction(Entity entity, const void* function, const void* data);

		void CheckMouseOnButton(Entity entity, Vec2f& mousePosition);
		void OnClick(Entity entity, void* (function)(void *), void* data);

		Vec2f GetPosition(Entity entity);
		Vec2f GetScale(Entity entity);
		float GetAngle(Entity entity);
		Vec2f GetSize(Entity entity);
		//std::string GetSprite(Entity entity);
	};

	class TextManager : public SingleComponentManager<Text, TextInfo, ComponentType::TEXT> {
		using SingleComponentManager::SingleComponentManager;
		void CreateComponent(json& componentJson, Entity entity) override;
		Text* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void SetString(Entity entity, const std::string& newText);
		void SetFont(Entity entity, const std::string& newFontPath);
		void SetSize(Entity entity, const unsigned newSize);

		std::string GetString(Entity entity);
		std::string GetFont(Entity entity);
		unsigned GetSize(Entity entity);
	};

	/*class ImageManager : public SingleComponentManager<Image, ImageInfo, ComponentType::IMAGE> {
		using SingleComponentManager::SingleComponentManager;
		void CreateComponent(json& componentJson, Entity entity) override;
		Image* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void SetSprite(Entity entity, const std::string& newSpritePath);
		std::string GetSprite(Entity entity);
	};

	class ButtonManager : public SingleComponentManager<Button, ButtonInfo, ComponentType::BUTTON> {
		using SingleComponentManager::SingleComponentManager;
		void CreateComponent(json& componentJson, Entity entity) override;
		Button* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void CheckMouseOnButton(Entity entity, Vec2f mousePosition);
		void OnClick(Entity entity, void* (function)(void *), void* data);
	};*/

	class UIManager : public System {
	public:
		UIManager(Engine& engine);

		void Init() override;
		void Update(float dt) override;
		void FixedUpdate() override;
		void Draw() override;
	private:
		// Managers
		EntityManager* m_EntityManager;
		RectTransformManager* m_RectTransformManager;
		TextManager* m_TextManager;
		/*ImageManager* m_ImageManager;
		ButtonManager* m_ButtonManager;*/
	};
}
#endif
