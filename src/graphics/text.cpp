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
#include <imgui.h>
#include "utility/file_utility.h"

namespace sfge
{
	Text& Text::operator=(const Text&)
	{
		return *this;
	}

	void Text::Init()
	{
		
	}

	void Text::Update(Vec2f position)
	{
		text.setPosition(position);
	}

	void Text::Draw(sf::RenderWindow& window) const
	{
		window.draw(text);
	}

	void Text::SetString(std::string newText)
	{
		text.setString(newText);
	}

	void Text::SetSize(unsigned newSize)
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

	void Text::SetColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;

		text.setFillColor(color);
	}

	void Text::SetColor(sf::Color newColor)
	{
		color = newColor;

		text.setFillColor(color);
	}

	void editor::TextInfo::DrawOnInspector()
	{
		ImGui::Separator();
		ImGui::Text("Text");
	}

	TextManager::TextManager(Engine& engine):SingleComponentManager(engine)
	{
		m_RectTransformManager = engine.GetRectTransformManager();
	}

	TextManager::~TextManager()
	= default;

	TextManager& TextManager::operator=(const Text&)
	{
		return *this;
	}

	void TextManager::CreateComponent(json& componentJson, Entity entity)
	{
		if (CheckJsonParameter(componentJson, "font", json::value_t::string))
		{
			std::string path = m_Engine.GetConfig()->dataDirname + componentJson["font"].get<std::string>();

			if (FileExists(path))
			{
				if (m_Components[entity].font.loadFromFile(path))
				{
					std::string strText = "Empty text";

					if (CheckJsonExists(componentJson, "text"))
						strText = componentJson["text"].get<std::string>();

					sf::Text text(strText, m_Components[entity].font);

					m_Components[entity].text = text;

					if (CheckJsonExists(componentJson, "color"))
						m_Components[entity].SetColor(componentJson["color"][0], componentJson["color"][1], componentJson["color"][2], componentJson["color"][3]);
					if (CheckJsonExists(componentJson, "size"))
						m_Components[entity].SetSize(componentJson["size"]);
				}
				else
				{
					std::ostringstream oss;
					oss << "Font file " << path << " cannot be loaded";
					Log::GetInstance()->Error(oss.str());
				}
			}
			else
			{
				std::ostringstream oss;
				oss << "Font file " << path << " does not exist";
				Log::GetInstance()->Error(oss.str());
			}
		}
		else
		{
			Log::GetInstance()->Error("[Error] No font for Sprite");
		}
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
		m_RectTransformManager = m_Engine.GetRectTransformManager();
	}

	void TextManager::Update(float dt)
	{
		System::Update(dt);
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::TEXT) && m_EntityManager->HasComponent(i + 1, ComponentType::RECTTRANSFORM))
			{
				/* // Adjustement of the text position in order to be centered in the sprite of the entity
				 * // Currently not working, waiting on camera to finish
				sf::FloatRect rect = m_RectTransformManager->GetComponentPtr(i + 1)->rectAdjusted;

				Vec2f position = { rect.left + rect.width, rect.top - rect.height };

				m_Components[i + 1].Update(position);*/

				m_Components[i + 1].Update(m_RectTransformManager->GetComponentPtr(i+1)->Position);
			}
		}
	}

	void TextManager::DrawTexts(sf::RenderWindow& window)
	{	
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::TEXT))
				m_Components[i + 1].Draw(window);
		}
	}
}
