/*
 * transform.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: efarhan
 */


#include <engine/transform2d.h>
#include <imgui.h>
#include <engine/engine.h>
namespace sfge
{
void editor::Transform2dInfo::DrawOnInspector()
{
	
	float pos[2] = { transform->Position.x, transform->Position.y };
	ImGui::Separator();
	ImGui::Text("Transform");
	ImGui::InputFloat2("Position", pos);
	float scale[2] = { transform->Scale.x, transform->Scale.y };
	ImGui::InputFloat2("Scale", scale);
	ImGui::InputFloat("Angle", &transform->EulerAngle);
}


Transform2d* Transform2dManager::AddComponent(Entity entity)
{

	auto& transform = GetComponentRef(entity);
	m_ComponentsInfo[entity - 1].transform = &transform;
	m_ComponentsInfo[entity - 1].SetEntity(entity);
	m_Engine.GetEntityManager()->AddComponentType(entity, ComponentType::TRANSFORM2D);
	return &transform;
}

void Transform2dManager::CreateComponent(json& componentJson, Entity entity)
{

	//Log::GetInstance()->Msg("Create component Transform");
	auto* transform = AddComponent(entity);
	if (CheckJsonExists(componentJson, "position"))
		transform->Position = GetVectorFromJson(componentJson, "position");
	if (CheckJsonExists(componentJson, "scale"))
		transform->Scale = GetVectorFromJson(componentJson, "scale");
	if (CheckJsonExists(componentJson, "angle") && CheckJsonNumber(componentJson, "angle"))
		transform->EulerAngle = componentJson["angle"];
}

void Transform2dManager::DestroyComponent(Entity entity)
{
	m_Engine.GetEntityManager()->RemoveComponentType(entity, ComponentType::TRANSFORM2D);
}


void Transform2dManager::Update(float dt) {
    System::Update(dt);
    for(auto& transform : m_Components)
	{
    	if(transform.EulerAngle > 180.0f)
		{
    		transform.EulerAngle -= 360.0f;
		}

		if(transform.EulerAngle < -180.0f)
		{
			transform.EulerAngle += 360.0f;
		}
	}
}

void Transform2dManager::OnResize(size_t newSize) {
	m_Components.resize(newSize);
	m_ComponentsInfo.resize(newSize);

	for (size_t i = 0; i < newSize; ++i) {
		m_ComponentsInfo[i].SetEntity(i + 1);
		m_ComponentsInfo[i].transform = &m_Components[i];
	}
}
}
