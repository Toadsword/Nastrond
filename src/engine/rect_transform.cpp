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

#include <engine/rect_transform.h>

namespace sfge
{
	void RectTransformInfo::DrawOnInspector()
	{

	}

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
}
