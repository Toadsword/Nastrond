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

#include <string>
#include <SFML/Graphics.hpp>
#include <engine/component.h>
#include <input/input.h>
#include <engine/rect_transform.h>
#include <graphics/texture.h>
#include <python/pycomponent.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace sfge
{
	enum ButtonState
	{
		NONE,
		HOVERED,
		CLICKED
	};

	class Button final
	{
	public:
		Button& operator=(const Button&);

		//Button();

		void Init();
		void Update(Vec2f position);//RectTransform* rectTransform);
		void Draw(sf::RenderWindow& window) const;

		void SetSpriteNone(std::string spritePath);
		void SetSpriteHovered(std::string spritePath);
		void SetSpriteClicked(std::string spritePath);
		void SetColorNone(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
		void SetColorNone(sf::Color color);
		void SetColorHovered(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
		void SetColorHovered(sf::Color color);
		void SetColorClicked(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
		void SetColorClicked(sf::Color color);

		std::string spriteNonePath;
		std::string spriteHoveredPath;
		std::string spriteClickedPath;
		sf::Uint8 colorNone[4] = {0, 0, 0, 255};
		sf::Uint8 colorHovered[4] = { 0, 0, 0, 255 };
		sf::Uint8 colorClicked[4] = { 0, 0, 0, 255 };
		//char pyFunction = 'action';

		ButtonState state = ButtonState::NONE;
		TextureId spriteNone;
		TextureId spriteHovered;
		TextureId spriteClicked;
		sf::Sprite sprite;
		sf::Color color;
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

		ButtonManager& operator=(const Button&);

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
