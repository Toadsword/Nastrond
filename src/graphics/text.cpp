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

#include <graphics/text.h>

namespace sfge
{
	void TextInfo::DrawOnInspector()
	{
		
	}

	void TextManager::CreateComponent(json& componentJson, Entity entity)
	{
		auto* text = AddComponent(entity);
		if (CheckJsonExists(componentJson, "text"))
			text->text.setString(componentJson["text"]);
		if (CheckJsonExists(componentJson, "size"))
			text->text.setCharacterSize(componentJson["size"]);
		if (CheckJsonExists(componentJson, "font"))
		{
			if (text->font.loadFromFile(componentJson["font"]))
				text->text.setFont(text->font);
		}

		// Set default color value
		text->color.r = 0;
		text->color.g = 0;
		text->color.b = 0;
		text->color.a = 1;

		if (CheckJsonExists(componentJson, "red"))
			text->color.r = componentJson["red"];
		if (CheckJsonExists(componentJson, "green"))
			text->color.g = componentJson["green"];
		if (CheckJsonExists(componentJson, "blue"))
			text->color.b = componentJson["blue"];
		if (CheckJsonExists(componentJson, "alpha"))
			text->color.a = componentJson["alpha"];
	}

	Text* TextManager::AddComponent(Entity entity)
	{
		auto& text = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].text = &text;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::TEXT);
		return &text;
	}

	void TextManager::DestroyComponent(Entity entity)
	{
		m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::TEXT);
	}

	void TextManager::SetString(Entity entity, const std::string& newText)
	{
		GetComponentPtr(entity)->text.setString(newText);
	}

	void TextManager::SetFont(Entity entity, const std::string& newFontPath)
	{
		auto component = GetComponentPtr(entity);

		if (component->font.loadFromFile(newFontPath))
			component->text.setFont(component->font);
	}

	void TextManager::SetSize(Entity entity, const unsigned newSize)
	{
		GetComponentPtr(entity)->text.setCharacterSize(newSize);
	}

	std::string TextManager::GetString(Entity entity)
	{
		return GetComponentPtr(entity)->text.getString();
	}

	std::string TextManager::GetFont(Entity entity)
	{
		return GetComponentPtr(entity)->text.getFont()->getInfo().family;
	}

	unsigned TextManager::GetSize(Entity entity)
	{
		return GetComponentPtr(entity)->text.getCharacterSize();
	}
}
