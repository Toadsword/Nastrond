#include <cstdlib>
#include <SFML\Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "SFML DEMO");
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
			
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
			}
			
			/*
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
			*/
		}

		ImGui::SFML::Update(window, sf::seconds(clock.getElapsedTime().asSeconds()));

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		//p_open = NULL; // Don't pass our bool* to Begin

		ImGui::ShowDemoWindow();

		/*
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(900.0f, 20.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("menu", NULL, window_flags))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New")) {
						std::cout << "New select \n";
					}
					if (ImGui::MenuItem("Open", "Ctrl+O")) {
						std::cout << "Open select \n";
					}
					if (ImGui::MenuItem("Quit", "Alt+F4")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}
		ImGui::End();
		*/
		/*
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(450.0f, 300.0f), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("F1", NULL, window_flags))
		{
			if (ImGui::Selectable("Coucou Je suis un seletable"))
				std::cout << "Je suis select \n";
		}
		ImGui::End();
		ImGui::SetNextWindowPos(ImVec2(450.0f, 0.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(450.0f, 300.0f), ImGuiCond_FirstUseEver);
		if(ImGui::Begin("F2", NULL, window_flags))
		{	
			if (ImGui::Selectable("Coucou Je suis un seletable"))
				std::cout << "Je suis select \n";
		}
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0.0f, 300.0f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(900, 300.0f), ImGuiCond_FirstUseEver);
		if(ImGui::Begin("F3", NULL, window_flags))
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
	}
	ImGui::Shutdown();

	system("PAUSE");
	return 0;
}
