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

#include <graphics/rect_transform.h>
#include <imgui.h>

namespace sfge
{
	RectTransform& RectTransform::operator=(const RectTransform&)
	{
		return *this;
	}

	RectTransform::RectTransform()
	= default;

	RectTransform::~RectTransform() { }

	void RectTransform::Init(Vec2f position)
	{
		basePosition = position;
	}

	void RectTransform::Update(Camera* camera)
	{
		if(camera == nullptr)
			Position = basePosition;
		else
			Position = basePosition + camera->GetPosition();

		rect.left = Position.x;
		rect.top = Position.y;
	}

	bool RectTransform::Contains(float x, float y)
	{
		float minX = std::min(rect.left, static_cast<float>(rect.left + rect.width));
		float maxX = std::max(rect.left, static_cast<float>(rect.left + rect.width));
		float minY = std::min(rect.top, static_cast<float>(rect.top + rect.height));
		float maxY = std::max(rect.top, static_cast<float>(rect.top + rect.height));

		return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY);
	}

	void RectTransform::SetPosition(Vec2f position)
	{
		Position = position;
	}

	void RectTransform::SetPosition(float x, float y)
	{
		Position = { x, y };
	}

	void RectTransform::SetRectDimension(float width, float height)
	{
		rect.width = width;
		rect.height = height;
	}

	Vec2f RectTransform::GetPosition() const
	{
		return Position;
	}

	sf::FloatRect RectTransform::GetRect() const
	{
		return rect;
	}

	void editor::RectTransformInfo::DrawOnInspector()
	{
		float pos[2] = { rectTransform->GetPosition().x, rectTransform->GetPosition().y };
		float rectPos[4] = { rectTransform->GetRect().left, rectTransform->GetRect().top, rectTransform->GetRect().width, rectTransform->GetRect().height };
		ImGui::Separator();
		ImGui::Text("RectTransform");
		ImGui::InputFloat2("Position", pos);
		float scale[2] = { rectTransform->Scale.x, rectTransform->Scale.y };
		ImGui::InputFloat2("Scale", scale);
		ImGui::InputFloat("Angle", &rectTransform->EulerAngle);
		ImGui::InputFloat4("Rect", rectPos);
	}

	RectTransformManager::RectTransformManager(Engine& engine):SingleComponentManager(engine)
	{
		m_CameraManager = engine.GetGraphics2dManager()->GetCameraManager();
	}

	RectTransformManager::~RectTransformManager()
	= default;

	RectTransformManager& RectTransformManager::operator=(const RectTransform&)
	{
		return *this;
	}

	void RectTransformManager::CreateComponent(json& componentJson, Entity entity)
	{
		auto& rectTransform = m_Components[entity - 1];
		if (CheckJsonExists(componentJson, "position"))
			rectTransform.Position = GetVectorFromJson(componentJson, "position");
		if (CheckJsonExists(componentJson, "scale"))
			rectTransform.Scale = GetVectorFromJson(componentJson, "scale");
		if (CheckJsonExists(componentJson, "angle") && CheckJsonNumber(componentJson, "angle"))
			rectTransform.EulerAngle = componentJson["angle"];
		if (CheckJsonExists(componentJson, "basePosition"))
		{
			Vec2f basePosition;
			basePosition.x = componentJson["basePosition"][0];
			basePosition.y = componentJson["basePosition"][1];
			rectTransform.Init(basePosition);
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
		System::Update(dt);
		for (auto i = 0u; i < m_Components.size(); i++)
		{
			if (m_EntityManager->HasComponent(i + 1, ComponentType::RECTTRANSFORM))
			{
				m_Components[i].Update(m_CameraManager->GetMainCamera());
			}
		}
	}

	void RectTransformManager::OnResize(size_t newSize)
	{
		m_Components.resize(newSize);
		m_ComponentsInfo.resize(newSize);

		for (size_t i = 0; i < newSize; ++i) {
			m_ComponentsInfo[i].SetEntity(i + 1);
			m_ComponentsInfo[i].rectTransform = &m_Components[i];
		}
	}
}
