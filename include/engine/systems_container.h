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

#ifndef SFGE_SYSTEMS_CONTAINER_H
#define SFGE_SYSTEMS_CONTAINER_H

#include <graphics/graphics2d.h>
#include <audio/audio.h>
#include <engine/scene.h>
#include <input/input.h>
#include <python/python_engine.h>
#include <physics/physics2d.h>
#include <engine/entity.h>
#include <engine/transform2d.h>
#include <editor/editor.h>

namespace sfge
{
struct SystemsContainer
{
 public:
  SystemsContainer(Engine &engine);

  Graphics2dManager graphics2dManager{m_Engine};
  AudioManager audioManager{m_Engine};
  SceneManager sceneManager{m_Engine};
  InputManager inputManager{m_Engine};
  PythonEngine pythonEngine{m_Engine};
  Physics2dManager physicsManager{m_Engine};
  Editor editor{m_Engine};
  EntityManager entityManager{m_Engine};
  Transform2dManager transformManager{m_Engine};
 private:
  Engine& m_Engine;

};
}
#endif //SFGE_SYSTEMS_CONTAINER_H
