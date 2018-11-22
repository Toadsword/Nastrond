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

#ifndef SFGE_PIRATE_SYSTEM_H
#define SFGE_PIRATE_SYSTEM_H

#include <engine/system.h>
#include <utility/time_utility.h>
#include <SFML/Audio.hpp>
#include <engine/globals.h>

namespace sfge
{
class Engine;
struct Transform2d;

struct PirateData
{
  sf::Vector2f direction = sf::Vector2f(0.0f,1.0f);
  Timer shootingTimer{-1.0f, 2.0f};
  Entity entity = INVALID_ENTITY;
  Transform2d* transform2d = nullptr;
  //Constant
  const float speed = 20.0f;
  const float rotSpeed = 20.0f;
  const float shootingRange = 200.0f;

};

class PirateSystem : public System
{
 public:
    PirateSystem(Engine& engine);

    void Init() override;
    void Update(float dt) override;
 private:
  PirateData* GetClosestPirate(PirateData* pirate);
  std::vector<PirateData> m_Pirates;

};

}
#endif //SFGE_PIRATE_SYSTEM_H
