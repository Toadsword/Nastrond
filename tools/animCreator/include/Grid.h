#ifndef GRID_H
#define GRID_H

#include <Element.h>

#include <vector>
#include <SFML\Graphics.hpp>

class Grid {
public:
	Grid();
	Grid(sf::Vector2i position, sf::Vector2i size, short numRow, short numCol) :
		position(position), size(size), numRow(numRow), numCol(numCol) {};
	~Grid();

	/**
	 * \brief Update the grid upon changes
	 */
	void Update();

	/**
	 * \brief Draw the grid on window
	 * \param window Window where to draw the content of the grid
	 */
	void Draw(sf::RenderWindow& window);

private:
	std::vector<Element> elements;

	sf::Vector2i position;
	sf::Vector2i size;
	short numRow;
	short numCol;
};
#endif