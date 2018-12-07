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
#include <graphics/animation2d.h>
#include <utility/file_utility.h>

#include <utility/log.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <engine/transform2d.h>

#include <imgui.h>
#include <imgui-SFML.h>

namespace sfge
{

Animation::Animation() :
	TransformRequiredComponent(nullptr), Offsetable(sf::Vector2f())
{
}

Animation::Animation(Transform2d* transform, sf::Vector2f offset)
	: TransformRequiredComponent(transform), Offsetable(offset)
{
}
void Animation::Draw(sf::RenderWindow& window)
{
	/*animation.setPosition(m_GameObject->GetTransform()->GetPosition()+m_Offset);
	animation.setScale(m_GameObject->GetTransform()->GetScale());
	animation.setRotation(m_GameObject->GetTransform()->GetEulerAngle());*/
	
	window.draw(sprite);
}

void Animation::SetAnimation(std::vector<AnimationFrame> newFrameList, float newSpeed, bool newIsLooped)
{
	this->frameList = newFrameList;
	this->speed = newSpeed;
	this->isLooped = newIsLooped;
	//this->currentFrame = newFrameList.begin();
	this->currentFrameKey = 0;
}

void Animation::Init()
{
}

void Animation::Update(float dt)
{
	timeSinceChangedFrame += dt;
	
	if(timeSinceChangedFrame >= speed && !frameList.empty())
	{
		timeSinceChangedFrame = 0;

		currentFrameKey++;
		bool changed = false;
		for(auto frame : frameList)
		{
			if(frame.key == currentFrameKey)
			{
				sprite.setTextureRect(frame.textureRect);
				sprite.setTexture(*frame.texture);
				changed = true;
				break;
			}
		}
		if(!changed)
		{
			currentFrameKey = 0;
			sprite.setTextureRect(frameList[currentFrameKey].textureRect);
			sprite.setTexture(*frameList[currentFrameKey].texture);
		}

		//With Iterator (Doesn't work)
		/* 
		if (currentFrame == frameList.end())
			currentFrame = frameList.begin();
		else
			++currentFrame;
		sprite.setTextureRect((*currentFrame).textureRect);
		sprite.setTexture(*(*currentFrame).texture);
		*/
	}
	
	Vec2f pos = m_Offset;
	if(m_Transform != nullptr)
	{
		pos += m_Transform->Position;
	}
	sprite.setPosition(pos);
}


void editor::AnimationInfo::DrawOnInspector()
{
	ImGui::Separator();
	ImGui::Text("Animation");
	ImGui::LabelText("Animation name", "%s", name.c_str());
	//ImGui::InputInt("Texture Id", (int*)&textureId);
	if(animation)
	{
		float offset[2] =
		{
			animation->GetOffset().x,
			animation->GetOffset().y
		};
		ImGui::InputFloat2("Offset", offset);
	}
}



Animation* AnimationManager::AddComponent(Entity entity)
{
	auto& animation = GetComponentRef(entity);
	auto& animationInfo = GetComponentInfo(entity);

	animation.SetTransform(m_Transform2dManager->GetComponentPtr(entity));
	animationInfo.animation = &animation;

	m_EntityManager->AddComponentType(entity, ComponentType::ANIMATION2D);
	return &animation;
}

void AnimationManager::Init()
{
	SingleComponentManager::Init();
	m_GraphicsManager = m_Engine.GetGraphics2dManager();
	m_Transform2dManager = m_Engine.GetTransform2dManager();
}

void AnimationManager::Update(float dt)
{

	rmt_ScopedCPUSample(Animation2dUpdate,0)
	for(auto i = 0u; i < m_Components.size();i++)
	{
		if(m_EntityManager->HasComponent(i + 1, ComponentType::ANIMATION2D))
		{
			m_Components[i].Update(dt);
		}
	}
}


void AnimationManager::DrawAnimations(sf::RenderWindow &window)
{

	rmt_ScopedCPUSample(Animation2dDraw,0)
	for (auto i = 0u; i<m_Components.size();i++)
	{
		if(m_EntityManager->HasComponent(i + 1, ComponentType::ANIMATION2D))
			m_Components[i].Draw(window);
	}
	
}

void AnimationManager::Reset()
{
}

void AnimationManager::Collect()
{
	
}

void AnimationManager::CreateComponent(json& componentJson, Entity entity)
{

	if (CheckJsonParameter(componentJson, "path", json::value_t::string))
	{
		std::string path = componentJson["path"];
		if (!FileExists(path))
		{
			std::ostringstream oss;
			oss << "Texture file " << path << " does not exist";
			Log::GetInstance()->Error(oss.str());
			return;
		}
		auto framesInfosPtr = LoadJson(path);
		if (framesInfosPtr == nullptr)
		{
			std::ostringstream oss;
			oss << "Texture file " << path << " cannot be loaded";
			Log::GetInstance()->Error(oss.str());
			return;
		}

		auto & newAnimation = m_Components[entity - 1];
		auto & newAnimationInfo = m_ComponentsInfo[entity - 1];
		
		newAnimation.SetTransform(m_Transform2dManager->GetComponentPtr(entity));
		
		std::string name = "";
		if (CheckJsonParameter(*framesInfosPtr, "name", json::value_t::string))
		{
			name = (*framesInfosPtr)["name"].get<std::string>();
			newAnimationInfo.name = name;
		}

		float speed = 0.1f;
		if (CheckJsonParameter(*framesInfosPtr, "speed", json::value_t::number_unsigned))
		{
			speed = (*framesInfosPtr)["speed"].get<float>() / 1000.0f;
			newAnimationInfo.speed = speed;
		}

		bool isLooped = false;
		if (CheckJsonParameter(*framesInfosPtr, "isLooped", json::value_t::boolean))
		{
			isLooped = (*framesInfosPtr)["isLooped"].get<bool>();
			newAnimationInfo.isLooped = isLooped;
		}

		if(CheckJsonParameter((*framesInfosPtr), "frames", json::value_t::array))
		{
			std::vector<AnimationFrame> newFrameList;
			auto* textureManager = m_GraphicsManager->GetTextureManager();
			for (auto& frameJson : (*framesInfosPtr)["frames"])
			{
				AnimationFrame newFrame;
				if (CheckJsonExists(frameJson, "filename"))
				{
					newFrame.filename = frameJson["filename"].get<std::string>();
					std::string texturePath = ANIM_FOLER + name + "/" + newFrame.filename;
					const TextureId textureId = textureManager->LoadTexture(texturePath);
					
					if (textureId != INVALID_TEXTURE)
					{
						newFrame.texture = textureManager->GetTexture(textureId);
					}
					else
					{
						std::ostringstream oss;
						oss << "Texture file " << texturePath << " cannot be loaded";
						Log::GetInstance()->Error(oss.str());
					}
				}

				if (CheckJsonExists(frameJson, "key"))
				{
					newFrame.key = frameJson["key"].get<short>();
				}

				if (CheckJsonExists(frameJson, "position") && CheckJsonExists(frameJson, "size"))
				{
					sf::Vector2f position(frameJson["position"]["x"].get<int>(), frameJson["position"]["y"].get<int>());
					sf::Vector2f size(frameJson["size"]["x"].get<int>(), frameJson["size"]["y"].get<int>());

					sf::IntRect textureRect = sf::IntRect(position.x, position.y, size.x, size.y);
					newFrame.textureRect = textureRect;
				}

				newFrameList.push_back(newFrame);
			}
			newAnimation.SetAnimation(newFrameList, speed, isLooped);
		}
		if (CheckJsonParameter(componentJson, "layer", json::value_t::number_integer))
		{
			newAnimation.SetLayer(componentJson["layer"]);
		}
	}
	else
	{
		Log::GetInstance()->Error("[Error] No Path for Animation");
	}
	
}

void AnimationManager::DestroyComponent(Entity entity)
{
	(void) entity;
}
}
