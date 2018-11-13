#include <Engine.h>

int main()
{
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

	system("PAUSE");
	return 0;
}
