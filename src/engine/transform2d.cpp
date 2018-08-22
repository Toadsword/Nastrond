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

Transform2dManager::Transform2dManager(Engine& engine):
	ComponentManager<sfge::Transform2d, sfge::editor::Transform2dInfo>(), 
	System(engine),
	ResizeObserver()
{

	m_Engine.GetEntityManager().AddObserver(this);
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

void Transform2dManager::OnResize(size_t new_size)
{
	m_Components.resize(new_size);
	m_ComponentsInfo.resize(new_size);
}

TransformRequiredComponent::TransformRequiredComponent(Transform2d* transform) : 
	m_Transform(transform)
{
}
}
