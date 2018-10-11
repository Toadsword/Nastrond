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
#include <vector>
#include <array>
#include <iostream>
#include<SFML/System.hpp>


#define ENTITY_NUMBERS 10'000'000
//#define SOA_VECTOR2
namespace SOA
{
#ifndef SOA_VECTOR2
class TransformSystem
{
public:
	TransformSystem()
	{
		m_PositionsX.resize(ENTITY_NUMBERS);
		m_PositionsY.resize(ENTITY_NUMBERS);
		m_ScalesX.resize(ENTITY_NUMBERS);
		m_ScalesY.resize(ENTITY_NUMBERS);
		m_EulerAngles.resize(ENTITY_NUMBERS);
		for (int i = 0; i < ENTITY_NUMBERS;i++)
		{
			m_PositionsX[i] = rand();
			m_PositionsY[i] = rand();
			m_ScalesX[i] = rand();
			m_ScalesY[i] = rand();
			m_EulerAngles[i] = rand();
		}
	}
	void Translate(sf::Vector2f moveValue)
	{
		/*	
		for (int i = 0; i < ENTITY_NUMBERS; i++)
		{
			m_PositionsX[i] += moveValue.x;
			m_PositionsY[i] += moveValue.y;
		}
		*/
		for (auto& posX : m_PositionsX)
		{
			posX += moveValue.x;
		}
		for (auto& posY : m_PositionsY)
		{
			posY += moveValue.y;
		}
	}
	void Scale(float scaleValue)
	{
		/*
		for (int i = 0; i < ENTITY_NUMBERS; i++)
		{
			m_ScalesX[i] *= scaleValue;
			m_ScalesY[i] *= scaleValue;
		}
		*/
		for (auto& scaleX : m_ScalesX)
		{
			scaleX *= scaleValue;
		}
		for (auto& scaleY : m_ScalesY)
		{
			scaleY *= scaleValue;
		}
	}
	void Rotate(float rotateValue)
	{
		for (int i = 0; i < ENTITY_NUMBERS; i++)
		{
			m_EulerAngles[i] += rotateValue;
		}
	}
private:
	std::vector<float> m_PositionsX;
	std::vector<float> m_PositionsY;
	std::vector<float> m_ScalesX;
	std::vector<float> m_ScalesY;
	std::vector<float> m_EulerAngles;
};
#else
class TransformSystem
{
public:
	TransformSystem()
	{
		m_Positions.resize(ENTITY_NUMBERS);
		m_Scales.resize(ENTITY_NUMBERS);
		m_EulerAngles.resize(ENTITY_NUMBERS);
		for (int i = 0; i < ENTITY_NUMBERS; i++)
		{
			m_Positions[i] = sf::Vector2f(rand(), rand());
			m_Scales[i] = sf::Vector2f(rand(), rand());
			m_EulerAngles[i] = rand();
		}
	}
	void Translate(sf::Vector2f moveValue)
	{
		for (int i = 0; i < ENTITY_NUMBERS; i++)
		{
			m_Positions[i] += moveValue;
		}
	}
	void Scale(float scaleValue)
	{
		for (int i = 0; i < ENTITY_NUMBERS; i++)
		{
			m_Scales[i] *= scaleValue;
		}
	}
	void Rotate(float rotateValue)
	{
		for (int i = 0; i < ENTITY_NUMBERS; i++)
		{
			m_EulerAngles[i] += rotateValue;
		}
	}
private:
	std::vector<sf::Vector2f> m_Positions;
	std::vector<sf::Vector2f> m_Scales;
	std::vector<float> m_EulerAngles;
};
#endif
}

namespace AOS
{
struct Transform
{
	sf::Vector2f position;
	sf::Vector2f scale;
	float eulerAngle;
	
};

class TransformSystem
{
public:
	TransformSystem()
	{
		m_Transforms.resize(ENTITY_NUMBERS);
		for (auto& transform : m_Transforms)
		{
			transform.position = sf::Vector2f(rand(), rand());
			transform.scale = sf::Vector2f(rand(), rand());
			transform.eulerAngle =rand();
		}
	}

	void Translate(sf::Vector2f moveValue)
	{
		for (auto& transform : m_Transforms)
		{
			transform.position += moveValue;
		}
	}
	void Scale(float scaleValue)
	{
		for (auto& transform : m_Transforms)
		{
			transform.scale *= scaleValue;
		}
	}
	void Rotate(float rotateValue)
	{
		for (auto& transform : m_Transforms)
		{
			transform.eulerAngle += rotateValue;
		}
	}
private:
	std::vector <Transform> m_Transforms;
};
}

int main()
{
	sf::Clock performanceClock;

	auto aosTransformSystem = std::make_unique<AOS::TransformSystem>();

	sf::Time aosLoadingTime = performanceClock.restart();
	aosTransformSystem->Translate(sf::Vector2f(9.0f, -4.0f));
	aosTransformSystem->Scale(0.5f);
	aosTransformSystem->Rotate(45.0f);
	sf::Time aosTime = performanceClock.restart();

	auto soaTransformSystem = std::make_unique<SOA::TransformSystem>();

	sf::Time soaLoadingTime = performanceClock.restart();

	soaTransformSystem->Translate(sf::Vector2f(9.0f, -4.0f));
	soaTransformSystem->Scale(0.5f);
	soaTransformSystem->Rotate(45.0f);
	sf::Time soaTime = performanceClock.restart();

	std::cout
		<< "AOS Loading: " << aosLoadingTime.asMicroseconds() << "\n"
		<< "AOS Compute: " << aosTime.asMicroseconds() << "\n"
		<< "SOA Loading: " << soaLoadingTime.asMicroseconds() << "\n"
		<< "SOA Compute: " << soaTime.asMicroseconds() << "\n";
#if WIN32
	system("pause");
#endif
	return EXIT_SUCCESS;
}