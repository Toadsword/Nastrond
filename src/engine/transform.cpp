/*
 * transform.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: efarhan
 */

#include <engine/transform.h>
#include <engine/game_object.h>
#include <utility/file_utility.h>
#include <utility/json_utility.h>
#include <engine/log.h>
//STL
#include <memory>
//Externals
#include <SFML/System.hpp>

namespace sfge
{



Transform* Transform::LoadTransform(json& componentJson, GameObject* gameObject)
{
	if (gameObject->GetTransform() != nullptr)
	{
		Log::GetInstance()->Error("[LOADING ERROR] GameObject " + gameObject->GetName() + " already has a Transform");
		return nullptr;
	}
	Transform* newTransform = new Transform(gameObject);
	Log::GetInstance()->Msg("Loading Transform");
	if (CheckJsonParameter(componentJson, "position", json::value_t::array))
	{
		auto positionJson = componentJson["position"];
		if (positionJson.size() == 2)
		{
			sf::Vector2f newPosition;
			if (IsJsonValueNumeric(positionJson[0]))
			{
				newPosition.x = positionJson[0];
			}
			if (IsJsonValueNumeric(positionJson[1]))
			{
				newPosition.y = positionJson[1];
			}
			newTransform->SetPosition(newPosition);
		}
	}
	if (CheckJsonParameter(componentJson, "scale", json::value_t::array))
	{
		auto scaleJson = componentJson["scale"];
		if (scaleJson.size() == 2)
		{
			sf::Vector2f newScale;
			if (IsJsonValueNumeric(scaleJson[0]))
			{
				newScale.x = scaleJson[0];
			}
			if (IsJsonValueNumeric(scaleJson[1]))
			{
				newScale.y = scaleJson[1];
			}
			newTransform->SetScale(newScale);
		}
	}
	if (CheckJsonExists(componentJson, "angle"))
	{
		if (IsJsonValueNumeric(componentJson["angle"]))
		{
			newTransform->SetEulerAngle(componentJson["angle"]);
		}
	}
	{
		std::ostringstream oss;
		oss << "New Transform with position: " << newTransform->GetPosition().x << ", " << newTransform->GetPosition().y<< " from: "<<componentJson;
		Log::GetInstance()->Msg(oss.str());
	}
	return newTransform;
}

void Transform::Init()
{

}

void Transform::Update(float dt)
{

}

const float Transform::GetEulerAngle()
{
	return m_EulerAngle;
}

void Transform::SetEulerAngle(float eulerAngle)
{
	this->m_EulerAngle = eulerAngle;
}

const sf::Vector2f Transform::GetPosition()
{
	return m_Position;
}

void Transform::SetPosition(sf::Vector2f position)
{
	this->m_Position = position;
}

const sf::Vector2f Transform::GetScale()
{
	return m_Scale;
}

void Transform::SetScale(sf::Vector2f scale)
{
	this->m_Scale = scale;
}

Transform::~Transform()
{

}

Transform & TransformManager::GetTransform(Entity entity)
{
	// TODO: insert return statement here
}

}
