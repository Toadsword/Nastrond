#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <vector>
#include <SFML/Graphics.hpp>

namespace sf {
	class RenderWindow;
}

	namespace sfge{
			struct PointOcc
			{
				float x = 0.0f;
				float y = 0.0f;

				PointOcc(int _x, int _y)
				{
					x = _x;
					y = _y;
				}
				PointOcc()
				{
					x = 0.0f;
					y = 0.0f;
				}
			};

			struct AABBOcc
			{
				PointOcc centre;
				PointOcc halfSize;

				bool* is_visible = nullptr;

				AABBOcc(bool* is_visible = nullptr, PointOcc centre = PointOcc(), PointOcc halfSize = PointOcc()) : centre(centre), halfSize(halfSize)
				{
					this->is_visible = is_visible;
				};

				bool contains(PointOcc a)
				{
					if (a.x < centre.x + halfSize.x && a.x > centre.x - halfSize.x)
					{
						if (a.y < centre.y + halfSize.y && a.y > centre.y - halfSize.y)
						{
							return true;
						}
					}
					return false;
				}

				bool intersectsCamera(AABBOcc other)
				{
					//this right > that left                                          this left <s that right
					if (centre.x + halfSize.x > other.centre.x - other.halfSize.x && centre.x - halfSize.x < other.centre.x + other.halfSize.x)
					{
						// This bottom > that top
						if (centre.y + halfSize.y > other.centre.y - other.halfSize.y && centre.y - halfSize.y < other.centre.y + other.halfSize.y)
						{
							return true;
						}
					}
					return false;
				}

				bool intersects(AABBOcc other)
				{
					//this right > that left                                          this left <s that right
					if (centre.x + halfSize.x > other.centre.x - other.halfSize.x || centre.x - halfSize.x < other.centre.x + other.halfSize.x)
					{
						// This bottom > that top
						if (centre.y + halfSize.y > other.centre.y - other.halfSize.y || centre.y - halfSize.y < other.centre.y + other.halfSize.y)
						{
							return true;
						}
					}
					return false;
				}
			};

			const short MAX_DEPTH = 7;

			class QuadtreeOcc
			{
			public:
				QuadtreeOcc();
				QuadtreeOcc(AABBOcc boundary);
				QuadtreeOcc(AABBOcc boundary, short depht);
				

				~QuadtreeOcc();

				void Draw(sf::RenderWindow* window);

				void CheckCamera(AABBOcc camera);

				int TreeHeight();

				bool insert(AABBOcc d);
				void subdivide();
				std::vector<AABBOcc> queryRange(AABBOcc range);
				AABBOcc boundary;

				//4 children
				QuadtreeOcc* nw;
				QuadtreeOcc* ne;
				QuadtreeOcc* sw;
				QuadtreeOcc* se;
			private:
				std::vector<AABBOcc> objects;

				int CAPACITY;

				bool is_visible = false;
				short depth;
			};

	}
#endif