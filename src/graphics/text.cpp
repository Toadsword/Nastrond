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
#include <utility/file_utility.h>

namespace sfge
{
	Text& Text::operator=(const Text&)
	{
		return *this;
	}

	Text::Text() { }

	Text::~Text() { }

	void Text::Init(const std::string text, const std::string fontPath, const sf::Uint8 color[4], const unsigned characterSize)
	{
		if (FileExists(fontPath))
		{
			if (this->font.loadFromFile(fontPath))
			{
				sf::Text iText(text, this->font);

				this->text = iText;

				this->SetColor(color[0], color[1], color[2], color[3]);

				this->SetSize(characterSize);
			}
			else
			{
				std::ostringstream oss;
				oss << "Font file " << fontPath << " cannot be loaded";
				Log::GetInstance()->Error(oss.str());
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "Font file " << fontPath << " does not exist";
			Log::GetInstance()->Error(oss.str());
		}
	}

	void Text::Update(const Vec2f position)
	{
		text.setPosition(position);
	}

	void Text::Draw(sf::RenderWindow& window) const
	{
		window.draw(text);
	}

	void Text::SetTextString(const std::string newText)
	{
		text.setString(newText);
	}

	void Text::SetSize(const unsigned newSize)
	{
		text.setCharacterSize(newSize);
	}

	std::string Text::GetTextString() const
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
		auto* text = AddComponent(entity);

		if (CheckJsonParameter(componentJson, "font", json::value_t::string))
		{
			// Prepare default values			
			std::string strText = "Empty text";
			sf::Uint8 color[4] = { 255,255,255,255 };
			unsigned size = 32;

			// Get values
			std::string path = m_Engine.GetConfig()->dataDirname + componentJson["font"].get<std::string>();
			if (CheckJsonExists(componentJson, "text"))
				strText = componentJson["text"].get<std::string>();
			if(CheckJsonExists(componentJson, "color"))
			{
				color[0] = componentJson["color"][0];
				color[1] = componentJson["color"][1];
				color[2] = componentJson["color"][2];
				color[3] = componentJson["color"][3];
			}
			if (CheckJsonExists(componentJson, "size"))
				size = componentJson["size"];

			// Initialize text
			text->Init(strText, path, color, size);
		}
		else
		{
			Log::GetInstance()->Error("[Error] No font for text");
		}
	}

	Text* TextManager::AddComponent(Entity entity)
	{
		auto& text = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].text = &text;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		/*
		* Component optimisation addition
		*/
		m_ConcernedEntities.push_back(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::TEXT);
		return &text;
	}

	void TextManager::DestroyComponent(Entity entity)
	{
		RemoveConcernedEntity(entity);
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
				m_Components[i].Update(m_RectTransformManager->GetComponentPtr(i+1)->Position);
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

	void TextManager::OnResize(size_t newSize)
	{
		m_Components.resize(newSize);
		m_ComponentsInfo.resize(newSize);

		for (size_t i = 0; i < newSize; ++i) {
			m_ComponentsInfo[i].SetEntity(i + 1);
			m_ComponentsInfo[i].text = &m_Components[i];
		}
	}
}
