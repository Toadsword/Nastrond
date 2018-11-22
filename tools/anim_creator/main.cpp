/*
MIT License

Copyright (c) 2018 SAE Institute Switzerland AG

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

----------------------------
Author : Duncan Bourquard (bourquardduncan@gmail.com)
Date : November 2018
Project : AnimationTool for SFGE
*/

#include <Engine.h>
/*
#include <TextureManager.h>
#include <AnimationManager.h>
#include <Utilities.h>
*/
int main()
{
	/*
	TextureManager textureMan;
	textureMan.LoadTexture("D:/Images/Artworks/Touhou/BvGapbeIcAArSAf.png", 48, 48);
	textureMan.LoadTexture("D:/Images/Artworks/Touhou/BvGapbeIcAArSAf.png", 48, 48, 1, 0);
	TextureInfos* textu1 = textureMan.GetTextureFromId(1);
	TextureInfos* textu2 = textureMan.GetTextureFromId(2);

	AnimationManager anim;
	anim.SetFPSSpeed(20);
	anim.AddOrInsertKey(1, textu1->id);
	anim.AddOrInsertKey(10, textu2->id);

	Utilities::ExportToJson(anim, textureMan.GetAllTextures(), "AnimTest");
	return 0;
	*/
	{
		Engine engine;

		engine.Init();
		engine.Start();

	}
	/*
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
	*/


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
#ifdef WIN32
	system("PAUSE");
#endif
	return EXIT_SUCCESS;
}
