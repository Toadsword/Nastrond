/*
 * transform.h
 *
 *  Created on: Feb 6, 2018
 *      Author: efarhan
 */

#ifndef SFGE_TRANSFORM_H_
#define SFGE_TRANSFORM_H_

#include <engine/entity.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

namespace sfge
{


struct Transform2d
{
	sf::Vector2f m_Position = sf::Vector2f();
	sf::Vector2f m_Scale = sf::Vector2f(1.0f,1.0f);
	float m_EulerAngle = 0.0f;

};

class Transform2dManager
{
	Transform2d& GetTransform(Entity entity);
private:
	std::vector<Transform2d> m_Transforms;
};

}

#endif /* INCLUDE_ENGINE_TRANSFORM_H_ */
