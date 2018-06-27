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

#include <cstdio>      /* printf, scanf, puts, NULL */
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
#include <vector>
#include <queue>

#include <physics/physics.h>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "engine/modules.h"

#define GAMEOBJECTS_NMB 100000
#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 800
#define GAMEOBJECT_SIZE 10
#define GAMEOBJECT_MARGIN 2

#define BODY_SPEED 4.0f
#define VELOCITY_ITERATIONS 8
#define POSITION_ITERATIONS 3
#define PHYSICS_UPDATE_DELTATIME 0.02f

#define DT_AVG_NMB 60

//#define USE_B2
//#define OOP
//#define COMP
#define ECS

#ifdef USE_B2
static b2World world(b2Vec2(0.0f, 0.0f));
#endif


#ifdef ECS

namespace ECS
{
	enum class Component
	{
		COMPONENT_NONE = 0,
		COMPONENT_RECTSHAPE = 1 << 0,
		COMPONENT_TRANSFORM = 1 << 1,
#ifdef USE_B2
		COMPONENT_BODY2D    = 1 << 2
#endif
	};
	struct Transform 
	{
		sf::Vector2f position = sf::Vector2f();
#ifndef USE_B2
		sf::Vector2f velocity = sf::Vector2f();
#endif
	};
	struct World
	{
		unsigned int mask[GAMEOBJECTS_NMB]{static_cast<unsigned int>(Component::COMPONENT_NONE)};

		sf::RectangleShape rectShapes[GAMEOBJECTS_NMB];
		Transform transforms[GAMEOBJECTS_NMB];
#ifdef USE_B2
		b2Body bodies[GAMEOBJECTS_NMB];
#endif
		unsigned int CreateEntity()
		{
			unsigned entity;
			for(entity = 0U ; entity < GAMEOBJECTS_NMB; entity++)
			{
				if(mask[entity] == static_cast<unsigned int>(Component::COMPONENT_NONE))
				{
					return entity;
				}
			}
		}
		void CreateRectObject(unsigned int entity)
		{
			mask[entity] = static_cast<unsigned>(Component::COMPONENT_RECTSHAPE) | 
				static_cast<unsigned>(Component::COMPONENT_TRANSFORM);
			Transform& transform = transforms[entity];
			sf::RectangleShape& rectShape = rectShapes[entity];

			transform.position = sf::Vector2f(rand() % WINDOW_SIZE_X, rand() % WINDOW_SIZE_Y);
			sf::Vector2f size = sf::Vector2f(
				GAMEOBJECT_SIZE + (rand() % (GAMEOBJECT_MARGIN * 2)) - GAMEOBJECT_MARGIN,
				GAMEOBJECT_SIZE + (rand() % (GAMEOBJECT_MARGIN * 2)) - GAMEOBJECT_MARGIN);
			rectShape.setPosition(transforms[entity].position);
			rectShape.setSize(size);
			rectShape.setOrigin(size / 2.0f);
			rectShape.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
#ifdef USE_B2
			b2BodyDef bodyDef;
			bodyDef.position = sfge::pixel2meter(position);
			bodyDef.type = b2_dynamicBody;

			body = world.CreateBody(&bodyDef);
			/*
			b2FixtureDef fixtureDef;
			fixtureDef.isSensor = true;
			fixtureDef.restitution = 0.0f;

			b2PolygonShape shape;
			shape.SetAsBox(sfge::pixel2meter(size.x / 2.0f), sfge::pixel2meter(size.y / 2.0f));
			fixtureDef.shape = &shape;

			body->CreateFixture(&fixtureDef);
			*/
			body->SetLinearVelocity(b2Vec2(BODY_SPEED*((rand() % 200) / 100.0f - 1.0f), BODY_SPEED*((rand() % 200) / 100.0f - 1.0f)));
#else
			transform.velocity = sfge::meter2pixel(b2Vec2(
				BODY_SPEED*((rand() % 200) / 100.0f - 1.0f),
				BODY_SPEED*((rand() % 200) / 100.0f - 1.0f)));
#endif
		}
		void DestroyEntity(unsigned int entity)
		{
			mask[entity] = static_cast<unsigned>(Component::COMPONENT_NONE);
		}

