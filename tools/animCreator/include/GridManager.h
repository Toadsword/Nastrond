#ifndef GRID_MANAGER_H
#define GRID_MANAGER_H

#include <Grid.h>
#include <SFML\Graphics.hpp>

class GridManager{
public:
	GridManager();
	~GridManager();

	/**
	 * \brief Update is called once per frame. It calls all the grids it knows.
	 */
	void Update();

	/**
	 * \brief Draw on screen the grid
	 * \param window Window where to draw the content of the grids
	 */
	void Draw(sf::RenderWindow& window);
	
	/**
	 * \brief Create a new grid to use in the UI.
	 * \param position Top left position of the grid.
	 * \param size Size of the grid.
	 * \param numRow Num of rows of the grid.
	 * \param numCol Num of cols of the grid.
	 * \return The newly created grid.
	 */
	Grid* CreateNewGrid(sf::Vector2i position, sf::Vector2i size, short numRow, short numCol);

private:	
	std::vector<Grid*> grids;
};
#endif