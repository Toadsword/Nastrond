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

#include <graphics/graphics.h>
#include <graphics/sprite.h>
#include <graphics/texture.h>
#include <utility/file_utility.h>

#include <engine/log.h>

#include <engine/config.h>
#include <engine/transform.h>

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
void Sprite::SetTextureId(TextureId textureId)
{
	m_TextureId = textureId;
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



SpriteManager::SpriteManager(Engine& engine, GraphicsManager& graphicsManager) : 
	Module(engine), m_GraphicsManager(graphicsManager)
{
	m_TransformManagerPtr = m_GraphicsManager.GetEngine().GetTransform2dManager();
	m_EntityManagerPtr = m_GraphicsManager.GetEngine().GetEntityManager();
}

void SpriteManager::Update(sf::Time dt)
{

}


void SpriteManager::Draw(sf::RenderWindow& window)
{
	if (auto entityManager = m_EntityManagerPtr.lock())
	{
		for (int i = 0; i<m_Components.size();i++)
		{
			if(entityManager->HasComponent(i+1,SPRITE))
				m_Components[i].Draw(window);
		}
	}
}
/*
void SpriteManager::LoadSprite(json& componentJson, Sprite* newSprite)
{
	if (newSprite == nullptr)
		return;

	if(CheckJsonParameter(componentJson, "path", json::value_t::string))
	{
		std::string path = componentJson["path"].get<std::string>();
		sf::Texture* texture = nullptr;
		if (FileExists(path))
		{
			unsigned int textureId = m_GraphicsManager.GetTextureManager()->LoadTexture(path);
			if (textureId != 0)
			{
				{
					std::ostringstream oss;
					oss << "Loading Sprite with Texture at: " << path << " with texture id: " << textureId;
					sfge::Log::GetInstance()->Msg(oss.str());
				}
				texture = m_GraphicsManager.GetTextureManager()->GetTexture(textureId);
				newSprite->SetTexture(texture);
				newSprite->SetTextureId(textureId);
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
		newSprite->SetLayer(componentJson["layer"]);
	}
	m_Sprites.push_back(newSprite);
}
*/
void SpriteManager::Reset()
{
}

void SpriteManager::Collect()
{
	
}

void SpriteManager::CreateComponent(json& componentJson, Entity entity)
{
}

void SpriteManager::DestroyComponent(Entity entity)
{
}

}
