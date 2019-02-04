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

#include <SFML/Graphics.hpp>
#include <editor/editor_info.h>
#include <utility/json_utility.h>
#include <engine/component.h>

namespace sfge
{
	enum ButtonState
	{
		NONE,
		HOVERED,
		CLICKED
	};

	struct Button {
		sf::Sprite sprite;
		sf::Sprite spriteHovered;
		sf::Sprite spriteClicked;
		sf::Texture texture;
		sf::Texture textureHovered;
		sf::Texture textureClicked;
		sf::Color color;
		sf::Color colorHovered;
		sf::Color colorClicked;
		void* action = nullptr;
		void* data = nullptr;

		ButtonState state = ButtonState::NONE;
	};

	struct ButtonInfo : editor::ComponentInfo
	{
		void DrawOnInspector() override;
		Button* button = nullptr;
	};

	class ButtonManager : public SingleComponentManager<Button, ButtonInfo, ComponentType::BUTTON> {
		using SingleComponentManager::SingleComponentManager;
		void CreateComponent(json& componentJson, Entity entity) override;
		Button* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void SetSprite(Entity entity, const std::string& newSpritePath);
		void SetSpriteHovered(Entity entity, const std::string& newSpritePath);
		void SetSpriteClicked(Entity entity, const std::string& newSpritePath);
		void SetAction(Entity entity, const void* function, const void* data);

		void CheckMouseOnButton(Entity entity, Vec2f& mousePosition);
		void OnClick(Entity entity, void* (function)(void *), void* data);
	};
}