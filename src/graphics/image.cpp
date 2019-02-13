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

#include <graphics/image.h>

namespace sfge
{
	void Image::Init()
	{
		
	}

	void Image::Update(RectTransform* rectTransform)
	{
		sprite.setPosition(rectTransform->Position.x, rectTransform->Position.y);
	}

	void Image::Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

	void Image::SetSprite(std::string spritePath)
	{
		this->spritePath = spritePath;
	}

	void Image::SetColor(const sf::Uint8 r, const sf::Uint8 g, const sf::Uint8 b, const sf::Uint8 a)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
	}

	void Image::SetColor(sf::Color color)
	{
		this->color[0] = color.r;
		this->color[1] = color.g;
		this->color[2] = color.b;
		this->color[3] = color.a;
	}

	void ImageInfo::DrawOnInspector()
	{
		
	}

	void ImageManager::CreateComponent(json& componentJson, Entity entity)
	{
		if (CheckJsonExists(componentJson, "spritePath"))
		{
			m_Components[entity].SetSprite(componentJson["spritePath"]);

			if (CheckJsonExists(componentJson, "color"))
				m_Components[entity].SetColor(componentJson["color"][0], componentJson["color"][1], componentJson["color"][2], componentJson["color"][3]);
		}
	}

	Image* ImageManager::AddComponent(Entity entity)
	{
		auto& image = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].image = &image;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::IMAGE);
		return &image;
	}

	void ImageManager::DestroyComponent(Entity entity)
	{
		m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::IMAGE);
	}

	void ImageManager::Init()
	{
		SingleComponentManager::Init();
	}

	void ImageManager::Update(float dt)
	{
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::IMAGE) && m_EntityManager->HasComponent(i + 1, ComponentType::RECTTRANSFORM))
			{
				m_Components[i].Update(m_RectTransformManager->GetComponentPtr(i + 1));
			}
		}
	}

	void ImageManager::DrawImages(sf::RenderWindow& window)
	{
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::IMAGE))
			{
				m_Components[i].Draw(window);
			}
		}
	}

	void ImageManager::LoadSprites()
	{
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::BUTTON))
			{
				m_Components[i].textureId = m_TextureManager->LoadTexture(m_Components[i].spritePath);
			}
		}
	}

	void ImageManager::SetSprite(Image* image) const
	{
		image->sprite.setTexture(*m_TextureManager->GetTexture(image->textureId));

		sf::Color color;

		color.r = image->color[0];
		color.g = image->color[1];
		color.b = image->color[2];
		color.a = image->color[3];

		image->sprite.setColor(color);
	}
}
