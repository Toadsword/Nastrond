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
}