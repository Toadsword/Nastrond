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

#include "engine/ui.h"

namespace  sfge
{
	/************************************RectTransform Part*********************************/
	void RectTransformManager::CreateComponent(json& componentJson, Entity entity)
	{
		auto* rectTransform = AddComponent(entity);
		if (CheckJsonExists(componentJson, "position"))
			rectTransform->Position = GetVectorFromJson(componentJson, "position");
		if (CheckJsonExists(componentJson, "scale"))
			rectTransform->Scale = GetVectorFromJson(componentJson, "scale");
		if (CheckJsonExists(componentJson, "angle") && CheckJsonNumber(componentJson, "angle"))
			rectTransform->EulerAngle = componentJson["angle"];
		if (CheckJsonExists(componentJson, "size"))
			rectTransform->size = GetVectorFromJson(componentJson, "size");
		if (CheckJsonExists(componentJson, "sprite"))
		{
			if (rectTransform->texture.loadFromFile(componentJson["sprite"]))
				rectTransform->sprite.setTexture(rectTransform->texture);
		}

		// Set default color value
		rectTransform->color.r = 0;
		rectTransform->color.g = 0;
		rectTransform->color.b = 0;
		rectTransform->color.a = 1;

		if (CheckJsonExists(componentJson, "red"))
			rectTransform->color.r = componentJson["red"];
		if (CheckJsonExists(componentJson, "green"))
			rectTransform->color.g = componentJson["green"];
		if (CheckJsonExists(componentJson, "blue"))
			rectTransform->color.b = componentJson["blue"];
		if (CheckJsonExists(componentJson, "alpha"))
			rectTransform->color.a = componentJson["alpha"];

		/*
		 * TODO:
		 * Assign an action to the rectTransform (both in python and C++
		 */
	}

	RectTransform* RectTransformManager::AddComponent(Entity entity)
	{
		auto& rectTransform = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].rectTransform = &rectTransform;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::RECTTRANSFORM);
		return &rectTransform;
	}

	void RectTransformManager::DestroyComponent(Entity entity)
	{
		m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::RECTTRANSFORM);
	}

	void RectTransformManager::SetPosition(Entity entity, const Vec2f& newPosition)
	{
		GetComponentPtr(entity)->Position = newPosition;
	}

	void RectTransformManager::SetScale(Entity entity, const Vec2f& newScale)
	{
		GetComponentPtr(entity)->Scale = newScale;
	}

	void RectTransformManager::SetAngle(Entity entity, const float& newAngle)
	{		
		GetComponentPtr(entity)->EulerAngle = newAngle;
	}

	void RectTransformManager::SetSize(Entity entity, const Vec2f& newSize)
	{
		GetComponentPtr(entity)->size = newSize;
	}

	void RectTransformManager::SetSprite(Entity entity, const std::string& newSpritePath)
	{
		auto component = GetComponentPtr(entity);

		component->sprite.setTexture(component->texture);
	}

	void RectTransformManager::SetAction(Entity entity, const void* function, const void* data)
	{
		const auto component = GetComponentPtr(entity);

		component->action = (void*)function;
		component->data = (void*)data;
	}

	void RectTransformManager::CheckMouseOnButton(Entity entity, class Vec2f& mousePosition)
	{
		
	}

	void RectTransformManager::OnClick(Entity entity, void* function(void*), void* data)
	{
		function(data);
	}

	Vec2f RectTransformManager::GetPosition(Entity entity)
	{
		return GetComponentPtr(entity)->Position;
	}

	Vec2f RectTransformManager::GetScale(Entity entity)
	{
		return GetComponentPtr(entity)->Scale;
	}

	float RectTransformManager::GetAngle(Entity entity)
	{
		return GetComponentPtr(entity)->EulerAngle;
	}

	Vec2f RectTransformManager::GetSize(Entity entity)
	{
		return GetComponentPtr(entity)->size;
	}
	/***************************************************************************************/

	/************************************TextManager Part***********************************/
	void TextManager::CreateComponent(json& componentJson, Entity entity)
	{
		auto* text = AddComponent(entity);
		if (CheckJsonExists(componentJson, "text"))
			text->text.setString(componentJson["text"]);
		if (CheckJsonExists(componentJson, "size"))
			text->text.setCharacterSize(componentJson["size"]);
		if (CheckJsonExists(componentJson, "font"))
		{
			if(text->font.loadFromFile(componentJson["font"]))
				text->text.setFont(text->font);
		}

		// Set default color value
		text->color.r = 0;
		text->color.g = 0;
		text->color.b = 0;
		text->color.a = 1;

		if (CheckJsonExists(componentJson, "red"))
			text->color.r = componentJson["red"];
		if (CheckJsonExists(componentJson, "green"))
			text->color.g = componentJson["green"];
		if (CheckJsonExists(componentJson, "blue"))
			text->color.b = componentJson["blue"];
		if (CheckJsonExists(componentJson, "alpha"))
			text->color.a = componentJson["alpha"];
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

	void TextManager::SetString(Entity entity, const std::string& newText)
	{
		GetComponentPtr(entity)->text.setString(newText);
	}

	void TextManager::SetFont(Entity entity, const std::string& newFontPath)
	{
		auto component = GetComponentPtr(entity);

		if (component->font.loadFromFile(newFontPath))
			component->text.setFont(component->font);
	}

	void TextManager::SetSize(Entity entity, const unsigned newSize)
	{
		GetComponentPtr(entity)->text.setCharacterSize(newSize);
	}

	std::string TextManager::GetString(Entity entity)
	{
		return GetComponentPtr(entity)->text.getString();
	}

	std::string TextManager::GetFont(Entity entity)
	{
		return GetComponentPtr(entity)->text.getFont()->getInfo().family;
	}

	unsigned TextManager::GetSize(Entity entity)
	{
		return GetComponentPtr(entity)->text.getCharacterSize();
	}
	/***************************************************************************************/

	/*******************************ImageManager Part***************************************/
	/*void ImageManager::CreateComponent(json& componentJson, Entity entity)
	{
		auto* image = AddComponent(entity);

		if (CheckJsonExists(componentJson, "sprite"))
			image->sprite.set
	}

	/***************************************************************************************/
	void UIManager::Init()
	{
		m_EntityManager = m_Engine.GetEntityManager();
		m_RectTransformManager = m_Engine.GetRectTransformManager();
		m_TextManager = m_Engine.GetTextManager();
		/*m_ImageManager = m_Engine.GetImageManager();
		m_ButtonManager = m_Engine.GetButtonManager();*/
	}

	void UIManager::Update(float dt)
	{
		
	}

	void UIManager::FixedUpdate()
	{
		
	}

	void UIManager::Draw()
	{
		
	}
}
