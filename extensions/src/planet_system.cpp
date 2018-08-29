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

#include <extensions/planet_system.h>
#include <engine/engine.h>
#include <engine/config.h>
#include <graphics/graphics2d.h>

namespace sfge::ext
{

PlanetSystem::PlanetSystem(Engine& engine): System(engine),
	m_Transform2DManager(m_Engine.GetTransform2dManager()),
	m_Body2DManager(m_Engine.GetPhysicsManager().GetBodyManager()),
	m_TextureManager(m_Engine.GetGraphics2dManager().GetTextureManager()),
	m_SpriteManager(m_Engine.GetGraphics2dManager().GetSpriteManager())
{
}

void PlanetSystem::Init()
{
	auto config = m_Engine.GetConfig().lock();
	screenSize = sf::Vector2f(config->screenResolution.x, config->screenResolution.y);
	auto& entityManager = m_Engine.GetEntityManager();
	entityManager.ResizeEntityNmb(entitiesNmb);
	for (int i = 0; i < entitiesNmb; i++)
	{
		const auto newEntity = entityManager.CreateEntity(i + 1);
		auto transformPtr = m_Transform2DManager.AddComponent(newEntity);
		transformPtr->Position = sf::Vector2f(std::rand() % static_cast<int>(screenSize.x), std::rand() % static_cast<int>(screenSize.y));

		auto body = m_Body2DManager.AddComponent(newEntity);
		body->SetLinearVelocity(CalculateInitSpeed(transformPtr));

		const auto textureId = m_TextureManager.LoadTexture("data/sprites/round.png");
		const auto texture = m_TextureManager.GetTexture(textureId);

		auto sprite = m_SpriteManager.AddComponent(newEntity);
		sprite->SetTexture(texture);
	}
}

void PlanetSystem::Update(float dt)
{
}

void PlanetSystem::FixedUpdate()
{
	for(int i = 0; i < entitiesNmb;i++)
	{
		auto transformPtr = m_Engine.GetTransform2dManager().GetComponentPtr(i + 1);
		auto bodyPtr = m_Engine.GetPhysicsManager().GetBodyManager().GetComponentPtr(i + 1);
		bodyPtr->ApplyForce(CalculateNewForce(transformPtr));
	}
}

b2Vec2 PlanetSystem::CalculateInitSpeed(Transform2d* transformPtr)
{
	const auto deltaToCenter = screenSize / 2.0f - transformPtr->Position;
	auto vel_dir = sf::Vector2f(-deltaToCenter.y, deltaToCenter.x);
	vel_dir /= Magnitude(vel_dir);

	auto speed = sqrtf(Magnitude(CalculateNewForce(transformPtr)) / planetMass * pixel2meter(Magnitude(deltaToCenter)));
	return b2Vec2(vel_dir.x*speed, vel_dir.y*speed);

}

b2Vec2 PlanetSystem::CalculateNewForce(Transform2d* transformPtr)
{
	const auto deltaToCenter = screenSize / 2.0f - transformPtr->Position;
	const auto r = Magnitude(deltaToCenter);
	const auto force = gravityConst * centerMass*planetMass / (r*r);
	return pixel2meter(deltaToCenter / r * force);
}

float PlanetSystem::Magnitude(sf::Vector2f v)
{
	return sqrtf(v.x*v.x + v.y*v.y);
}

float PlanetSystem::Magnitude(b2Vec2 v)
{
	return sqrtf(v.x*v.x + v.y*v.y);
}
}
