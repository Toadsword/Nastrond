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
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <physics/physics.h>


#define SUN_MASS 100.0f
#define PLANET_NMB 5

//Equivalent to p2Body
struct Body 
{
	b2Vec2 linearVelocity;
	b2Vec2 position;
	float mass;
};

//Equivalent to p2World
class World
{
public:
	~World()
	{
		auto itr = m_Bodies.begin();
		while (itr != m_Bodies.end())
		{
			delete(*itr);
			itr = m_Bodies.erase(itr);
		}
	}
	void Step(float dt)
	{
		for (auto body : m_Bodies)
		{
			//Force calculus
			//Should be done in Planet class and go through add force
			b2Vec2 deltaPos = sfge::pixel2meter(sf::Vector2f(400.0f, 400.0f)) - body->position; //
			b2Vec2 forceGravitation = deltaPos;
			forceGravitation.Normalize();
			forceGravitation *= SUN_MASS * body->mass / deltaPos.LengthSquared();
			/*
			for (auto otherBody : m_Bodies)
			{
				if (body != otherBody)
				{
					b2Vec2 r = otherBody->position - body->position;
					b2Vec2 planetGravitation = r;
					planetGravitation.Normalize();
					planetGravitation *= otherBody->mass*body->mass / r.LengthSquared();
					forceGravitation += planetGravitation;
				}
			}*/
			//Acceleration calculus
			b2Vec2 acceleration = forceGravitation;
			acceleration *= 1.0f/body->mass;

			//Kinematic resolution
			b2Vec2 deltaV = acceleration;
			deltaV *= dt;

			b2Vec2 newLinearVelocity = deltaV + body->linearVelocity;
			b2Vec2 deltaMove = newLinearVelocity;
			deltaMove *= dt;
			body->position += deltaMove;
			body->linearVelocity = newLinearVelocity;
		}
	}
	Body* CreateBody()
	{
		Body* newBody = new Body();
		m_Bodies.push_back(newBody);
		return newBody;
	}
	void RemoveBody(Body* body)
	{
		m_Bodies.remove(body);
	}
private:
	std::list<Body*> m_Bodies;
};

struct Planet
{
	Planet()
	{
		position = sf::Vector2f(std::rand() % 800, std::rand() % 800);
		mass = (std::rand() % 5) + 1;
		circleShape.setRadius(3.0f);
		circleShape.setOrigin(sf::Vector2f(3.0f, 3.0f));
		circleShape.setPosition(position);
		circleShape.setFillColor(sf::Color::Red);
	}

	void Update()
	{
		position = sfge::meter2pixel(body->position); //meter 2 pixel
		circleShape.setPosition(position);
	}

	sf::Vector2f position;
	float mass;
	sf::CircleShape circleShape;
	Body* body;
};

int main()
{
	
	World world;
	//Sun graphics representation
	sf::CircleShape sun(6.0f);
	sun.setOrigin(sf::Vector2f(6.0f, 6.0f));
	sun.setPosition(sf::Vector2f(400, 400));
	sun.setFillColor(sf::Color::Yellow);

	std::list<Planet*> planets;
	std::srand(31);//time(NULL));

	for (int i = 0; i < PLANET_NMB; i++)
	{
		Planet* p = new Planet();
		p->body = world.CreateBody();
		sf::Vector2f r = p->position - sun.getPosition();
		b2Vec2 normalizedR = sfge::pixel2meter(r);
		normalizedR.Normalize();

		p->body->position = sfge::pixel2meter(p->position);
		p->body->mass = p->mass;
		p->body->linearVelocity = b2Vec2(-normalizedR.y, normalizedR.x); //for direction
		p->body->linearVelocity *= sqrt(SUN_MASS / sfge::pixel2meter(r).Length()); //for length

		planets.push_back(p);
	}
	
	

	// create the window
	sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
	window.setFramerateLimit(60);
	sf::Clock clock;
	// run the program as long as the window is open
	while (window.isOpen())
	{
		auto deltaTime = clock.restart();
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		world.Step(deltaTime.asSeconds());

		// clear the window with black color
		//window.clear(sf::Color::Black);

		window.draw(sun);
		for (auto& p : planets)
		{
			p->Update();
			window.draw(p->circleShape);
		}
		// draw everything here...
		// window.draw(...);

		// end the current frame
		window.display();
	}

	auto pItr = planets.begin();
	while (pItr != planets.end())
	{
		delete(*pItr);
		pItr = planets.erase(pItr);
	}

	return 0;
}
