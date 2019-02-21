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

#include <gtest/gtest.h>
#include "engine/engine.h"
#include "engine/component.h"
#include "graphics/texture.h"
#include <graphics/graphics2d.h>

TEST(Graphics2d, TestSpriteAnimation)
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
	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);

	engine.Start();
}


TEST(Graphics2d, TestSprite)
{
	sfge::Engine engine;
	engine.Init();
	json sceneJson;
	json entityJson;
	json spriteJson;
	spriteJson["path"] = "data/sprites/roguelikeDungeon_transparent.png";
	spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
	entityJson["components"] = json::array({ spriteJson });

	json fakeEntityJson;
	json fakeSpriteJson;
	fakeSpriteJson["path"] = "fake/path/prout.jpg";
	fakeSpriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
	fakeEntityJson["components"] = json::array({ fakeSpriteJson });


	sceneJson["entities"] = json::array({ entityJson, fakeEntityJson });
	sceneJson["name"] = "Test Sprite";
	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);

	engine.Start();
}


TEST(Graphics2d, TestTexture)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->devMode = false;
	config->windowLess = true;
	engine.Init(std::move(config));

	auto* textureManager = engine.GetGraphics2dManager()->GetTextureManager();

	const std::string goodTextPath = "data/sprites/other_play.png";
	const std::string badTextPath = "fake/path/prout.jpg";
	const std::string badTextPathWithoutExtension = "fake/path/prout";

	const sfge::TextureId goodTextId = textureManager->LoadTexture(goodTextPath);
	const sfge::TextureId badTextId = textureManager->LoadTexture(badTextPath);
	const sfge::TextureId badTextExtId = textureManager->LoadTexture(badTextPathWithoutExtension);

	sf::Sprite sprite;

	ASSERT_EQ(badTextId, sfge::INVALID_TEXTURE);
	ASSERT_EQ(badTextExtId, sfge::INVALID_TEXTURE);
	ASSERT_NE(goodTextId, sfge::INVALID_TEXTURE);

	if (badTextId != sfge::INVALID_TEXTURE)
	{
		sfge::Log::GetInstance()->Msg("Loading Bad File");
		sprite.setTexture(*textureManager->GetTexture(badTextId));
	}
	else
	{
		std::ostringstream oss;
		oss << "Bad file: " << badTextPath << " could not be loaded";
		sfge::Log::GetInstance()->Error(oss.str());
	}
	if (goodTextId != sfge::INVALID_TEXTURE)
	{

		sfge::Log::GetInstance()->Msg("Loading Good File");
		sprite.setTexture(*textureManager->GetTexture(goodTextId));
	}
	else
	{
		std::ostringstream oss;
		oss << "Bad file: " << goodTextPath << " could not be loaded";
		sfge::Log::GetInstance()->Error(oss.str());
	}



	// create the window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Test Sprite");

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event{};
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		window.draw(sprite);

		// end the current frame
		window.display();
	}
	engine.Destroy();
}



TEST(Graphics2d, TestPyCamera)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->gravity = b2Vec2(0,0);
	engine.Init(std::move(config));

	json sceneJson;
	json entityJson;

	json spriteJson;
	spriteJson["path"] = "data/sprites/other_play.png";
	spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
	json transformJson =
	{
		{ "position", { 0, 0 } },
		{ "angle", 0.0f },
		{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
	};
	entityJson["components"] = json::array({ spriteJson, transformJson });

	json entityJson2;
	json CameraJson;
	CameraJson["type"] = static_cast<int>(sfge::ComponentType::CAMERA);
	json pyCameraJson;
	pyCameraJson["script_path"] = "scripts/camera_manager.py";
	pyCameraJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);
	entityJson2["components"] = json::array({ CameraJson, pyCameraJson });


	sceneJson["entities"] = json::array({ entityJson2, entityJson });
	sceneJson["name"] = "Test Camera";
	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);

	engine.Start();

}

