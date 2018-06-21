#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

int main()
{
	sf::Window window(sf::VideoMode(800, 600), "My window");
	glEnable(GL_TEXTURE_2D);
	return 0;
}