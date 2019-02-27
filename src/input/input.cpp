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

#include <sstream>

#include <input/input.h>
#include <utility/log.h>
#include <SFML/Window.hpp>
#include <imgui-SFML.h>

namespace sfge
{


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

void InputManager::Update(float dt)
{
	m_KeyboardManager.Update(dt);
	m_MouseManager.Update(dt);
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

void KeyboardManager::Update(float dt)
{
	(void) dt;
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
	{
		keyPressedStatusArray[i].previousKeyPressed = keyPressedStatusArray[i].keyPressed;
		keyPressedStatusArray[i].keyPressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
		/*
		if(IsKeyDown(static_cast<sf::Keyboard::Key>(i)))
		{
			std::ostringstream oss;
			oss << "[Input] Pressing key: "<<i;
			Log::GetInstance()->Msg(oss.str());
		}
		*/
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

void MouseManager::Update(float dt)
{
	(void)dt;
	for (int i = 0; i < sf::Mouse::ButtonCount; i++)
	{
		buttonPressedStatusArray[i].previousKeyPressed = buttonPressedStatusArray[i].keyPressed;
		buttonPressedStatusArray[i].keyPressed = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));
		/*
		if (IsButtonDown(static_cast<sf::Mouse::Button>(i)))
		{
			std::ostringstream oss;
			oss << "[Input] Pressing key: " << i;
			Log::GetInstance()->Msg(oss.str());
		}
		*/
	}
}

sf::Vector2i MouseManager::GetLocalPosition() const
{
	return sf::Mouse::getPosition(*m_Engine.GetGraphics2dManager()->GetWindow());
}
sf::Vector2i MouseManager::GetWorldPosition() const
{
	return sf::Mouse::getPosition();
}
bool MouseManager::IsButtonHeld(sf::Mouse::Button button) const
{
	return buttonPressedStatusArray[(int)button].keyPressed;
}
bool MouseManager::IsButtonDown(sf::Mouse::Button button) const
{
	return !buttonPressedStatusArray[(int)button].previousKeyPressed && buttonPressedStatusArray[(int)button].keyPressed;
}
bool MouseManager::IsButtonUp(sf::Mouse::Button button) const
{
	return !buttonPressedStatusArray[(int)button].keyPressed && buttonPressedStatusArray[(int)button].previousKeyPressed;
}
}
