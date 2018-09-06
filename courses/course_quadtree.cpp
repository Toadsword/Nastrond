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

#include <list>
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
#include <sstream>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <physics/physics2d.h>
#include <imgui-SFML.h>
#include <imgui.h>




struct SquareObject
{
	SquareObject(b2Vec2 position, b2Vec2 extends, b2Vec2 speed)
	{
		this->position = position;
		this->extends = extends;
		this->speed = speed;
		Update(0.0f);
	}
	sf::RectangleShape rectangle;
	void Update(float dt)
	{
		b2Vec2 deltaPos = speed;
		deltaPos *= dt;
		position = position + deltaPos;
		aabb.lowerBound = position - extends;
		aabb.upperBound = position + extends;
		
	}
	void Draw(sf::RenderWindow& window)
	{
		rectangle.setPosition(sfge::meter2pixel(position) - sfge::meter2pixel(extends));
		rectangle.setSize ( sfge::meter2pixel(extends)*2.0f);
		window.draw(rectangle);
	}

	b2AABB aabb;
	b2Vec2 position;
	b2Vec2 extends;
	b2Vec2 speed;
};

struct Contact
{
	SquareObject* o1;
	SquareObject* o2;
};

class QuadTree
{
public:
	QuadTree(int nodeLevel, b2AABB bounds)
	{
		m_NodeLevel = nodeLevel;
		m_Bounds = bounds;
	}
	~QuadTree()
	{
		for (int i = 0; i < CHILD_TREE_NMB; i++)
		{
			if (nodes[i] != nullptr)
			{
				nodes[i]->Clear();
				delete(nodes[i]);
				nodes[i] = nullptr;
			}
		}
	}
	void Clear()
	{
		m_Objects.clear();
		
		for (int i = 0; i < CHILD_TREE_NMB; i++)
		{
			if (nodes[i] != nullptr)
			{
				nodes[i]->Clear();
				delete(nodes[i]);
				nodes[i] = nullptr;
			}
		}
		
	}
	void Split()
	{
		b2Vec2 center = m_Bounds.GetCenter();
		nodes[0] = new QuadTree(m_NodeLevel + 1, 
			{center, m_Bounds.upperBound});
		nodes[1] = new QuadTree(m_NodeLevel + 1, 
			{b2Vec2(m_Bounds.lowerBound.x, center.y), b2Vec2(center.x, m_Bounds.upperBound.y)});
		nodes[2] = new QuadTree(m_NodeLevel + 1, 
			{m_Bounds.lowerBound, center});
		nodes[3] = new QuadTree(m_NodeLevel + 1, 
			{b2Vec2(center.x, m_Bounds.lowerBound.y), b2Vec2(m_Bounds.upperBound.x, center.y)});
	}

	
	int GetIndex(SquareObject* rect)
	{
		int index = -1;
		double verticalMidpoint = m_Bounds.GetCenter().x;
		double horizontalMidpoint = m_Bounds.GetCenter().y;

		// Object can completely fit within the top quadrants
		bool topQuadrant = 
			(rect->aabb.lowerBound.y > horizontalMidpoint);
		// Object can completely fit within the bottom quadrants
		bool bottomQuadrant = 
			(rect->aabb.upperBound.y < horizontalMidpoint);

		// Object can completely fit within the left quadrants
		if (rect->aabb.upperBound.x < verticalMidpoint) 
		{
			if (topQuadrant) 
			{
				index = 1;
			}
			else if (bottomQuadrant) 
			{
				index = 2;
			}
		}
		// Object can completely fit within the right quadrants
		else if (rect->aabb.lowerBound.x > verticalMidpoint) 
		{
			if (topQuadrant) {
				index = 0;
			}
			else if (bottomQuadrant) {
				index = 3;
			}
		}
		return index;
	}
	void Insert(SquareObject* obj)
	{
		if (nodes[0] != nullptr) 
		{
			int index = GetIndex(obj);

			if (index != -1) 
			{
				nodes[index]->Insert(obj);

				return;
			}
		}

		m_Objects.push_back(obj);

		if (m_Objects.size() > MAX_OBJECTS && m_NodeLevel < MAX_LEVELS) {
			if (nodes[0] == nullptr) 
			{
				Split();
			}

			auto objItr = m_Objects.begin();
			while (objItr != m_Objects.end()) 
			{
				int index = GetIndex(*objItr);
				if (index != -1) 
				{
					nodes[index]->Insert(*objItr);
					objItr = m_Objects.erase(objItr);
				}
				else {
					objItr++;
				}
			}
		}
	}
	std::list<Contact> Retrieve()
	{
		std::list<Contact> contacts;
		if (nodes[0] == nullptr)
		{
			if (m_Objects.size() > 1)
			{
				auto objItr = m_Objects.begin();
				auto objSecondItr = m_Objects.begin();
				while (objItr != m_Objects.end())
				{
					objSecondItr = objItr;
					objSecondItr++;
					while (objSecondItr != m_Objects.end())
					{
						if ((*objItr)->aabb.Contains((*objSecondItr)->aabb))
						{
							contacts.push_back({ (*objItr), (*objSecondItr) }); //Create contact struct
						}
						objSecondItr++;
					}
					objItr++;
				}
			}
		}
		else
		{
			//Add children contacts
			for (int i = 0; i < CHILD_TREE_NMB; i++)
			{
				auto childContacts = nodes[i]->Retrieve();
				contacts.insert(contacts.end(), childContacts.begin(), childContacts.end());
			}
			//Retrieve all parents objects
			for (auto objItr = m_Objects.begin(); objItr != m_Objects.end(); objItr++)
			{
				for (int i = 0; i < CHILD_TREE_NMB; i++)
				{
					for (auto childObjItr = nodes[i]->m_Objects.begin(); childObjItr != nodes[i]->m_Objects.end(); childObjItr++)
					{
						if ((*objItr)->aabb.Contains((*childObjItr)->aabb))
						{
							contacts.push_back({ (*objItr), (*childObjItr) }); //Create contact struct
						}
					}
				}
			}
		}

		return contacts;
	}
	void Update(float dt)
	{
		position = m_Bounds.GetCenter();
		extends = m_Bounds.GetExtents();
		for (int i = 0; i < CHILD_TREE_NMB; i++)
		{
			if (nodes[i] != nullptr)
			{
				nodes[i]->Update(dt);
			}
		}
	}
	void Draw(sf::RenderWindow& window)
	{
		rectangle.setPosition(sfge::meter2pixel(position) - sfge::meter2pixel(extends));
		rectangle.setSize(sfge::meter2pixel(extends)*2.0f);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineThickness(1.0f);
		rectangle.setOutlineColor(sf::Color::Blue);
		window.draw(rectangle);
		for (int i = 0; i < CHILD_TREE_NMB; i++)
		{
			if (nodes[i] != nullptr)
			{
				nodes[i]->Draw(window);
			}
		}
	}
private:

