#include <Grid.h>

#include <SFML\Graphics.hpp>

Grid::Grid()
{
	position = sf::Vector2i(0, 0);
	size = sf::Vector2i(0, 0);
	numCol = 0;
	numRow = 0;
}

Grid::~Grid()
{
}

void Grid::Update()
{
}

void Grid::Draw(sf::RenderWindow& window)
{

}
