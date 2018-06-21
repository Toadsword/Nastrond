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

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <cmath>
#include <iostream>

#include <engine/modules.h>

int factorial(int n)
{
	int res = 1;
	for (int i = 1; i <= n; i++)
	{
		res *= i;
	}
	return res;
}

int binomialCoeff(int n, int k)
{
	return factorial(n)/(factorial(k)*factorial(n-k));
}

class LerpCurve
{

};

class BezierCurve
{
public:
	BezierCurve(int resolution = 5)
	{
		this->resolution = resolution;
		
	}

	sf::Vector2f CalculateBezier(float t)
	{
		int n = points.size()-1;
		sf::Vector2f res(0.0f, 0.0f);
		for (int i = 0; i <= n; i++)
		{
			auto bTerm = binomialCoeff(n, i) * pow(1.0f - t, n - i) * pow(t, i) * points[i];
			res += bTerm;
		}
		return res;
	}

	void Draw(sf::RenderWindow& window)
	{
		int n = points.size();
		int curveLength = n * resolution + 1;
		
		sf::VertexArray bezierCurveVertexArray(sf::PrimitiveType::LinesStrip, curveLength);
		
		sf::VertexArray bezierPointsVertexArray(sf::PrimitiveType::Points, n);
		
		for (int i = 0; i < n; i++)
		{
			if (n > 1)
			{
				for (int j = 0; j < resolution; j++)
				{
					sf::Vector2f bezierValue = CalculateBezier((float)(i*resolution + j) / (curveLength));
					bezierCurveVertexArray[i * resolution + j] = sf::Vertex(bezierValue, sf::Color::Red);
				}
				sf::Vector2f bezierValue = CalculateBezier(1.0f);
				bezierCurveVertexArray[curveLength-1] = sf::Vertex(bezierValue, sf::Color::Red);
			}
			bezierPointsVertexArray[i] = sf::Vertex(points[i], sf::Color::Blue);
			
		}
		if (points.size() > 1)
		{
			window.draw(bezierCurveVertexArray);
		}
		window.draw(bezierPointsVertexArray);
		

	}

	void Clear()
	{
		points.clear();
	}

	void Append(sf::Vector2f p)
	{
		points.push_back(p);
	}

private:
	std::vector<sf::Vector2f> points;
	int resolution;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Bezier Curve test");
	BezierCurve b(1);

	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					b.Append((sf::Vector2f)sf::Mouse::getPosition(window));
				}
				else if (event.mouseButton.button == sf::Mouse::Button::Right)
				{
					b.Clear();
				}

			}
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		b.Draw(window);

		// end the current frame
		window.display();
	}

#if WIN32
	system("pause");
#endif
	return 0;
}
