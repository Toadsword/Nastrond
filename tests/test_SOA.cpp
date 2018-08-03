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
#include <iostream>
#include<SFML/System.hpp>


#define ENTITY_NUMBERS 100'000'000

namespace SOA
{
class TransformSystem
{
public:
	TransformSystem()
	{
		m_Positions.resize(ENTITY_NUMBERS);
		m_Scales.resize(ENTITY_NUMBERS);
		m_EulerAngles.resize(ENTITY_NUMBERS);
		for (int i = 0; i < ENTITY_NUMBERS;i++)
		{
			m_Positions[i] = sf::Vector2f(rand(), rand());
			m_Scales[i] = sf::Vector2f(rand(), rand());
			m_EulerAngles[i] = rand();
		}
	}
	void Translate(sf::Vector2f moveValue)
	{
		for (auto& position : m_Positions)
		{
			position += moveValue;
		}
	}
	void Scale(float scaleValue)
	{
		for (auto& scale : m_Scales)
		{
			scale *= scaleValue;
		}
	}
	void Rotate(float rotateValue)
	{
		for (auto& eulerAngle : m_EulerAngles)
		{
			eulerAngle += rotateValue;
		}
	}
private:
	std::vector<sf::Vector2f> m_Positions;
	std::vector<sf::Vector2f> m_Scales;
	std::vector<float> m_EulerAngles;
};
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

	system("pause");
	return EXIT_SUCCESS;
}