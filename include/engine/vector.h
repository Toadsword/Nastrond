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

#ifndef SFGE_VECTOR_H
#define SFGE_VECTOR_H
#include <SFML/System/Vector2.hpp>
namespace sfge
{

class Vec2f
{
 public:
  float x;
  float y;

  Vec2f(float x, float y);
  Vec2f();
#ifdef SFML_VECTOR2_HPP
  Vec2f( const sf::Vector2f& v);//copy construct
#endif
  float GetMagnitude();
  Vec2f Normalized();
  static Vec2f Lerp(const Vec2f& v1, const Vec2f& v2, float t);
  static float AngleBetween(const Vec2f& v1, const Vec2f& v2);
  static float Dot(const Vec2f& v1, const Vec2f& v2);

  bool operator==(const Vec2f &rhs) const;
  bool operator!=(const Vec2f &rhs) const;

  Vec2f operator+(const Vec2f& rhs) const;
  Vec2f& operator+=(const Vec2f& rhs);
  Vec2f operator-(const Vec2f& rhs) const;

  Vec2f& operator-=(const Vec2f& rhs);
  Vec2f operator*(float rhs) const;
  Vec2f operator/(float rhs) const;

  operator sf::Vector2f() const;


};


class Vec2i
{
public:
	int x;
	int y;

	Vec2i(int x, int y);
	Vec2i();
#ifdef SFML_VECTOR2_HPP
	Vec2i(const sf::Vector2f& v);//copy construct
#endif
	static Vec2i Lerp(const Vec2i& v1, const Vec2i& v2, float t);
	static int Dot(const Vec2i& v1, const Vec2i& v2);

	bool operator==(const Vec2i &rhs) const;
	bool operator!=(const Vec2i &rhs) const;

	Vec2i operator+(const Vec2i& rhs) const;
	Vec2i& operator+=(const Vec2i& rhs);
	Vec2i operator-(const Vec2i& rhs) const;

	Vec2i& operator-=(const Vec2i& rhs);
	Vec2i operator*(int rhs) const;
	Vec2i operator/(int rhs) const;

	operator sf::Vector2f() const;


};

}
#endif //SFGE_VECTOR_H
