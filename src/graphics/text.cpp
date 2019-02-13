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
	void Text::Init()
	{
		
	}

	void Text::Update(RectTransform* rectTransform)
	{
		text.setPosition(rectTransform->Position);
	}

	void Text::Draw(sf::RenderWindow& window) const
	{
		window.draw(text);
	}

	void Text::SetString(const std::string& newText)
	{
		text.setString(newText);
	}

	void Text::SetFont(const std::string& newFontPath)
	{
		if (font.loadFromFile(newFontPath))
			text.setFont(font);
	}

	void Text::SetSize(const unsigned newSize)
	{
		text.setCharacterSize(newSize);
	}

	std::string Text::GetString() const
	{
		return text.getString();
	}

	std::string Text::GetFont() const
	{
		return font.getInfo().family;
	}

	unsigned Text::GetSize() const
	{
		return text.getCharacterSize();
	}

	void Text::SetColor(const sf::Uint8 r, const sf::Uint8 g, const sf::Uint8 b, const sf::Uint8 a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;

		text.setColor(color);
	}

	void Text::SetColor(sf::Color newColor)
	{
		color = newColor;
		text.setColor(color);
	}

	void TextInfo::DrawOnInspector()
	{
		
	}

	void TextManager::CreateComponent(json& componentJson, Entity entity)
	{
		if (CheckJsonExists(componentJson, "text"))
			m_Components[entity].SetString(componentJson["text"]);
		if (CheckJsonExists(componentJson, "size"))
			m_Components[entity].SetSize(componentJson["size"]);
		if (CheckJsonExists(componentJson, "font"))
			m_Components[entity].SetFont(componentJson["font"]);
		if (CheckJsonExists(componentJson, "color"))
			m_Components[entity].SetColor(componentJson["color"][0], componentJson["color"][1], componentJson["color"][2], componentJson["color"][3]);
		else
			m_Components[entity].SetColor(sf::Color::Black);
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

	void TextManager::Init()
	{
		SingleComponentManager::Init();
	}

	void TextManager::Update(float dt)
	{
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::TEXT) && m_EntityManager->HasComponent(i + 1, ComponentType::RECTTRANSFORM))
			{
				m_Components[i].Update(m_RectTransformManager->GetComponentPtr(i+1));
			}
		}
	}

	void TextManager::DrawTexts(sf::RenderWindow& window)
	{	
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::TEXT))
				m_Components[i].Draw(window);
		}
	}
}
