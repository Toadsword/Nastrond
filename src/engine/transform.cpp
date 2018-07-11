/*
 * transform.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: efarhan
 */

#include <engine/transform.h>

namespace sfge
{
bool Transform2dManager::CreateComponent()
{
	return false;
}
bool Transform2dManager::DestroyComponent()
{
	return false;
}
Transform2dManager::Transform2dManager(Engine& engine) :
	Module(engine)
{
	
}

}
