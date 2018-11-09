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

void Transform2dManager::Init()
{
}

Transform2d* Transform2dManager::AddComponent(Entity entity)
{

	auto& transform = GetComponentRef(entity);
	m_ComponentsInfo[entity - 1].transform = &transform;

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

TransformRequiredComponent::TransformRequiredComponent(Transform2d* transform) : 
	m_Transform(transform)
{
}
void TransformRequiredComponent::SetTransform (Transform2d *transform)
{
	m_Transform = transform;
}
Transform2d *TransformRequiredComponent::GetTransform ()
{
	return m_Transform;
}
}
