#include <cstdlib>
#include <SFML\Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML DEMO");
	window.setFramerateLimit(60);

	sf::Clock clock;

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

	ImGui::SFML::Init(window, true);

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
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
					leftButtonDown = false;
				std::cout << leftButtonDown << "\n";
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
					leftButtonDown = false;
				std::cout << leftButtonDown << "\n";
				break;
			}
		}
		

		ImGui::SFML::Update(window, sf::seconds(clock.getElapsedTime().asSeconds()));
		ImGui::ShowDemoWindow();
		/*
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(150.0f, 100.0f), ImGuiCond_FirstUseEver);
		ImGui::Begin("sfhbfdsxxxxxhb");
		{	
			if (ImGui::Selectable("Coucou Je suis un seletable"))
				std::cout << "Je suis select \n";
		}
		ImGui::End();
		*/

		/*
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
			*/



		window.clear();
		ImGui::SFML::Render(window);
		//window.draw(playerSprite);
		window.display();
		clock.restart();
		//ImGui::EndFrame();
	}
	ImGui::Shutdown();

	system("PAUSE");
	return 0;
}
