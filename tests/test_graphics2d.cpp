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

TEST(Graphics2d, TestCamera)
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
	sf::RenderWindow window(sf::VideoMode(800, 600), "Test Camera");
	sf::View mainCamera;

	mainCamera.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
	mainCamera.rotate(0);

	window.setView(mainCamera);

	sf::Vector2u sizeWindow = window.getSize();
	sf::Vector2i positionWindow = window.getPosition();

	sf::Vector2f PositionCamera = sf::Vector2f(0, 0);
	float velocityCamera = 50.0f;


	// Create a clock for measuring the time elapsed
	sf::Clock clock;

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

		//Systeme curseur
		float deltaTime = clock.restart().asSeconds();
		sf::Vector2i positionCursor = sf::Mouse::getPosition();
		positionWindow = window.getPosition();

		if (positionCursor.x >= positionWindow.x && positionCursor.x <= positionWindow.x + sizeWindow.x
			&& positionCursor.y >= positionWindow.y && positionCursor.y <= positionWindow.y + sizeWindow.y)
		{

			if (positionCursor.y < positionWindow.y + sizeWindow.y / 100 * 10)
			{
				std::cout << PositionCamera.x << std::endl;
				PositionCamera += sf::Vector2f(0, velocityCamera*deltaTime);
			}

			if (positionCursor.y > positionWindow.y + sizeWindow.y / 100 * 90)
			{
				std::cout << PositionCamera.x << std::endl;
				PositionCamera -= sf::Vector2f(0, velocityCamera*deltaTime);
			}

			if (positionCursor.x < positionWindow.x + sizeWindow.x / 100 * 10)
			{
				std::cout << PositionCamera.x << std::endl;
				PositionCamera += sf::Vector2f(velocityCamera*deltaTime, 0);
			}

			if (positionCursor.x > positionWindow.x + sizeWindow.x / 100 * 90)
			{
				std::cout << PositionCamera.x << std::endl;
				PositionCamera -= sf::Vector2f(velocityCamera*deltaTime, 0);
			}
		}

		//on met la modification
		mainCamera.setCenter(PositionCamera);
		//on l'envoie à la vue
		window.setView(mainCamera);
		
		// clear the window with black color
		window.clear(sf::Color::Black);

		window.draw(sprite);

		// end the current frame
		window.display();
	}
	engine.Destroy();
}

