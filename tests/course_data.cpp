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

#include <memory>
#include <cmath>
#include <vector>
#include <list>
#include <cstdlib>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>

#define LENGTH 1'000'000

int main()
{
    std::vector<float> numbers1(LENGTH);
    std::array<float, LENGTH> numbers5;
    std::list<float> numbers2;
    std::vector<float*> numbers3(LENGTH);
    std::list<float*> numbers4;
    std::array<float, 8> tmp;
    int tmpIndex = 0;
    for(int i = 0; i<LENGTH;i++)
    {
        numbers3[i] = new float(std::rand());
        numbers5[i] = *numbers3[i];
        numbers1[i] = *numbers3[i];
        numbers4.push_back(new float(std::rand()));
        numbers2.push_back(*numbers4.back());
    }

    sf::Clock clock;

    for(int i = 0; i < LENGTH; i++)
    {
        numbers1[i] += numbers1[i];
    }
    sf::Time vectorFloatTime = clock.restart();
    for(int i = 0; i < LENGTH; i++)
    {
        numbers5[i] += numbers5[i];
    }
    sf::Time arrayFloatTime = clock.restart();

    for(int i = 0; i<LENGTH;i++)
    {
        tmp[i%tmp.size()] += *numbers3[i];
    }

    sf::Time vectorFloatPtrTime = clock.restart();

    for(auto elem : numbers2)
    {
        sqrt(elem);
    }

    sf::Time listFloatTime = clock.restart();

    for(auto& elem : numbers4)
    {
        sqrt(*elem);
    }

    sf::Time listFloatPtrTime = clock.restart();

    std::cout << "Vector Float Time: " << vectorFloatTime.asMicroseconds() << "\n"
              << "Array Float Time: " << arrayFloatTime.asMicroseconds() << "\n"
              << "Vector Float Ptr Time: " << vectorFloatPtrTime.asMicroseconds() << "\n"
              << "List Float Time: " << listFloatTime.asMicroseconds() << "\n"
              << "List Float Ptr Time: " << listFloatPtrTime.asMicroseconds() << "\n"
              ;
    return EXIT_SUCCESS;


};