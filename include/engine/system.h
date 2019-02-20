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

#ifndef SFGE_SYSTEM_H
#define SFGE_SYSTEM_H

#include <utility/json_utility.h>

namespace sfge
{

class Engine;
/**
* \brief Systems are classes used by the Engine to init and update features, new features can be added through PySystem
*/
class System
{
public:
	System(Engine& engine);

	virtual ~System() = default;
	System& operator=(const System&) = delete;
	System ( System && ) = default; //move constructor
	System ( const System & ) = delete; //delete copy constructor
	//explicit System(const System & system);

	/**
	* \brief Called to initialize the module
	*/
	virtual void Init();
	/**
	* \brief Called every frame to update the module
	* \param dt The delta time since last frame
	*/
	virtual void Update(float dt) {(void) dt;}
	/**
	* \brief Called directly after the physics finished his job
	*/
	virtual void FixedUpdate() {}

	virtual void Draw(){}
	/**
	* \brief Used instead of the destructor to delete all heap created structure and finalize
	*/
	virtual void Destroy();
	/**
	* \brief Called before we load a scene
	*/
	virtual void Clear() {}
	/**
	* \brief Called after we load a scene
	*/
	virtual void Collect() {}

	/**
	 * \brief Called when the engine saves the game
	 */

	void SetEnable(bool enable);
	bool GetEnable() const;

	Engine& GetEngine() const;
	bool GetInitlialized() const;
protected:
	bool m_Enable = true;
	Engine& m_Engine;
	bool m_Initialized = false;
};
}
#endif //SFGE_SYSTEM_H
