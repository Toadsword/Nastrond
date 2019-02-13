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

#include <graphics/button.h>

namespace sfge
{
	void ButtonInfo::DrawOnInspector()
	{

	}

	void Button::Init()
	{
		
	}

	void Button::Update(RectTransform* rectTransform)
	{
		sprite.setPosition(rectTransform->Position.x, rectTransform->Position.y);
	}

	void Button::Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
	}

	void Button::SetSpriteNone(const std::string& spritePath)
	{
		spriteNonePath = spritePath;
	}

	void Button::SetSpriteHovered(const std::string& spritePath)
	{
		spriteHoveredPath = spritePath;
	}

	void Button::SetSpriteClicked(const std::string& spritePath)
	{
		spriteClickedPath = spritePath;
	}

	void Button::SetColorNone(const sf::Uint8 r, const sf::Uint8 g, const sf::Uint8 b, const sf::Uint8 a)
	{
		colorNone[0] = r;
		colorNone[1] = g;
		colorNone[2] = b;
		colorNone[3] = a;
	}

	void Button::SetColorNone(sf::Color color)
	{
		colorNone[0] = color.r;
		colorNone[1] = color.g;
		colorNone[2] = color.b;
		colorNone[3] = color.a;
	}

	void Button::SetColorHovered(const sf::Uint8 r, const sf::Uint8 g, const sf::Uint8 b, const sf::Uint8 a)
	{
		colorHovered[0] = r;
		colorHovered[1] = g;
		colorHovered[2] = b;
		colorHovered[3] = a;
	}

	void Button::SetColorHovered(sf::Color color)
	{
		colorHovered[0] = color.r;
		colorHovered[1] = color.g;
		colorHovered[2] = color.b;
		colorHovered[3] = color.a;
	}

	void Button::SetColorClicked(const sf::Uint8 r, const sf::Uint8 g, const sf::Uint8 b, const sf::Uint8 a)
	{
		colorClicked[0] = r;
		colorClicked[1] = g;
		colorClicked[2] = b;
		colorClicked[3] = a;
	}

	void Button::SetColorClicked(sf::Color color)
	{
		colorClicked[0] = color.r;
		colorClicked[1] = color.g;
		colorClicked[2] = color.b;
		colorClicked[3] = color.a;
	}

	void ButtonManager::CreateComponent(json& componentJson, Entity entity)
	{
		if (CheckJsonExists(componentJson, "spriteNone"))
		{
			m_Components[entity].SetSpriteNone(componentJson["spriteNone"]);

			if (CheckJsonExists(componentJson, "colorNone"))
				m_Components[entity].SetColorNone(componentJson["colorNone"][0], componentJson["colorNone"][1], componentJson["colorNone"][2], componentJson["colorNone"][3]);
		}
		if (CheckJsonExists(componentJson, "spriteHovered"))
		{
			m_Components[entity].SetSpriteNone(componentJson["spriteHovered"]);

			if (CheckJsonExists(componentJson, "colorHovered"))
				m_Components[entity].SetColorHovered(componentJson["colorHovered"][0], componentJson["colorHovered"][1], componentJson["colorHovered"][2], componentJson["colorHovered"][3]);
		}
		if (CheckJsonExists(componentJson, "spriteClicked"))
		{
			m_Components[entity].SetSpriteNone(componentJson["spriteClicked"]);

			if (CheckJsonExists(componentJson, "colorClicked"))
				m_Components[entity].SetColorClicked(componentJson["colorClicked"][0], componentJson["colorClicked"][1], componentJson["colorClicked"][2], componentJson["colorClicked"][3]);
		}

		/*
		* TODO:
		* Assign an action to the button (both in python and C++)
		* Python only need the module and the function name (as string) to be done with :
		*		getattr(module, 'function')
		*		Now : How to launch a function with parameter? Python function == without parameter?
		* C++ need both pointer to the function and the datas :
		*		A dictionary could be made with [key][ptr], but it needs to be done dynamically
		*		as it should be built on initialisation. (-> put int UIManager and add to it when
		*		initializing a button) How to get the ptr of the function?
		*/
	}

	Button* ButtonManager::AddComponent(Entity entity)
	{
		auto& button = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].button = &button;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::BUTTON);
		return &button;
	}

	void ButtonManager::DestroyComponent(Entity entity)
	{
		m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::BUTTON);
	}

	void ButtonManager::Init()
	{
		SingleComponentManager::Init();
		m_MouseManager = &m_Engine.GetInputManager()->GetMouseManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
	}

	void ButtonManager::Update(float dt)
	{
		bool checkClick = m_MouseManager->IsButtonUp(sf::Mouse::Button::Left);

		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::BUTTON) && m_EntityManager->HasComponent(i + 1, ComponentType::RECTTRANSFORM))
			{
				m_Components[i].Update(m_RectTransformManager->GetComponentPtr(i + 1));

				// Determine the size of the button
				const Vec2f buttonPosition = m_RectTransformManager->GetComponentPtr(i + 1)->Position;
				const Vec2f buttonSize = m_RectTransformManager->GetComponentPtr(i + 1)->halfSize;

				// Temporary value of the button state
				const ButtonState tmpState = m_Components[i].state;

				// Check if the cursor is on the button
				if (buttonPosition.x - buttonSize.x < mousePosition.x && buttonPosition.x + buttonSize.x > mousePosition.x
					&& buttonPosition.y - buttonSize.y < mousePosition.y && buttonPosition.y + buttonSize.y > mousePosition.y)
				{
					m_Components[i].state = ButtonState::HOVERED;

					if (checkClick)
					{
						m_Components[i].state = ButtonState::CLICKED;

						/*
						* Call the associated action
						*/

						checkClick = false;
					}					
				}
				else
				{
					m_Components[i].state = ButtonState::NONE;
				}

				// Change the sprite of the button if his state has changed
				if (tmpState != m_Components[i].state)
					SetSprite(&m_Components[i]);
			}
		}
	}

	void ButtonManager::DrawButtons(sf::RenderWindow& window)
	{
		/*
		 * Should be in the update but the window reference is given here
		 */
		mousePosition.x = m_MouseManager->GetLocalPosition(window).x;
		mousePosition.y = m_MouseManager->GetLocalPosition(window).y;

		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::BUTTON) && m_EntityManager->HasComponent(i + 1, ComponentType::RECTTRANSFORM))
			{
				m_Components[i].Draw(window);
			}
		}
	}

	void ButtonManager::LoadSprites()
	{
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::BUTTON))
			{
				m_Components[i].spriteNone = m_TextureManager->LoadTexture(m_Components[i].spriteNonePath);
				m_Components[i].spriteHovered = m_TextureManager->LoadTexture(m_Components[i].spriteHoveredPath);
				m_Components[i].spriteClicked = m_TextureManager->LoadTexture(m_Components[i].spriteClickedPath);
			}
		}
	}

	void ButtonManager::SetSprite(Button* button) const
	{
		switch (button->state)
		{
		case ButtonState::NONE:
			button->sprite.setTexture(*m_TextureManager->GetTexture(button->spriteNone));
			button->color.r = button->colorNone[0];
			button->color.r = button->colorNone[1];
			button->color.r = button->colorNone[2];
			button->color.r = button->colorNone[3];
			break;
		case ButtonState::HOVERED:
			button->sprite.setTexture(*m_TextureManager->GetTexture(button->spriteHovered));
			button->color.r = button->colorHovered[0];
			button->color.r = button->colorHovered[1];
			button->color.r = button->colorHovered[2];
			button->color.r = button->colorHovered[3];
			break;
		case ButtonState::CLICKED:
			button->sprite.setTexture(*m_TextureManager->GetTexture(button->spriteClicked));
			button->color.r = button->colorClicked[0];
			button->color.r = button->colorClicked[1];
			button->color.r = button->colorClicked[2];
			button->color.r = button->colorClicked[3];
			break;
		}		
	}
}
