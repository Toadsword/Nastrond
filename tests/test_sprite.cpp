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
#include <graphics/sprite.h>
#include <engine/game_object.h>
#include <engine/transform.h>
#include <utility/json_utility.h>
#include <engine/log.h>
#include <engine/modules.h>

//Dependencies
#include <SFML/Graphics.hpp>


int main()
{
	sfge::Engine engine;
	engine.Init(true);

	json gameObjectJson;
	json spriteJson;
	spriteJson["path"] = "data/sprites/boss_01_dialog_pose_001_b.png";
	spriteJson["type"] = (int)sfge::ComponentType::SPRITE;
	gameObjectJson["components"] = json::array({ spriteJson });

	sfge::GameObject* gameObject = sfge::GameObject::LoadGameObject(engine, gameObjectJson);
	gameObject->GetTransform()->SetPosition(sf::Vector2f(400, 300));
	auto sprite = gameObject->GetComponent<sfge::Sprite>();
	if (sprite != nullptr)
	{
		// create the window
		sf::RenderWindow window(sf::VideoMode(800, 600), "Test Sprite");


		sf::Clock clock;
		// run the program as long as the window is open
		while (window.isOpen())
		{
			sf::Time dt = clock.restart();
			// check all the window's events that were triggered since the last iteration of the loop
			sf::Event event;
			while (window.pollEvent(event))
			{
				// "close requested" event: we close the window
				if (event.type == sf::Event::Closed)
					window.close();
			}
			gameObject->GetTransform()->SetPosition(gameObject->GetTransform()->GetPosition() + sf::Vector2f(10.f, 10.f)*dt.asSeconds());
			// clear the window with black color
			window.clear(sf::Color::Black);

			sprite->Draw(window);

			// end the current frame
			window.display();
		}
	}
	else
	{
		sfge::Log::GetInstance()->Error("COULD NOT LOAD SPRITE");
	}
	engine.Destroy();
#if WIN32
	system("pause");
#endif
	return EXIT_SUCCESS;
}