	b2Vec2 position;
	b2Vec2 extends;
	sf::RectangleShape rectangle;
	static const int MAX_OBJECTS = 10;
	static const int MAX_LEVELS = 10;
	static const int CHILD_TREE_NMB = 4;
	int m_NodeLevel = 0;
	QuadTree* nodes[CHILD_TREE_NMB] = {nullptr};
	std::list<SquareObject*> m_Objects;
	b2AABB m_Bounds;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "QuadTree test");
	QuadTree quad(0, { b2Vec2(0,0), sfge::pixel2meter(sf::Vector2i(800,800)) });

	srand(time(NULL));

	std::list<SquareObject> objectsList;
	//const int objNmb = 100;
	const int objNmb = 1000;
	for (int i = 0; i < objNmb; i++)
	{
		objectsList.push_back({
			sfge::pixel2meter(sf::Vector2f(rand() % 800, rand() % 800)),
			sfge::pixel2meter(sf::Vector2f(rand() % 10, rand() % 10)),
			sfge::pixel2meter(sf::Vector2f((rand() % 100) - 50, (rand() % 100) - 50)),
			});
	}
	ImGui::SFML::Init(window);
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		ImGui::SFML::Update(window, dt);

		

		quad.Clear();
		for (auto& obj : objectsList)
		{
			obj.Update(dt.asSeconds());
			sf::Vector2f objPos = sfge::meter2pixel(obj.position);
			if (objPos.x < 0.0f && obj.speed.x<0.0f)
			{
				obj.speed.x = -obj.speed.x;
			}
			else if (objPos.x > 800.0f && obj.speed.x > 0.0f)
			{
				obj.speed.x = -obj.speed.x;
			}
			if (objPos.y < 0.0f && obj.speed.y<0.0f)
			{
				obj.speed.y = -obj.speed.y;
			}
			else if (objPos.y > 800.0f && obj.speed.y > 0.0f)
			{
				obj.speed.y = -obj.speed.y;
			}
			quad.Insert(&obj);
		}
		auto contacts = quad.Retrieve();
		ImGui::Begin("Stats");
		{
			std::ostringstream oss;
			oss << "FPS: " << 1.0f / dt.asSeconds()<<"\n"<<"Contact numbers: "<<contacts.size();

			ImGui::Text(oss.str().c_str());
		}

		ImGui::End();
		window.clear(sf::Color::Black);

		for (auto& obj : objectsList)
		{
			obj.Draw(window);
		}

		quad.Update(dt.asSeconds());
		quad.Draw(window);
		ImGui::SFML::Render(window);

		window.display();
	}
	ImGui::SFML::Shutdown();

	return EXIT_SUCCESS;
}