		void Update(float dt)
		{
			for(int i = 0; i < GAMEOBJECTS_NMB; i++)
			{
				if ((mask[i] & static_cast<unsigned int>(Component::COMPONENT_TRANSFORM)) == static_cast<unsigned int>(Component::COMPONENT_TRANSFORM) and 
					(mask[i] & static_cast<unsigned int>(Component::COMPONENT_RECTSHAPE)) == static_cast<unsigned int>(Component::COMPONENT_RECTSHAPE))
				{
					auto& transform = transforms[i];
					transform.position = transform.velocity * dt + transform.position;
					if (transform.position.x < 0 and transform.velocity.x < 0)
					{
						transform.velocity.x = -transform.velocity.x;
					}
					if (transform.position.x > WINDOW_SIZE_X and transform.velocity.x > 0)
					{
						transform.velocity.x = -transform.velocity.x;
					}
					if (transform.position.y < 0 and transform.velocity.y < 0)
					{
						transform.velocity.y = -transform.velocity.y;
					}
					if (transform.position.y > WINDOW_SIZE_Y and transform.velocity.y > 0)
					{
						transform.velocity.y = -transform.velocity.y;
					}
					rectShapes[i].setPosition(transform.position);
				}
			}
		}
		void Draw(sf::RenderWindow& window)
		{
			for(int i = 0; i < GAMEOBJECTS_NMB; i++)
			{
				if((mask[i] & static_cast<unsigned int>(Component::COMPONENT_RECTSHAPE)) == static_cast<unsigned int>(Component::COMPONENT_RECTSHAPE))
				{
					window.draw(rectShapes[i]);
				}
			}
		}
	};

	
}
#endif

#ifdef OOP

namespace OOP
{
    struct GameObject
    {
        GameObject()
        {
            position = sf::Vector2f(rand()%WINDOW_SIZE_X, rand()%WINDOW_SIZE_Y);
            size = sf::Vector2f(
                GAMEOBJECT_SIZE+(rand()%(GAMEOBJECT_MARGIN*2))-GAMEOBJECT_MARGIN,
                GAMEOBJECT_SIZE+(rand()%(GAMEOBJECT_MARGIN*2))-GAMEOBJECT_MARGIN);
            shape.setPosition(position);
            shape.setSize(size);
            shape.setOrigin(size/2.0f);
            shape.setFillColor(sf::Color(rand()%256, rand()%256, rand()%256));
#ifdef USE_B2
            b2BodyDef bodyDef;
            bodyDef.position = sfge::pixel2meter(position);
            bodyDef.type = b2_dynamicBody;

            body = world.CreateBody(&bodyDef);
			/*
            b2FixtureDef fixtureDef;
            fixtureDef.isSensor = true;
            fixtureDef.restitution = 0.0f;

            b2PolygonShape shape;
            shape.SetAsBox(sfge::pixel2meter(size.x / 2.0f), sfge::pixel2meter(size.y / 2.0f));
            fixtureDef.shape = &shape;

            body->CreateFixture(&fixtureDef);
			*/
			body->SetLinearVelocity(b2Vec2(BODY_SPEED*((rand() % 200) / 100.0f - 1.0f), BODY_SPEED*((rand() % 200) / 100.0f - 1.0f)));
#else
			velocity = sfge::meter2pixel(b2Vec2(
				BODY_SPEED*((rand() % 200) / 100.0f - 1.0f), 
				BODY_SPEED*((rand() % 200) / 100.0f - 1.0f)));
#endif
			

        }

