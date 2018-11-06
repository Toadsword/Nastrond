#include <GridManager.h>
#include <SFML\Graphics.hpp>

#include <vector>

void GridManager::Update()
{
	for (auto grid : grids)
	{
		grid->Update();
	}
}

void GridManager::Draw(sf::RenderWindow& window)
{
	for (auto grid: grids)
	{
		grid->Draw(window);
	}
}

Grid* GridManager::CreateNewGrid(sf::Vector2i position, sf::Vector2i size, short numRow, short numCol)
{
	Grid* newGrid = new Grid(position, size, numRow, numCol);
	grids.push_back(newGrid);
	return newGrid;
}
