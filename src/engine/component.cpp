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

#include <engine/component.h>
#include <engine/transform.h>
#include <graphics/sprite.h>
#include <graphics/shape.h>
#include <python/pycomponent.h>
#include <physics/body2d.h>
#include <physics/collider.h>
#include <audio/audio.h>

#include <engine/log.h>

namespace sfge
{


unsigned int Component::incrementalComponentId = 1U;

Component::Component(GameObject* gameObject) :
	m_GameObject(gameObject)
{
}

	


	Component* Component::LoadComponent(Engine& engine, json& componentJson, GameObject* gameObject)
{
	Component* component = nullptr;
	std::string componentName = "";
	unsigned int componentId = incrementalComponentId;
	if(CheckJsonParameter(componentJson, "name", json::value_t::string))
	{
		std::ostringstream oss;
		oss << componentJson["name"];
		componentName = oss.str();
	}
	else
	{
		std::ostringstream oss;
		oss << "Component " << componentId;
		componentName = oss.str();
	}

	Log::GetInstance()->Msg("Loading component: " + componentName);
	if (CheckJsonNumber(componentJson, "type"))
	{
		ComponentType componentType = (ComponentType)componentJson["type"];

		switch(componentType)
		{
		case ComponentType::TRANSFORM:
			component = Transform::LoadTransform(componentJson, gameObject);
			if (component != nullptr)
			{
				componentName = "Transform";
				gameObject->SetTransform(dynamic_cast<Transform*>(component));
			}
			else
			{
				Log::GetInstance()->Error("Loaded Transform was nullptr");
			}
			break;
		case ComponentType::SPRITE:
			Log::GetInstance()->Msg("Loading Sprite Component");
			component = Sprite::LoadSprite(engine,  componentJson, gameObject);
			break;
		case ComponentType::PYCOMPONENT:
			component = PyComponent::LoadPythonScript(engine,  componentJson, gameObject);
			break;
		case ComponentType::SHAPE:
			component = Shape::LoadShape(engine, componentJson, gameObject);
			break;
		case ComponentType::BODY2D:
			component = Body2d::LoadBody2d(engine, gameObject, componentJson);
			break;
		case ComponentType::COLLIDER:
			component = Collider::LoadCollider(engine, gameObject, componentJson);
			break;
		case ComponentType::SOUND:
			component = Sound::LoadSound(engine, componentJson, gameObject);
			break;
		default:
			break;
		}
		if (component != nullptr)
		{
			component->SetName(componentName);
			component->m_ComponentId = incrementalComponentId;
			component->m_ComponentType = componentType;

			incrementalComponentId++;
		}
		else
		{
			Log::GetInstance()->Error("Undefined type for component");
		}
	}
	else
	{
		Log::GetInstance()->Error("No type defined for component");
	}
	return component;
}

GameObject* Component::GetGameObject()
{
	return m_GameObject;
}

const std::string & Component::GetName()
{
	return m_Name;
}

void Component::SetName(const std::string & name)
{
	m_Name = name;
}

ComponentType Component::GetComponentType()
{
	return m_ComponentType;
}

void Component::OnTriggerEnter(Collider * collider)
{
}

void Component::OnCollisionEnter(Collider * collider)
{
}

void Component::OnTriggerExit(Collider * collider)
{
}

void Component::OnCollisionExit(Collider * collider)
{
}

Offsetable::Offsetable()
{
}

sf::Vector2f Offsetable::GetOffset()
{
	return m_Offset;
}

void Offsetable::SetOffset(sf::Vector2f newOffset)
{
	m_Offset = newOffset;
}

}
