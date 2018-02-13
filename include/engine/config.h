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

#ifndef SFGE_CONFIG_H
#define SFGE_CONFIG_H
//Externals include
#include <SFML/System/Vector2.hpp>
//STL includes
#include <list>
#include <memory>


namespace sfge
{
/**
* \brief Used by the Engine to get a Configuration.
*/
struct Configuration
{
	/**
	 * \brief The screen resolution used for the editor
	 */
	sf::Vector2i screenResolution = sf::Vector2i(800, 600);
	/**
	 * \brief The limited framerate
	 */
	unsigned int maxFramerate = 60;
	/**
	 * \brief The list of Scene that can be loaded by the SceneManager
	 */
	std::list<std::string> scenesList;
	/**
	* \brief Used to load the overall Configuration of the GameEngine at start
	*/
	static std::unique_ptr<Configuration> LoadConfig(std::string configFilename = "data/config.json");
};
}
#endif // !CONFIG_H
