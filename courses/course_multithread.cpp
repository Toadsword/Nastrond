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
#include <cmath>
#include <iostream>
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
#include <future>
#include <sstream>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <physics/physics2d.h>
#include <engine/vector.h>


//#define MULTITHREAD
#define OBJ_NMB 5'000
#define SCREEN_SIZE 800
#define VELOCITY_ITERATIONS 8
#define POSITION_ITERATIONS 3
#define PHYSICS_UPDATE_DELTATIME 0.02f
#define PHYSICS_CORES 4

struct SquareObject
{
	SquareObject()
	{

	}
	void Init(sfge::Vec2f position, sfge::Vec2f size, b2World* world)
	{
		rectangle.setPosition(position);
		rectangle.setSize(size);
		rectangle.setOrigin(rectangle.getSize() / 2.0f);
		rectangle.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		b2BodyDef bodyDef;
		bodyDef.position = sfge::pixel2meter(position);
		bodyDef.type = b2_dynamicBody;

		body = world->CreateBody(&bodyDef);

		b2FixtureDef fixtureDef;
		fixtureDef.restitution = 0.0f;
		fixtureDef.isSensor = false;
		b2PolygonShape shape;
		shape.SetAsBox(sfge::pixel2meter(size.x / 2.0f), sfge::pixel2meter(size.y / 2.0f));
		fixtureDef.shape = &shape;

		body->CreateFixture(&fixtureDef);
	}
	void Update()
	{
		if(body)
		{
			rectangle.setPosition(sfge::meter2pixel(body->GetPosition()));
		}
	}
	void Draw(sf::RenderWindow& window)
	{
		
		window.draw(rectangle);
	}

	
	sf::RectangleShape rectangle;
	b2Body* body = nullptr;
};

struct Contact
{
	SquareObject* o1;
	SquareObject* o2;
};

void f1()
{}

void f2()
{}

int main()
{

	sf::RenderWindow window(sf::VideoMode(800, 800), "Multithread test");
	b2World world(b2Vec2(0.0f, 9.81f));
	std::vector<SquareObject> objects(OBJ_NMB);

	srand(time(nullptr));

	//window.setFramerateLimit(120U);
	
	ImGui::SFML::Init(window);
	sf::Clock graphClock;
	sf::Clock physicsClock;
	sf::Time graphDt;
	sf::Time physicsDt;
#ifdef  MULTITHREAD
	std::future<void> physicsHandler;
#endif
	const int gridSize = sqrt(OBJ_NMB);
	std::cout << "Initialize the objects\n";
	//Initialise objects
	for(int i = 0; i < OBJ_NMB; i++)
	{
		const float x = ((float)i / gridSize)/gridSize * SCREEN_SIZE;
		const float y = (float)(i % gridSize)/gridSize * SCREEN_SIZE;
		objects[i].Init(sf::Vector2f(x,y), sf::Vector2f(SCREEN_SIZE/gridSize/2.0f, SCREEN_SIZE/gridSize/2.0f), &world);
	}
	//Adding ground to the world
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position = sfge::pixel2meter(sf::Vector2f(0.0f, 800.0f));
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2FixtureDef groundFixtureDef;
	groundFixtureDef.restitution = 0.2f;
	groundFixtureDef.isSensor = false;
	b2PolygonShape shape;
	shape.SetAsBox(sfge::pixel2meter(800.0f), sfge::pixel2meter(50.0f));
	groundFixtureDef.shape = &shape;

	groundBody->CreateFixture(&groundFixtureDef);
	std::cout << "Initialization DONE\n";
	while (window.isOpen())
	{
		graphDt = graphClock.restart();


		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		ImGui::SFML::Update(window, graphDt);

#ifdef MULTITHREAD
		if(physicsHandler.valid() && !world.IsLocked())
		{
			physicsHandler.get();
			//Update objects
			for (auto& obj : objects)
			{
				obj.Update();
			}
		}
#else
		if (physicsClock.getElapsedTime().asSeconds() > PHYSICS_UPDATE_DELTATIME)
		{
			world.Step(PHYSICS_UPDATE_DELTATIME, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
			physicsDt = physicsClock.restart();
			//Update objects
			for (auto& obj : objects)
			{
				obj.Update();
			}
		}

#endif
		
#ifdef MULTITHREAD
		if (physicsClock.getElapsedTime().asSeconds() > PHYSICS_UPDATE_DELTATIME && !world.IsLocked())
		{
			//Start physics before graphics
			physicsHandler = std::async(&b2World::Step, &world, PHYSICS_UPDATE_DELTATIME, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
			physicsDt = physicsClock.restart();
		}
#endif
		ImGui::Begin("Stats");
		{
			std::ostringstream oss;
			oss << "Graph FPS: " << 1.0f / graphDt.asSeconds() << "\nPhys FPS: "<<1.0f / physicsDt.asSeconds();

			ImGui::Text(oss.str().c_str());
		}

		ImGui::End();
		window.clear(sf::Color::Black);
		//Draw objects
		for(auto& obj : objects)
		{
			obj.Draw(window);
		}
		ImGui::SFML::Render(window);

		window.display();
	}
	ImGui::SFML::Shutdown();

	return EXIT_SUCCESS;
}