        void Update(float dt)
        {
#ifdef USE_B2
			position = sfge::meter2pixel(body->GetPosition());

			const b2Vec2 bodyVelocity = body->GetLinearVelocity();
			
			if (position.x < 0 and bodyVelocity.x < 0)
			{
				body->SetLinearVelocity(b2Vec2(-bodyVelocity.x, bodyVelocity.y));
			}
			if (position.x > WINDOW_SIZE_X and bodyVelocity.x > 0)
			{
				body->SetLinearVelocity(b2Vec2(-bodyVelocity.x, bodyVelocity.y));
			}
			if (position.y < 0 and bodyVelocity.y < 0)
			{
				body->SetLinearVelocity(b2Vec2(bodyVelocity.x, -bodyVelocity.y));
			}
			if (position.y > WINDOW_SIZE_Y and body->GetLinearVelocity().y > 0)
			{
				body->SetLinearVelocity(b2Vec2(bodyVelocity.x, -bodyVelocity.y));
			}
#else
			position = velocity * dt + position;
			if (position.x < 0 and velocity.x < 0)
			{
				velocity.x = -velocity.x;
			}
			if (position.x > WINDOW_SIZE_X and velocity.x > 0)
			{
				velocity.x = -velocity.x;
			}
			if (position.y < 0 and velocity.y < 0)
			{
				velocity.y = -velocity.y;
			}
			if (position.y > WINDOW_SIZE_Y and velocity.y > 0)
			{
				velocity.y = -velocity.y;
			}
#endif
			shape.setPosition(position);
        }

        void Draw(sf::RenderWindow& window) const
        {
            window.draw(shape);
        }
#ifdef USE_B2
        b2Body* body = nullptr;
        b2Fixture* fixture = nullptr;
#else
		sf::Vector2f velocity;
#endif
        sf::RectangleShape shape;

        sf::Vector2f position;
        sf::Vector2f size;
    };
}

#endif
int main()
{
    std::deque<sf::Int64> deltaTimes;
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "ECS test");

#ifdef OOP
    GameObject* gameObjects = new GameObject[GAMEOBJECTS_NMB];
#endif
#ifdef ECS
	auto world = std::make_unique<World>();
	for(int i = 0; i<GAMEOBJECTS_NMB;i++)
	{
		const auto entity = world->CreateEntity();
		world->CreateRectObject(entity);
	}
#endif


    srand(time(nullptr));
    ImGui::SFML::Init(window);
    sf::Clock clock;
	sf::Clock physicsClock;
	sf::Time physicsDt;
	sf::Clock profilingClock;
	sf::Time updateProfileTime;
	sf::Time graphicsProfileTime;
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

		if (physicsClock.getElapsedTime().asSeconds() > PHYSICS_UPDATE_DELTATIME)
		{
			profilingClock.restart();
#ifdef USE_B2
			world.Step(PHYSICS_UPDATE_DELTATIME, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
#endif
			physicsDt = physicsClock.restart();
#ifdef OOP
			//Update objects
			for (int i = 0; i < GAMEOBJECTS_NMB ; i++)
			{
				gameObjects[i].Update(physicsDt.asSeconds());
			}
#endif
#ifdef ECS
			world->Update(physicsDt.asSeconds());
#endif
			updateProfileTime = profilingClock.getElapsedTime();
			deltaTimes.push_back(updateProfileTime.asMicroseconds());
			if (deltaTimes.size() > DT_AVG_NMB)
			{
				deltaTimes.pop_front();
			}
		}

        ImGui::SFML::Update(window, dt);

        ImGui::Begin("Stats");
        {
            std::ostringstream oss;
            oss << "FPS: " << 1.0f / dt.asSeconds()<<"\n";
            sf::Int64 high_fps_dt = -1;
			sf::Int64 low_fps_dt = -1;
            for(auto dt_value : deltaTimes)
            {
                if(high_fps_dt < 0.0f or dt_value < high_fps_dt)
                {
                    high_fps_dt = dt_value;
                }
                if(low_fps_dt < 0.0f or dt_value > low_fps_dt)
                {
                    low_fps_dt = dt_value;
                }
            }
			oss << "UPDATE:  " << updateProfileTime.asMicroseconds() << "\n";

			oss << "LOWEST:  " << low_fps_dt << "\n";
			oss << "HIGHEST: " << high_fps_dt << "\n";
            ImGui::Text(oss.str().c_str());
        }

        ImGui::End();
        window.clear(sf::Color::Black);
#ifdef OOP
        //Show the game objects
        for(int i = 0; i< GAMEOBJECTS_NMB; i++)
        {
            gameObjects[i].Draw(window);
        }
#endif
#ifdef ECS
		world->Draw(window);
#endif

        ImGui::SFML::Render(window);

        window.display();
    }
#ifdef OOP
	delete[] gameObjects;
#endif
    return EXIT_SUCCESS;
}

