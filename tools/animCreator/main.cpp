#include <Engine.h>
#include <Utilities.h>

int main()
{
	std::map<short, short> anim;
	std::vector<TextureInfos*> textures;
	Utilities::ExportToJson(anim, textures);
	return 0;
	
	Engine* engine = new Engine();

	engine->Init();
	engine->Start();

	delete(engine);

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

	system("PAUSE");
	return 0;
}
