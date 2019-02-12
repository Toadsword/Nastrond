#include <graphics/quadtree.h>

namespace sfge {
		QuadtreeOcc::QuadtreeOcc()
		{
			CAPACITY = 4;
			nw = NULL;
			ne = NULL;
			sw = NULL;
			se = NULL;
			boundary = AABBOcc();
			objects = std::vector<AABBOcc>();
			//std::cout << "Init QuadTree" << std::endl;
		}

		void QuadtreeOcc::Draw(sf::RenderWindow* window)
		{
			sf::Color color;
			if (is_visible)
			{
				color = sf::Color::White;
			}
			else
			{
				color = sf::Color::Red;
			}

			//sf::VertexArray vertices(sf::LineStrip, 5);
			//vertices[0].position = sf::Vector2f(boundary.centre.x - boundary.halfSize.x, boundary.centre.y - boundary.halfSize.y);
			//vertices[0].color = color;
			//vertices[1].position = sf::Vector2f(boundary.centre.x + boundary.halfSize.x, boundary.centre.y - boundary.halfSize.y);
			//vertices[0].color = color;
			//vertices[2].position = sf::Vector2f(boundary.centre.x + boundary.halfSize.x, boundary.centre.y + boundary.halfSize.y);
			//vertices[0].color = color;
			//vertices[3].position = sf::Vector2f(boundary.centre.x - boundary.halfSize.x, boundary.centre.y + boundary.halfSize.y);
			//vertices[0].color = color;
			//vertices[4].position = sf::Vector2f(boundary.centre.x - boundary.halfSize.x, boundary.centre.y - boundary.halfSize.y);
			//vertices[4].color = color;
			//window->draw(vertices);
			
			if (ne != nullptr)
			{
				ne->Draw(window);
			}
			if (nw != nullptr)
			{
				nw->Draw(window);
			}
			if (se != nullptr)
			{
				se->Draw(window);
			}
			if (sw != nullptr)
			{
				sw->Draw(window);
			}
		}

		QuadtreeOcc::QuadtreeOcc(AABBOcc boundary)
		{
			objects = std::vector<AABBOcc>();
			CAPACITY = 4;
			nw = NULL;
			ne = NULL;
			sw = NULL;
			se = NULL;
			this->boundary = boundary;
		}

		QuadtreeOcc::~QuadtreeOcc()
		{
			delete nw;
			delete sw;
			delete ne;
			delete se;
		}

		int QuadtreeOcc::TreeHeight()
		{
			return objects.capacity();
		}

		void QuadtreeOcc::subdivide()
		{
			PointOcc qSize = PointOcc(boundary.halfSize.x / 2, boundary.halfSize.y / 2);
			PointOcc qCentre = PointOcc(boundary.centre.x - qSize.x, boundary.centre.y - qSize.y);
			nw = new QuadtreeOcc(AABBOcc(nullptr ,qCentre, qSize));

			qCentre = PointOcc(boundary.centre.x + qSize.x, boundary.centre.y - qSize.y);
			ne = new QuadtreeOcc(AABBOcc(nullptr, qCentre, qSize));

			qCentre = PointOcc(boundary.centre.x - qSize.x, boundary.centre.y + qSize.y);
			sw = new QuadtreeOcc(AABBOcc(nullptr, qCentre, qSize));

			qCentre = PointOcc(boundary.centre.x + qSize.x, boundary.centre.y + qSize.y);
			se = new QuadtreeOcc(AABBOcc(nullptr, qCentre, qSize));

			std::vector<AABBOcc>::iterator it = objects.begin();

			std::vector<AABBOcc> tmp_vector = objects;
			for(auto element : objects)
			{	
				bool move = false;
				if (nw->insert(element))
				{
					move = true;
				}
				if (ne->insert(element))
				{
					move = true;
				}
				if (sw->insert(element))
				{
					move = true;
				}
				if (se->insert(element))
				{
					move = true;
				}

				if(!move)
				{
					tmp_vector.push_back(element);
				}
			}
			objects = tmp_vector;
		}
		void QuadtreeOcc::CheckCamera(AABBOcc camera)
		{
				bool result = boundary.intersectsCamera(camera);
				is_visible = result;
				for (AABBOcc element : objects)
				{
					(*element.is_visible) = is_visible;
				}
				if (ne != nullptr)
				{
					ne->CheckCamera(camera);
				}
				if (nw != nullptr)
				{
					nw->CheckCamera(camera);
				}
				if (se != nullptr)
				{
					se->CheckCamera(camera);
				}
				if (sw != nullptr)
				{
					sw->CheckCamera(camera);
				}
		}

		bool QuadtreeOcc::insert(AABBOcc d)
		{
			if(!boundary.intersects(d))
			{
				return false;
			}

			if (!boundary.contains(d.centre))
			{
				return false;
			}

			if (objects.size() < CAPACITY)
			{
				objects.push_back(d);
				return true;
			}


			if (nw == NULL)
			{
				subdivide();
			}


			if (nw->insert(d))
			{
				return true;
			}
			if (ne->insert(d))
			{
				return true;
			}
			if (sw->insert(d))
			{
				return true;
			}
			if (se->insert(d))
			{
				return true;
			}
			
			objects.push_back(d);
			return false;
		}

		std::vector<AABBOcc> QuadtreeOcc::queryRange(AABBOcc range)
		{
			std::vector<AABBOcc> pInRange = std::vector<AABBOcc>();

			if (!boundary.intersects(range))
			{
				return pInRange;
			}

			for (int i = 0; i < objects.size(); i++)
			{
				if (range.contains(objects.at(i).centre))
				{
					pInRange.push_back(objects.at(i));
				}
			}

			if (nw == NULL)
			{
				return pInRange;
			}

			std::vector<AABBOcc> temp = nw->queryRange(range);
			pInRange.insert(pInRange.end(), temp.begin(), temp.end());

			temp = ne->queryRange(range);
			pInRange.insert(pInRange.end(), temp.begin(), temp.end());

			temp = sw->queryRange(range);
			pInRange.insert(pInRange.end(), temp.begin(), temp.end());

			temp = se->queryRange(range);
			pInRange.insert(pInRange.end(), temp.begin(), temp.end());

			return pInRange;
		}
}
