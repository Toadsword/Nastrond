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
#include <functional>
#include <queue>
#include <ctpl_stl.h>

#include <physics/physics.h>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
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
#define THREAD_POOL_NMB 4

//#define USE_B2
//#define OOP
//#define COMP
#define ECS
#define THREAD_ECS

#ifdef USE_B2
static b2World world(b2Vec2(0.0f, 0.0f));
#endif


#ifdef ECS

#ifdef THREAD_ECS
ctpl::thread_pool threadPool(THREAD_POOL_NMB);
#endif

namespace ECS
{
	enum class Component
	{
		COMPONENT_NONE = 0,
		COMPONENT_RECTANGLE = 1 << 0,
		COMPONENT_TRANSFORM = 1 << 1,
#ifdef USE_B2
		COMPONENT_BODY2D    = 1 << 2
#endif
	};
	
	struct Transform 
	{
		sf::Vector2f position = sf::Vector2f();
		sf::Vector2f size = sf::Vector2f();
#ifndef USE_B2
		sf::Vector2f velocity = sf::Vector2f();
#endif
	};
	struct World
	{
		unsigned int mask[GAMEOBJECTS_NMB]{static_cast<unsigned int>(Component::COMPONENT_NONE)};
		
		//sf::RectangleShape rectangles[GAMEOBJECTS_NMB];
		sf::VertexArray rectVertexArrays;
		Transform transforms[GAMEOBJECTS_NMB];
#ifdef USE_B2
		b2Body bodies[GAMEOBJECTS_NMB];
#endif
		World()
		{
			rectVertexArrays.setPrimitiveType(sf::Quads);
			rectVertexArrays.resize(GAMEOBJECTS_NMB << 2);
		}
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
			return GAMEOBJECTS_NMB;
		}
		void CreateRectObject(unsigned int entity)
		{
			mask[entity] = static_cast<unsigned>(Component::COMPONENT_RECTANGLE) | 
				static_cast<unsigned>(Component::COMPONENT_TRANSFORM);
			auto& transform = transforms[entity];
			//auto& rectangle = rectangles[entity];

			transform.position = sf::Vector2f(rand() % WINDOW_SIZE_X, rand() % WINDOW_SIZE_Y);
			transform.size = sf::Vector2f(
				GAMEOBJECT_SIZE + (rand() % (GAMEOBJECT_MARGIN * 2)) - GAMEOBJECT_MARGIN,
				GAMEOBJECT_SIZE + (rand() % (GAMEOBJECT_MARGIN * 2)) - GAMEOBJECT_MARGIN);
			
			sf::Color rectColor = (sf::Color(rand() % 256, rand() % 256, rand() % 256));
			for(int i = 0; i < 4; i++)
			{
				rectVertexArrays[entity*4 + i].color = rectColor;
			}
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
#ifdef THREAD_ECS
			std::future<void> joinFutures[THREAD_POOL_NMB];
			for(int threadIndex = 0; threadIndex<THREAD_POOL_NMB;threadIndex++)
			{
				int start = (threadIndex + 1)*GAMEOBJECTS_NMB / (THREAD_POOL_NMB + 1);
				int end = (threadIndex + 2)*GAMEOBJECTS_NMB / (THREAD_POOL_NMB + 1)-1;
				auto worldUpdateFunction = std::bind(&World::UpdateRange, this, dt, start, end);
				joinFutures[threadIndex] = threadPool.push(worldUpdateFunction);
			}
			UpdateRange(dt, 0, GAMEOBJECTS_NMB / (THREAD_POOL_NMB + 1) - 1);
			for(int i = 0; i<THREAD_POOL_NMB;i++)
			{
				joinFutures[i].get();
			}
#else
			UpdateRange(dt, 0, GAMEOBJECTS_NMB);
#endif
		}
		void Draw(sf::RenderWindow& window)
		{
			window.draw(rectVertexArrays);
		}
	private:
		void UpdateRange(float dt, int start, int end)
		{
			for (int i = start; i < end; i++)
			{
				if ((mask[i] & static_cast<unsigned int>(Component::COMPONENT_TRANSFORM)) == static_cast<unsigned int>(Component::COMPONENT_TRANSFORM))
				{
					auto& transform = transforms[i];
					transform.position = transform.velocity * dt + transform.position;
					if ((transform.position.x < 0 and transform.velocity.x < 0) or 
						(transform.position.x > WINDOW_SIZE_X and transform.velocity.x > 0))
					{
						transform.velocity.x = -transform.velocity.x;
					}
					
					if ((transform.position.y < 0 and transform.velocity.y < 0) or
						(transform.position.y > WINDOW_SIZE_Y and transform.velocity.y > 0))
					{
						transform.velocity.y = -transform.velocity.y;
					}
					rectVertexArrays[i * 4].position = transform.position - transform.size / 2.0f;
					rectVertexArrays[i * 4 + 1].position = sf::Vector2f(transform.position.x + transform.size.x / 2.0f, 
																  transform.position.y - transform.size.y/2.0f);
					rectVertexArrays[i * 4 + 2].position = transform.position + transform.size / 2.0f;
					rectVertexArrays[i * 4 + 3].position = sf::Vector2f(transform.position.x - transform.size.x / 2.0f, 
																  transform.position.y + transform.size.y / 2.0f);
					//rectangles[i].setPosition(transform.position);
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
	sf::Clock profilingClock;
	profilingClock.restart();
	sf::Time loadingTime;
    std::deque<sf::Int64> physicsDeltaTimes;
	std::deque<sf::Int64> graphicsDeltaTimes;
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "ECS test");
	
#ifdef OOP
    auto* gameObjects = new GameObject[GAMEOBJECTS_NMB];
#endif
#ifdef ECS
	auto world = std::make_unique<World>();
	for(int i = 0; i<GAMEOBJECTS_NMB;i++)
	{
		const auto entity = world->CreateEntity();
		world->CreateRectObject(entity);
	}
#endif
	loadingTime = profilingClock.getElapsedTime();

    srand(time(nullptr));
    ImGui::SFML::Init(window);
    sf::Clock clock;
	sf::Clock physicsClock;
	sf::Time physicsDt;
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
			physicsDeltaTimes.push_back(updateProfileTime.asMicroseconds());
			if (physicsDeltaTimes.size() > DT_AVG_NMB)
			{
				physicsDeltaTimes.pop_front();
			}
		}

        ImGui::SFML::Update(window, dt);

        ImGui::Begin("Stats");
        {
            std::ostringstream oss;
            oss << "FPS: " << 1.0f / dt.asSeconds()<<"\n";
            sf::Int64 phyiscs_high_dt = -1;
			sf::Int64 physics_low_dt = -1;
            for(auto dt_value : physicsDeltaTimes)
            {
                if(phyiscs_high_dt < 0.0f or dt_value < phyiscs_high_dt)
                {
                    phyiscs_high_dt = dt_value;
                }
                if(physics_low_dt < 0.0f or dt_value > physics_low_dt)
                {
                    physics_low_dt = dt_value;
                }
            }
			sf::Int64 graphics_high_dt = -1;
			sf::Int64 graphics_low_dt = -1;
			for (auto dt_value : graphicsDeltaTimes)
			{
				if (graphics_high_dt < 0.0f or dt_value < graphics_high_dt)
				{
					graphics_high_dt = dt_value;
				}
				if (graphics_low_dt < 0.0f or dt_value > graphics_low_dt)
				{
					graphics_low_dt = dt_value;
				}
			}
			oss << "UPDATE:  " << updateProfileTime.asMicroseconds() << "\n";
			oss << "LOWEST:  " << physics_low_dt << "\n";
			oss << "HIGHEST: " << phyiscs_high_dt << "\n\n";

			oss << "GRAPHICS:" << graphicsProfileTime.asMicroseconds() << "\n";
			oss << "LOWEST  :" << graphics_low_dt << "\n";
			oss << "HIGHEST :" << graphics_high_dt << "\n\n";

			oss << "LOADING :" << loadingTime.asSeconds() << "\n";
            ImGui::Text(oss.str().c_str());
        }

        ImGui::End();
		profilingClock.restart();
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
		graphicsProfileTime = profilingClock.getElapsedTime();
		graphicsDeltaTimes.push_back(graphicsProfileTime.asMicroseconds());
		if(graphicsDeltaTimes.size() > DT_AVG_NMB)
		{
			graphicsDeltaTimes.pop_front();
		}
    }
#ifdef OOP
	delete[] gameObjects;
#endif
    return EXIT_SUCCESS;
}

