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

//Engine
#include <engine/engine.h>
#include <graphics/sprite2d.h>
#include <engine/transform2d.h>
#include <utility/json_utility.h>
#include <utility/log.h>
#include <engine/config.h>
#include <engine/scene.h>
//Dependencies
#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>

TEST(TestAnimation, TestAnimation)
{
	sfge::Engine engine;
	engine.Init();
	json sceneJson;
	json entityJson;
	json animationJson;
	animationJson["path"] = "data/animSaves/cowboy_walk.json";
	animationJson["type"] = static_cast<int>(sfge::ComponentType::ANIMATION2D);
	entityJson["components"] = json::array({ animationJson });

	sceneJson["entities"] = json::array({ entityJson });
	sceneJson["name"] = "Test Animation";
	engine.GetSceneManager().LoadSceneFromJson(sceneJson);
	
	engine.Start();
}

