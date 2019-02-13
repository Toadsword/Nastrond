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
	void RectTransform::Update(Camera* camera)
	{
		Position = basePosition + camera->GetPosition();
	}

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
		if (CheckJsonExists(componentJson, "basePosition"))
		{
			rectTransform->basePosition.x = componentJson["basePosition"][0];
			rectTransform->basePosition.y = componentJson["basePosition"][1];
		}
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

	void RectTransformManager::Init()
	{
		SingleComponentManager::Init();
		m_CameraManager = m_Engine.GetGraphics2dManager()->GetCameraManager();
	}
	
	void RectTransformManager::Update(float dt)
	{
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::RECTTRANSFORM))
			{
				m_Components[i].Update(m_CameraManager->GetMainCamera());
			}
		}
	}
}
