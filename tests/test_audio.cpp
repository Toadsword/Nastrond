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
#include <engine/engine.h>
#include <engine/config.h>
#include <gtest/gtest.h>
#include <audio/sound.h>
#include <SFML/Audio.hpp>

TEST(TestAudio, Sound)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->devMode = false;
	config->windowLess = true;
	engine.Init(std::move(config));

	json sceneJson;
	json entityJson;
	json soundJson;
	soundJson["path"] = "data/sounds/doorClose_1.ogg";
	soundJson["type"] = static_cast<int>(sfge::ComponentType::SOUND);
	

	json playSoundScriptJson;
	playSoundScriptJson["script_path"] = "scripts/play_sound.py";
	playSoundScriptJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);
	entityJson["components"] = json::array({ soundJson, playSoundScriptJson });

	json fakeEntityJson;
	json fakeSoundJson;
	fakeSoundJson["path"] = "fake/path/prout.wav";
	fakeSoundJson["type"] = static_cast<int>(sfge::ComponentType::SOUND);
	fakeEntityJson["components"] = json::array({ fakeSoundJson });


	sceneJson["entities"] = json::array({ entityJson, fakeEntityJson });
	sceneJson["name"] = "Test Sound";
	engine.GetSceneManager().LoadSceneFromJson(sceneJson);

	engine.Start();
}
