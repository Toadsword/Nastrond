#include <cstdlib>
#include <SFML\Graphics.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML DEMO");
	window.setFramerateLimit(60);


	sf::Texture spriteSheet;
	sf::Sprite playerSprite;
	int frameCounter = 0;
	int numRows = 4;
	int numCols = 3;
	int curColumn = 0;
	int curRow = 0;

	if (!spriteSheet.loadFromFile("data/sprites/pepperSprite.png"))
	{
		std::cout << "Fail to load file\n";
	}
	else
	{
		playerSprite.setTexture(spriteSheet);
	}

	bool leftButtonDown = false;
	sf::Mouse myMouse;

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			/*
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonPressed:
					std::cout << "Mouse button pressed. \n";

					event.
					switch(event.key.code)
					{
						case sf::Mouse::Left:
							std::cout << "Left Key pressed \n";
							break;
						case sf::Mouse::Right:
							std::cout << "Right Key pressed \n";
					}
					break;
			}
			*/

			switch(event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
					leftButtonDown = true;
				std::cout << leftButtonDown << "\n";
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
					leftButtonDown = false;
				std::cout << leftButtonDown << "\n";
				break;
			}
		}
		window.clear();

		frameCounter++;
		if(frameCounter == 15)
		{
			curColumn = (curColumn + 1) % numCols;
			if (curColumn == 0)
				curRow = (curRow + 1) % numRows;
			frameCounter = 0;
		}

		playerSprite.setTextureRect(sf::IntRect(24 * curColumn, 32 * curRow, 24, 32));

		if (leftButtonDown)
			playerSprite.setPosition(myMouse.getPosition(window).x, myMouse.getPosition(window).y);

		if (playerSprite.getGlobalBounds().contains(myMouse.getPosition(window).x, myMouse.getPosition(window).y))
			std::cout << "Mouse inside sprite. \n";


		window.draw(playerSprite);
		window.display();
	}


	system("PAUSE");
	return 0;
}
