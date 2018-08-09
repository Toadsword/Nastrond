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
#include <graphics/sprite2d.h>
#include <graphics/texture.h>
#include <engine/log.h>
int main()
{
	sfge::TextureManager textureManager;

	unsigned int goodTextId = textureManager.LoadTexture("data/sprites/boss_01_dialog_pose_001_b.png");
	unsigned int badTextId = textureManager.LoadTexture("prout.jpg");

	sf::Sprite sprite;
	if (badTextId != 0)
	{
		sfge::Log::GetInstance()->Msg("Loading Bad File");
		sprite.setTexture(*textureManager.GetTexture(badTextId));
	}
	if (goodTextId != 0)
	{

		sfge::Log::GetInstance()->Msg("Loading Good File");
		sprite.setTexture(*textureManager.GetTexture(goodTextId));
	}

	// create the window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Test Sprite");

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
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
#if WIN32
	system("pause");
#endif
	return EXIT_SUCCESS;
}
