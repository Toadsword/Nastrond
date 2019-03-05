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
#include <imgui.h>
#include <utility/file_utility.h>
#include <graphics/texture.h>

namespace sfge
{
	Image& Image::operator=(const Image&)
	{
		return *this;
	}

	Image::Image() { }

	Image::~Image() { }

	void Image::Init(std::string spritePath, sf::Texture* texture, TextureId textureID)
	{
		this->sprite.setTexture(*texture);
		this->textureId = textureID;
		this->spritePath = spritePath;
	}

	void Image::Update(Vec2f position)
	{
		sprite.setPosition(position.x, position.y);
	}

	void Image::Draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

	void Image::SetColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;

		sprite.setColor(color);
	}

	void Image::SetColor(sf::Color color)
	{
		this->color = color;

		sprite.setColor(color);
	}

	sf::FloatRect Image::GetDimension() const
	{
		return sprite.getGlobalBounds();
	}

	void editor::ImageInfo::DrawOnInspector()
	{
		ImGui::Separator();
		ImGui::Text("Image");
	}

	void ImageManager::CreateComponent(json& componentJson, Entity entity)
	{
		auto* image = AddComponent(entity);

		if (CheckJsonParameter(componentJson, "spritePath", json::value_t::string))
		{			
			const auto path = componentJson["spritePath"].get<std::string>();
			
			if (FileExists(path))
			{
				sf::Texture* texture = nullptr;
				const TextureId textureId = m_TextureManager->LoadTexture(path);
				if (textureId != INVALID_TEXTURE)
				{
					texture = m_TextureManager->GetTexture(textureId);

					image->Init(path, texture, textureId);

					if (CheckJsonExists(componentJson, "color"))
						image->SetColor(componentJson["color"][0], componentJson["color"][1], componentJson["color"][2], componentJson["color"][3]);

					if (m_EntityManager->HasComponent(entity, ComponentType::RECTTRANSFORM))
						m_RectTransformManager->GetComponentPtr(entity)->SetRectDimension(image->GetDimension().width, image->GetDimension().height);
				}
				else
				{
					std::ostringstream oss;
					oss << "Texture file " << path << " cannot be loaded";
					Log::GetInstance()->Error(oss.str());
				}
			}
			else
			{
				std::ostringstream oss;
				oss << "Texture file " << path << " does not exist";
				Log::GetInstance()->Error(oss.str());
			}
		}
		else
		{
			Log::GetInstance()->Error("[Error] No Path for Sprite");
		}		
	}

	Image* ImageManager::AddComponent(Entity entity)
	{
		auto& image = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].image = &image;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		m_ConcernedEntities.push_back(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::IMAGE);
		return &image;
	}

	void ImageManager::DestroyComponent(Entity entity)
	{
		if (m_Engine.GetEntityManager()->HasComponent(entity, ComponentType::IMAGE))
		{
			RemoveConcernedEntity(entity);
			m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::IMAGE);
		}
	}

	ImageManager::ImageManager(Engine& engine):SingleComponentManager(engine)
	{
		m_RectTransformManager = engine.GetRectTransformManager();
		m_TextureManager = engine.GetGraphics2dManager()->GetTextureManager();
	}

	ImageManager::~ImageManager()
	= default;

	ImageManager& ImageManager::operator=(const Image&)
	{
		return *this;
	}

	void ImageManager::Init()
	{
		SingleComponentManager::Init();
		m_RectTransformManager = m_Engine.GetRectTransformManager();
		m_TextureManager = m_Engine.GetGraphics2dManager()->GetTextureManager();
	}

	void ImageManager::Update(float dt)
	{
		System::Update(dt);
		for (auto i = 0u; i < m_ConcernedEntities.size(); i++)
			m_Components[m_ConcernedEntities[i] - 1].Update(m_RectTransformManager->GetComponentPtr(m_ConcernedEntities[i])->Position);
	}

	void ImageManager::DrawImages(sf::RenderWindow& window)
	{
		for (auto i = 0u; i < m_ConcernedEntities.size(); i++)
			m_Components[m_ConcernedEntities[i] - 1].Draw(window);
	}

	void ImageManager::OnResize(size_t newSize)
	{
		m_Components.resize(newSize);
		m_ComponentsInfo.resize(newSize);

		for (size_t i = 0; i < newSize; ++i) {
			m_ComponentsInfo[i].SetEntity(i + 1);
			m_ComponentsInfo[i].image = &m_Components[i];
		}
	}
}
