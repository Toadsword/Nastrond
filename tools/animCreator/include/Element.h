#ifndef ELEMENT_H
#define ELEMENT_H

#include <SFML\Graphics.hpp>

class Element {
public:
	Element();

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
	short indexList;
	sf::Texture texture;
};
#endif