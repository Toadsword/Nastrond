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

#include <SFML/Graphics.hpp>
#include <editor/editor_info.h>
#include <utility/json_utility.h>
#include <engine/component.h>
#include <Input/input.h>
#include <engine/rect_transform.h>
#include <graphics/graphics2d.h>

namespace sfge
{
	enum ButtonState
	{
		NONE,
		HOVERED,
		CLICKED
	};

	struct Button {
	public:
		void Init();
		void Update(RectTransform* rectTransform);
		void Draw(sf::RenderWindow& window) const;

		void SetSpriteNone(const std::string& spritePath);
		void SetSpriteHovered(const std::string& spritePath);
		void SetSpriteClicked(const std::string& spritePath);
		void SetColorNone(const sf::Uint8 r, const sf::Uint8 g, const sf::Uint8 b, const sf::Uint8 a);
		void SetColorNone(sf::Color color);
		void SetColorHovered(const sf::Uint8 r, const sf::Uint8 g, const sf::Uint8 b, const sf::Uint8 a);
		void SetColorHovered(sf::Color color);
		void SetColorClicked(const sf::Uint8 r, const sf::Uint8 g, const sf::Uint8 b, const sf::Uint8 a);
		void SetColorClicked(sf::Color color);

		std::string spriteNonePath;
		std::string spriteHoveredPath;
		std::string spriteClickedPath;
		sf::Uint8 colorNone[4] = {0, 0, 0, 255};
		sf::Uint8 colorHovered[4] = { 0, 0, 0, 255 };
		sf::Uint8 colorClicked[4] = { 0, 0, 0, 255 };

		ButtonState state = ButtonState::NONE;
		TextureId spriteNone;
		TextureId spriteHovered;
		TextureId spriteClicked;
		sf::Sprite sprite;
		sf::Color color;
	};

	struct ButtonInfo : editor::ComponentInfo
	{
		void DrawOnInspector() override;
		Button* button = nullptr;
	};

	class ButtonManager : public SingleComponentManager<Button, ButtonInfo, ComponentType::BUTTON> {
	public:
		using SingleComponentManager::SingleComponentManager;
		void CreateComponent(json& componentJson, Entity entity) override;
		Button* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void Init() override;
		void Update(float dt) override;
		void DrawButtons(sf::RenderWindow& window);

		void LoadSprites();
		void SetSprite(Button* button) const;
	protected:
		Vec2f mousePosition;
		RectTransformManager* m_RectTransformManager;
		MouseManager* m_MouseManager;
		TextureManager* m_TextureManager;
	};
}
#endif
