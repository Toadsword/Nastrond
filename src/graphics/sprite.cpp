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
void Sprite::SetTextureId(unsigned int textureId)
{
	m_TextureId = textureId;
}

void Sprite::Init()
{
}



SpriteManager::SpriteManager(GraphicsManager& graphicsManager): m_GraphicsManager(graphicsManager)
{
}

void SpriteManager::Update()
{
}

void SpriteManager::Draw(sf::RenderWindow& window)
{
	for (auto& sprite : m_Components)
	{
		sprite.Draw(window);
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

bool SpriteManager::CreateComponent()
{
	return false;
}

bool SpriteManager::DestroyComponent()
{
	return false;
}
}
