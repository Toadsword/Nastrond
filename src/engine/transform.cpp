/*
 * transform.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: efarhan
 */

#include <engine/transform.h>

namespace sfge
{

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
}

void Transform2dManager::DestroyComponent(Entity entity)
{
}

TransformRequiredComponent::TransformRequiredComponent(Transform2d* transform) : 
	m_Transform(transform)
{
}
}
