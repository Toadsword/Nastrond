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

#include <graphics/graphics2d.h>
#include <graphics/sprite2d.h>
#include <graphics/texture.h>
#include <utility/file_utility.h>

#include <engine/log.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <engine/transform2d.h>

#include <imgui.h>
#include <imgui-SFML.h>

namespace sfge
{
void Sprite::Draw(sf::RenderWindow& window)
{
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height) / 2.0f);
	/*sprite.setPosition(m_GameObject->GetTransform()->GetPosition()+m_Offset);
	sprite.setScale(m_GameObject->GetTransform()->GetScale());
	sprite.setRotation(m_GameObject->GetTransform()->GetEulerAngle());*/
	
	window.draw(sprite);
}
void Sprite::SetTexture(sf::Texture* newTexture)
{
	sprite.setTexture(*newTexture);
}

Sprite::Sprite() : 
	TransformRequiredComponent(nullptr), Offsetable(sf::Vector2f())
{
}

Sprite::Sprite(Transform2d* transform, sf::Vector2f offset)
	: TransformRequiredComponent(transform), Offsetable(offset)
{
}

void Sprite::Init()
{
}


void editor::SpriteInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Sprite");
	ImGui::LabelText("Texture Path", texturePath.c_str());
	ImGui::InputInt("Texture Id", (int*)&textureId);
	if(sprite)
	{
		float offset[2] =
		{
			sprite->GetOffset().x,
			sprite->GetOffset().y
		};
		ImGui::InputFloat2("Offset", offset);
	}
}

SpriteManager::SpriteManager(Engine& engine):
	ComponentManager<Sprite, editor::SpriteInfo>(),
	System(engine),
	m_GraphicsManager(m_Engine.GetGraphicsManager()),
	m_Transform2dManager(m_Engine.GetTransform2dManager()),
	m_EntityManager(m_Engine.GetEntityManager())
{
}

void SpriteManager::Init()
{
	System::Init();
	m_EntityManager.AddObserver(this);
}

void SpriteManager::Update(float dt)
{

}


void SpriteManager::Draw(sf::RenderWindow& window)
{
	
	for (int i = 0; i<m_Components.size();i++)
	{
		if(m_EntityManager.HasComponent(i+1, ComponentType::SPRITE2D))
			m_Components[i].Draw(window);
	}
	
}

void SpriteManager::Reset()
{
}

void SpriteManager::Collect()
{
	
}

void SpriteManager::CreateComponent(json& componentJson, Entity entity)
{

	Sprite newSprite;
	editor::SpriteInfo newSpriteInfo;
	if (CheckJsonParameter(componentJson, "path", json::value_t::string))
	{
		std::string path = componentJson["path"].get<std::string>();
		newSpriteInfo.texturePath = path;
		sf::Texture* texture = nullptr;
		if (FileExists(path))
		{
			auto& textureManager = m_GraphicsManager.GetTextureManager();
			const TextureId textureId = textureManager.LoadTexture(path);
			if (textureId != INVALID_TEXTURE)
			{
				{
					std::ostringstream oss;
					oss << "Loading Sprite with Texture at: " << path << " with texture id: " << textureId;
					sfge::Log::GetInstance()->Msg(oss.str());
				}
				texture = textureManager.GetTexture(textureId);
				newSprite.SetTexture(texture);
				newSpriteInfo.textureId = textureId;
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
	if (CheckJsonParameter(componentJson, "layer", json::value_t::number_integer))
	{
		newSprite.SetLayer(componentJson["layer"]);
	}
	m_Components[entity - 1] = newSprite;
	m_ComponentsInfo[entity - 1] = newSpriteInfo;
}

void SpriteManager::DestroyComponent(Entity entity)
{
}

void SpriteManager::OnResize(size_t new_size)
{
	m_Components.resize(new_size);
	m_ComponentsInfo.resize(new_size);
}
}
