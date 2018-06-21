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

#ifndef SFGE_COMPONENT_H
#define SFGE_COMPONENT_H

#include <engine/game_object.h>
#include <utility/json_utility.h>

//Externals includes
#include <SFML/System.hpp>


namespace sfge
{
class Transform;

enum class ComponentType
{
	NONE,
	TRANSFORM,
	SPRITE,
	SHAPE,
	PYCOMPONENT,
	BODY2D,
	COLLIDER,
	SOUND
};

/**
 * \brief A Component is attached to a GameObject
 */
class Component
{
public:
	/**
	 * \brief Constructor of Component takes the parent GameObject as reference
	 * \param gameObject The parent GameObject
	 */
	Component(GameObject* gameObject);
	virtual ~Component() = default;
	/**
	 *
	 * \brief Static method to laod a generic Component. It calls more concrete known component types
	 * \param engine Game engine reference to use other modules
	 * \param componentJson Content of the component such as types, values
	 * \param gameObject GameObject that the Component is going to be attached to
	 * \return A pointer to the Component created
	 */
	static Component* LoadComponent(Engine& engine, json& componentJson, GameObject* gameObject);
	virtual void Init() = 0;
	/**
	* \brief Update the Component
	* \param dt Delta time since last frame
	*/
	virtual void Update(float dt) = 0;

	GameObject* GetGameObject();

	const std::string& GetName();
	void SetName(const std::string& name);

	ComponentType GetComponentType();

	virtual void OnTriggerEnter(Collider* collider);
	virtual void OnCollisionEnter(Collider* collider);

	virtual void OnTriggerExit(Collider* collider);
	virtual void OnCollisionExit(Collider* collider);

	static unsigned int incrementalComponentId;
protected:

	ComponentType m_ComponentType = ComponentType::NONE;
	unsigned int m_ComponentId = 0U;
	std::string m_Name = "";
	/**
	* \brief The pointer to the GameObject the Component is attached to
	*/
	GameObject* m_GameObject;

};

class Offsetable
{
public:
	Offsetable();
	sf::Vector2f GetOffset();
	void SetOffset(sf::Vector2f newOffset);
protected:
	sf::Vector2f m_Offset = sf::Vector2f();
};


}
#endif
