/*
 * transform.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: efarhan
 */

#include <engine/transform.h>
#include <imgui.h>

namespace sfge
{
void editor::TransformInfo::DrawOnInspector()
{
	
	float pos[2] = { transform->Position.x, transform->Position.y };
	ImGui::Separator();
	ImGui::Text("Transform");
	ImGui::InputFloat2("Position", pos);
	float scale[2] = { transform->Scale.x, transform->Scale.y };
	ImGui::InputFloat2("Scale", scale);
	ImGui::InputFloat("Angle", &transform->EulerAngle);
}

void Transform2dManager::CreateComponent(json& componentJson, Entity entity)
{

	Log::GetInstance()->Msg("Create component Transform");
	auto& transform = GetComponent(entity);
	if (CheckJsonExists(componentJson, "position"))
		transform.Position = GetVectorFromJson(componentJson, "position");
	if (CheckJsonExists(componentJson, "scale"))
		transform.Scale = GetVectorFromJson(componentJson, "scale");
	if (CheckJsonExists(componentJson, "angle") && CheckJsonNumber(componentJson, "angle"))
		transform.EulerAngle = componentJson["angle"];
	m_ComponentsInfo[entity - 1].transform = &transform;
}

void Transform2dManager::DestroyComponent(Entity entity)
{
}

TransformRequiredComponent::TransformRequiredComponent(Transform2d* transform) : 
	m_Transform(transform)
{
}
}
