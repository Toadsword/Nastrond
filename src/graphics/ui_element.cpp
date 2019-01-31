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

#include <graphics/ui_element.h>

namespace sfge
{
	void UIElementInfo::DrawOnInspector()
	{
		
	}

	void UIElementManager::CreateComponent(json& componentJson, Entity entity)
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

	UIElement* UIElementManager::AddComponent(Entity entity)
	{
		auto& uiElement = GetComponentRef(entity);
		m_ComponentsInfo[entity - 1].rectTransform = &uiElement;
		m_ComponentsInfo[entity - 1].SetEntity(entity);
		m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::UIELEMENT);
		return &uiElement;
	}

	void UIElementManager::DestroyComponent(Entity entity)
	{
		m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::UIELEMENT);
	}

	void UIElementManager::SetPosition(Entity entity, const Vec2f& newPosition)
	{
		GetComponentPtr(entity)->Position = newPosition;
	}

	void UIElementManager::SetScale(Entity entity, const Vec2f& newScale)
	{
		GetComponentPtr(entity)->Scale = newScale;
	}

	void UIElementManager::SetAngle(Entity entity, const float& newAngle)
	{
		GetComponentPtr(entity)->EulerAngle = newAngle;
	}

	void UIElementManager::SetSize(Entity entity, const Vec2f& newSize)
	{
		GetComponentPtr(entity)->size = newSize;
	}

	void UIElementManager::SetSprite(Entity entity, const std::string& newSpritePath)
	{
		auto component = GetComponentPtr(entity);

		component->sprite.setTexture(component->texture);
	}

	void UIElementManager::SetAction(Entity entity, const void* function, const void* data)
	{
		const auto component = GetComponentPtr(entity);

		component->action = (void*)function;
		component->data = (void*)data;
	}

	void UIElementManager::CheckMouseOnButton(Entity entity, class Vec2f& mousePosition)
	{

	}

	void UIElementManager::OnClick(Entity entity, void* function(void*), void* data)
	{
		function(data);
	}

	Vec2f UIElementManager::GetPosition(Entity entity)
	{
		return GetComponentPtr(entity)->Position;
	}

	Vec2f UIElementManager::GetScale(Entity entity)
	{
		return GetComponentPtr(entity)->Scale;
	}

	float UIElementManager::GetAngle(Entity entity)
	{
		return GetComponentPtr(entity)->EulerAngle;
	}

	Vec2f UIElementManager::GetSize(Entity entity)
	{
		return GetComponentPtr(entity)->size;
	}
}
