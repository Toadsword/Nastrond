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

#include <input/input.h>
#include <engine/log.h>
#include <SFML/Window.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

namespace sfge
{
InputManager::InputManager(Engine & engine, bool enable): Module(engine,enable)
{
	enable = m_Enable;
}


KeyboardManager& InputManager::GetKeyboardManager()
{
	return m_KeyboardManager;
}

MouseManager& InputManager::GetMouseManager()
{
	return m_MouseManager;
}

void InputManager::Init()
{
}

void InputManager::Update(sf::Time dt)
{
	m_KeyboardManager.Update(dt);
}

void InputManager::Destroy()
{
}

void InputManager::Clear()
{
}

void InputManager::Collect()
{
}

void KeyboardManager::Update(sf::Time dt)
{
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		keyPressedStatusArray[i].previousKeyPressed = keyPressedStatusArray[i].keyPressed;
		keyPressedStatusArray[i].keyPressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
	}
}

bool KeyboardManager::IsKeyHeld(sf::Keyboard::Key key) const
{	
	return keyPressedStatusArray[(int) key].keyPressed;
}

bool KeyboardManager::IsKeyDown(sf::Keyboard::Key key) const
{
	return !keyPressedStatusArray[(int)key].previousKeyPressed && keyPressedStatusArray[(int)key].keyPressed;
}
bool KeyboardManager::IsKeyUp(sf::Keyboard::Key key) const
{
	return !keyPressedStatusArray[(int)key].keyPressed && keyPressedStatusArray[(int)key].previousKeyPressed;
}

sf::Vector2i MouseManager::GetLocalPosition(sf::Window& window) const
{
	return sf::Mouse::getPosition(window);
}
}
