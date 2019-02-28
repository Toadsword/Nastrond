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

#include <utility/log.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <engine/transform2d.h>

#include <imgui.h>
#include <imgui-SFML.h>

namespace sfge
{

Sprite::Sprite() : Offsetable(sf::Vector2f())
{
	is_visible = true;
}

Sprite::Sprite(Transform2d* transform, sf::Vector2f offset) : Offsetable(offset)
{
	is_visible = true;
}
void Sprite::Draw(sf::RenderWindow& window)
{
	/*sprite.setPosition(m_GameObject->GetTransform()->GetPosition()+m_Offset);
	sprite.setScale(m_GameObject->GetTransform()->GetScale());
	sprite.setRotation(m_GameObject->GetTransform()->GetEulerAngle());*/
	
	window.draw(sprite);
}
const sf::Texture* Sprite::GetTexture()
{
	return sprite.getTexture();
}
void Sprite::SetTexture(sf::Texture* newTexture)
{
	sprite.setTexture(*newTexture);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height) / 2.0f);
}

void Sprite::Init()
{
	is_visible = true;
}

void Sprite::Update(Transform2d* transform)
{
	auto pos = m_Offset;

	if(transform != nullptr)
	{
		pos += transform->Position;
		sprite.setRotation(transform->EulerAngle);
		sprite.setScale(transform->Scale.x, transform->Scale.y);
	}
	sprite.setPosition(pos);
}


void editor::SpriteInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Sprite");
	ImGui::LabelText("Texture Path", "%s", texturePath.c_str());
	ImGui::InputInt("Texture Id", (int*)&textureId);
	if(sprite)
	{
		float offset[2] =
		{
			sprite->GetOffset().x,
			sprite->GetOffset().y
		};
		ImGui::InputFloat2("Offset", offset);
		ImGui::Checkbox("Is Visible", &sprite->is_visible);
	}
}


void SpriteManager::Init()
{
	SingleComponentManager::Init();
	m_GraphicsManager = m_Engine.GetGraphics2dManager();
	m_Transform2dManager = m_Engine.GetTransform2dManager();
}

Sprite* SpriteManager::AddComponent(Entity entity)
{
	auto& sprite = GetComponentRef(entity);
	auto& spriteInfo = GetComponentInfo(entity);

	m_Components[entity - 1] = sprite;

	/*
	 * Component optimisation addition
	 */
	m_ConcernedEntities.push_back(entity);

	spriteInfo.sprite = &sprite;
	spriteInfo.SetEntity(entity);

	m_EntityManager->AddComponentType(entity, ComponentType::SPRITE2D);
	return &sprite;
}

void SpriteManager::Update(float dt)
{
	(void) dt;
	rmt_ScopedCPUSample(SpriteUpdate,0)
	for(auto i = 0u; i < m_Components.size();i++)
	{
		if(m_EntityManager->HasComponent(i+1, ComponentType::SPRITE2D) && m_EntityManager->HasComponent(i + 1, ComponentType::TRANSFORM2D))
		{
			m_Components[i].Update(m_Transform2dManager->GetComponentPtr(i + 1));
		}
	}
}

void SpriteManager::DrawSprites(sf::RenderWindow &window)
{

	rmt_ScopedCPUSample(SpriteDraw,0)
	for (auto i = 0u; i < m_Components.size();i++)
	{
		if(m_EntityManager->HasComponent(i + 1, ComponentType::SPRITE2D))
			if(m_Components[i].is_visible)
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
	auto & newSprite = m_Components[entity - 1];
	auto & newSpriteInfo = m_ComponentsInfo[entity - 1];
	if (CheckJsonParameter(componentJson, "path", json::value_t::string))
	{
		std::string path = componentJson["path"].get<std::string>();
		newSpriteInfo.texturePath = path;
		sf::Texture* texture = nullptr;
		if (FileExists(path))
		{
			auto* textureManager = m_GraphicsManager->GetTextureManager();
			const TextureId textureId = textureManager->LoadTexture(path);
			if (textureId != INVALID_TEXTURE)
			{
				/*{
					std::ostringstream oss;
					oss << "Loading Sprite with Texture at: " << path << " with texture id: " << textureId;
					sfge::Log::GetInstance()->Msg(oss.str());
				}*/
				texture = textureManager->GetTexture(textureId);
				newSprite.SetTexture(texture);
				newSpriteInfo.textureId = textureId;
				newSpriteInfo.sprite = &newSprite;
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
}

void SpriteManager::DestroyComponent(Entity entity)
{
	(void) entity;
}

json SpriteManager::Save()
{
	json j;
	for (auto i = 0u; i < m_ComponentsInfo.size(); i++)
	{
		if (m_EntityManager->HasComponent(i + 1, ComponentType::SPRITE2D))
		{	
			j[i]["type"] = static_cast<int>(ComponentType::SPRITE2D);
			j[i]["path"] = m_ComponentsInfo[i].texturePath;
			j[i]["is_visible"] = m_Components[i].is_visible;
		}
	}
	return j;
}

void SpriteManager::OnResize(size_t new_size)
{
	m_Components.resize(new_size);
	m_ComponentsInfo.resize(new_size);

	for (size_t i = 0; i < new_size; ++i) {
		m_ComponentsInfo[i].SetEntity(i + 1);
		m_ComponentsInfo[i].sprite = &m_Components[i];
	}
}
}
