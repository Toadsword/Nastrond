#include <Graphics/hashmap2d.h>
#include <iostream>

namespace sfge {

	HashTable::HashTable(int argCellSize)
	{
		cellSize = argCellSize;
		sparse = true;
	}

	HashTable::HashTable(int argCellSize, int argWidth, int argHeight)
	{
		cellSize = argCellSize;
		width = argWidth;
		height = argHeight;
	}

	HashTable::~HashTable(void)
	{

	}

	void HashTable::CheckCamera(AABBHash aabbCamera)
	{
		//bool result = boundary.intersectsCamera(camera);
		//is_visible = result;

		int i = 0;
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				sf::Vector2f key(x, y);
				if (table.count(key) > 0)
				{
					bool isVisible = aabbCamera.intersectsCamera(AABBHash(PointHash(nullptr, sf::Vector2f(x * width+ width/2, y * height+ height/2)), PointHash(nullptr, sf::Vector2f(width / 2, height / 2))));
						for (PointHash* element : table[key])
						{
							(*element->is_visible) = isVisible;
						}
					
				}else
				{
					//std::cout << "Elements Count: " << table.count(key) << "; ";
				}
				i++;
			}
		}

		std::cout << std::endl;
	}

	void HashTable::Draw(sf::RenderWindow* window)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++) {
				sf::VertexArray vertices(sf::LineStrip, 5);
				vertices[0].position = sf::Vector2f(j * width, i * height);
				vertices[1].position = sf::Vector2f(j * width + width, i * height);
				vertices[2].position = sf::Vector2f(j * width + width, i * height + height);
				vertices[3].position = sf::Vector2f(j * width, i * height + height);
				vertices[4].position = sf::Vector2f(j * width, i * height);
				window->draw(vertices);
			}
		}
	}


	sf::Vector2f HashTable::Hash(const sf::Vector2f& position)
	{
		sf::Vector2f returnPoint;
		returnPoint.x = (int)(position.x / (width));
		returnPoint.y = (int)(position.y / (height));
		return returnPoint;
		//return Vector2i((int)(position.x / cellSize), (int)(position.y / cellSize));
	}

	void HashTable::InsertObjectPoint(PointHash* element)
	{
		sf::Vector2f key = Hash(element->position);
		if (table.count(key) > 0)
		{
			table[key].push_back(element);
		}
		else
		{
			std::vector<PointHash*> newList;
			table[key] = newList;
			table[key].push_back(element);
		}
	}

	MyHashCompare::MyHashCompare()
	{
		
	}

	size_t MyHashCompare::operator()(const sf::Vector2f & Key) const
	{
		return size_t();
	}

	bool MyHashCompare::operator()(const sf::Vector2f& Key1, const sf::Vector2f& Key2) const
	{
			if (Key1.x == Key2.x && Key1.y == Key2.y)
			{
				return true;
			}
			return false;
	}


}
