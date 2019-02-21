/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

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
*/
#ifndef HASHMAP2D_H
#define HASHMAP2D_H

#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace sfge
{

	struct PointHash
	{
		sf::Vector2f position;

		bool* is_visible = nullptr;

		PointHash(bool& is_visible, float argX, float argY)
		{										
			position = sf::Vector2f(argX, argY);
			this->is_visible = &is_visible;																		 
		}

		PointHash(bool* is_visible, sf::Vector2f position)
		{
			this->position = position;
			this->is_visible = is_visible;
		}
		PointHash()																								 
		{																										 
																												 
		}																										 
	};																											 
																												 
	struct AABBHash																								 
	{																											 
		PointHash centre;																						 
		PointHash halfSize;																						 
		AABBHash()
		{
			
		};

		AABBHash(PointHash centre, PointHash halfSize) : centre(centre), halfSize(halfSize)						 
		{																										 
																												 
		};																										 
																												 
		bool contains(PointHash a)																				 
		{																										 
			if (a.position.x < centre.position.x + halfSize.position.x && a.position.x > centre.position.x - halfSize.position.x)
			{
				if (a.position.y < centre.position.y + halfSize.position.y && a.position.y > centre.position.y - halfSize.position.y)
				{
					return true;
				}
			}
			return false;
		}

		bool intersectsCamera(AABBHash other)
		{
			//this right > that left                                          this left <s that right
			if (centre.position.x + halfSize.position.x > other.centre.position.x - other.halfSize.position.x && centre.position.x - halfSize.position.x < other.centre.position.x + other.halfSize.position.x)
			{
				// This bottom > that top
				if (centre.position.y + halfSize.position.y > other.centre.position.y - other.halfSize.position.y && centre.position.y - halfSize.position.y < other.centre.position.y + other.halfSize.position.y)
				{
					return true;
				}
			}
			return false;
		}

		//bool intersects(AABBHash other)
		//{
		//	//this right > that left                                          this left <s that right
		//	if (centre.x + halfSize.x > other.centre.x - other.halfSize.x || centre.x - halfSize.x < other.centre.x + other.halfSize.x)
		//	{
		//		// This bottom > that top
		//		if (centre.y + halfSize.y > other.centre.y - other.halfSize.y || centre.y - halfSize.y < other.centre.y + other.halfSize.y)
		//		{
		//			return true;
		//		}
		//	}
		//	return false;
		//}
	};


	class MyHashCompare : public stdext::hash_compare <sf::Vector2f>
	{
	public:

		MyHashCompare();

		size_t operator( )(const sf::Vector2f& Key) const; // This is a hash function

		bool operator () (const sf::Vector2f& Key1, const sf::Vector2f& Key2) const;

	};

	class HashTable
	{
	public:
		HashTable(int argCellSize);
		HashTable(int argCellSize, int argWidth, int argHeight);
		~HashTable(void);

		void CheckCamera(AABBHash aabbCamera);

		void Draw(sf::RenderWindow* window);

		void InsertObjectPoint(PointHash* object);
	private:

		int cellSize;
		bool sparse;
		std::unordered_map<sf::Vector2f, std::vector<PointHash*>, MyHashCompare> table;
		int width, height;
		// Methods
		sf::Vector2f Hash(const sf::Vector2f& position);
	};
}
#endif