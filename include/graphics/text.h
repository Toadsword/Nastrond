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

#ifndef SFGE_TEXT_H
#define SFGE_TEXT_H

#include <string>
#include <SFML/Graphics.hpp>
#include <engine/component.h>
#include <graphics/rect_transform.h>

namespace sfge
{
	/*
	 * Author: Cédric Chambaz
	 */
	class Text final
	{
	public:
		Text& operator=(const Text&);

		Text();
		~Text();

		void Init(const std::string text, const std::string fontPath, const sf::Uint8 color[4], const unsigned characterSize);
		void Update(const Vec2f position);
		void Draw(sf::RenderWindow& window) const;

		void SetTextString(const std::string newText);
		void SetSize(unsigned newSize);
		void SetColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
		void SetColor(sf::Color newColor);

		std::string GetTextString() const;
		std::string GetFont() const;
		unsigned GetSize() const;

	protected:
		sf::Text text;
		sf::Font font;
		sf::Color color = { 0,0,0,0 };
	};

	namespace editor
	{
		struct TextInfo : ComponentInfo
		{
			void DrawOnInspector() override;
			Text* text = nullptr;
		};
	}

	class TextManager final : public SingleComponentManager<Text, editor::TextInfo, ComponentType::TEXT> {
	public:
		using SingleComponentManager::SingleComponentManager;

		TextManager(Engine& engine);
		~TextManager();

		TextManager& operator=(const Text&);

		void CreateComponent(json& componentJson, Entity entity) override;
		Text* AddComponent(Entity entity) override;
		void DestroyComponent(Entity entity) override;

		void Init() override;
		void Update(float dt) override;
		void DrawTexts(sf::RenderWindow& window);

		void OnResize(size_t newSize) override;
	protected:
		RectTransformManager* m_RectTransformManager;
	};
}
#endif