TEST(Graphics2d, TestPyCameraFullScreen)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->styleWindow = sf::Style::Fullscreen;
	engine.Init(std::move(config));

	json sceneJson;
	json entityJson;

	json spriteJson;
	spriteJson["path"] = "data/sprites/other_play.png";
	spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
	entityJson["components"] = json::array({ spriteJson });

	json entityJson2;
	json CameraJson;
	CameraJson["type"] = static_cast<int>(sfge::ComponentType::CAMERA);
	json pyCameraJson;
	pyCameraJson["script_path"] = "scripts/camera_manager.py";
	pyCameraJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);
	entityJson2["components"] = json::array({ CameraJson, pyCameraJson });

	sceneJson["entities"] = json::array({ entityJson2, entityJson });
	sceneJson["name"] = "Test Fullscreen";
	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);

	engine.Start();
}
TEST(Graphics2d, TestQuadtreeStar)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->gravity = b2Vec2(0, 0);
	engine.Init(std::move(config));
	json sceneJson;

	json entitiesArray = json::array();
	json entityJson2;
	json CameraJson;
	CameraJson["type"] = static_cast<int>(sfge::ComponentType::CAMERA);
	json pyCameraJson;
	pyCameraJson["script_path"] = "scripts/camera_manager.py";
	pyCameraJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);
	entityJson2["components"] = json::array({ CameraJson, pyCameraJson });
	entitiesArray.push_back(entityJson2);

	for (int i = 1; i < 10; i++) {
		json entityJson;

		json spriteJson;
		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		json transformJson =
		{
			//std::rand() % static_cast<int>
			{ "position", { -i * 75,  0 } },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);

		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		transformJson =
		{
			//std::rand() % static_cast<int>
			{ "position", { i * 75,  0} },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);

		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		transformJson =
		{
			//std::rand() % static_cast<int>
			{ "position", { 0, i * 75} },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);

		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		transformJson =
		{
			//std::rand() % static_cast<int>
			{ "position", { 0, -i * 75} },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);

		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		transformJson =
		{
			//std::rand() % static_cast<int>
			{ "position", {  i * 75, i * 75} },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);

		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		transformJson =
		{
			//std::rand() % static_cast<int>
			{ "position", { -i * 75, -i * 75} },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);

		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		transformJson =
		{
			//std::rand() % static_cast<int>
			{ "position", { -i * 75, i * 75} },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);

		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		transformJson =
		{
			//std::rand() % static_cast<int>
			{ "position", { i * 75, -i * 75} },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);
	}

	sceneJson["entities"] = entitiesArray;
	sceneJson["name"] = "Test Quadtree";
	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);
	engine.Start();
}

TEST(Graphics2d, TestQuadtree)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->gravity = b2Vec2(0, 0);
	engine.Init(std::move(config));
	json sceneJson;

	json entitiesArray = json::array();
	json entityJson2;
	json CameraJson;
	CameraJson["type"] = static_cast<int>(sfge::ComponentType::CAMERA);
	json pyCameraJson;
	pyCameraJson["script_path"] = "scripts/camera_manager.py";
	pyCameraJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);
	entityJson2["components"] = json::array({ CameraJson, pyCameraJson });
	entitiesArray.push_back(entityJson2);



	for (int i = 0; i < 0; i++) {
		json entityJson;

		json spriteJson;
		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		json transformJson =
		{
			{ "position", { std::rand() % static_cast<int>(10000 * 2) - 10000,  std::rand() % static_cast<int>(10000 * 2) - 10000 } },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);
	}
	sceneJson["entities"] = entitiesArray;
	sceneJson["name"] = "Test Quadtree";
	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);
	engine.Start();
}

TEST(Graphics2d, TestHashTable)
{
	sfge::Engine engine;
	auto config = std::make_unique<sfge::Configuration>();
	config->gravity = b2Vec2(0, 0);
	engine.Init(std::move(config));
	json sceneJson;

	json entitiesArray = json::array();
	json entityJson2;
	json CameraJson;
	CameraJson["type"] = static_cast<int>(sfge::ComponentType::CAMERA);
	json pyCameraJson;
	pyCameraJson["script_path"] = "scripts/camera_manager.py";
	pyCameraJson["type"] = static_cast<int>(sfge::ComponentType::PYCOMPONENT);
	entityJson2["components"] = json::array({ CameraJson, pyCameraJson });
	entitiesArray.push_back(entityJson2);



	for (int i = 0; i < 750; i++) {
		json entityJson;

		json spriteJson;
		spriteJson["path"] = "data/sprites/other_play_x50.png";
		spriteJson["type"] = static_cast<int>(sfge::ComponentType::SPRITE2D);
		json transformJson =
		{
			{ "position", { std::rand() % static_cast<int>(4000),  std::rand() % static_cast<int>(4000)} },
			{ "angle", 0.0f },
			{ "squale", { 0.5f, 0.5f} },
			{ "type", static_cast<int>(sfge::ComponentType::TRANSFORM2D) },
		};
		entityJson["components"] = json::array({ spriteJson, transformJson });

		entitiesArray.push_back(entityJson);
	}
	sceneJson["entities"] = entitiesArray;
	sceneJson["name"] = "Test Hash Table ";
	engine.GetSceneManager()->LoadSceneFromJson(sceneJson);
	engine.Start();
	engine.GetGraphics2dManager()->GetSpriteManager()->GetHashTable();
}
//--gtest_filter=Graphics2d.TestQuadtree