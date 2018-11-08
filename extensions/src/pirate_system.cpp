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

#include <extensions/pirate_system.h>
#include <engine/entity.h>
#include <engine/engine.h>
#include <engine/transform2d.h>
#include <graphics/graphics2d.h>
#include <physics/physics2d.h>

namespace sfge
{
static const int piratesNmb = 100;

PirateSystem::PirateSystem(Engine &engine) : System(engine)
{
    auto config = m_Engine.GetConfig();
    m_Engine.GetEntityManager().ResizeEntityNmb(piratesNmb);
    m_Pirates.resize(piratesNmb);
    for(int i = 0; i < piratesNmb; i++)
    {
        auto newEntity = m_Engine.GetEntityManager().CreateEntity(i+1);
        auto* transform = m_Engine.GetTransform2dManager().AddComponent(newEntity);
        transform->Position = sf::Vector2f(
            rand()%config->screenResolution.x,
            rand()%config->screenResolution.y);
        auto* sprite = m_Engine.GetGraphics2dManager().GetSpriteManager().AddComponent(newEntity);
        auto textureId = m_Engine.GetGraphics2dManager().GetTextureManager().LoadTexture("data/pirates/Ships/ship (1).png");
        auto* texture = m_Engine.GetGraphics2dManager().GetTextureManager().GetTexture(textureId);
        sprite->SetTexture(texture);
        m_Pirates[i].transform2d = transform;
        m_Pirates[i].entity = newEntity;

    }
}

void PirateSystem::Init()
{

}

void PirateSystem::Update(float dt)
{

    for(auto& pirate : m_Pirates)
    {
        auto* closestPirate = GetClosestPirate(&pirate);
        if(closestPirate != nullptr)
        {
            Vec2f shootingPos = closestPirate->transform2d->Position;
            Vec2f deltaPos = shootingPos-pirate.transform2d->Position;

            Vec2f wantedDirection;
            if(deltaPos.GetMagnitude() > pirate.shootingRange)
            {
                wantedDirection = deltaPos.Normalized();
            }
            else
            {
                wantedDirection = deltaPos.Normalized();
                wantedDirection = Vec2f(-wantedDirection.y, wantedDirection.x);
                //TODO Shoot
            }
            wantedDirection = wantedDirection.Normalized();
            pirate.direction = Vec2f::Lerp(pirate.direction, wantedDirection, dt).Normalized();

        }
        pirate.transform2d->EulerAngle = Vec2f::AngleBetween(pirate.direction, Vec2f(0.0f,1.0f));
        pirate.transform2d->Position += pirate.direction * dt * pirate.speed;
    }
}

PirateData* PirateSystem::GetClosestPirate(PirateData* pirate)
{
    PirateData* closestPirate = nullptr;
    float closestDistance = 10'000.0f;
    for(auto& otherPirate: m_Pirates)
    {
        if(&otherPirate != pirate)
        {
            float distance = (otherPirate.transform2d->Position-pirate->transform2d->Position).GetMagnitude();
            if(distance < closestDistance)
            {
                closestDistance = distance;
                closestPirate = &otherPirate;
            }
        }
    }
    return closestPirate;
}

}