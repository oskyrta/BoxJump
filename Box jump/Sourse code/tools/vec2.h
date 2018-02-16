#pragma once
/////////////////////////////////////////////////
// Include
#include <SFML\Graphics.hpp>

/////////////////////////////////////////////////
// Class Vec2
class Vec2
{
public:
	Vec2() : x(0), y(0) {};
	Vec2(float X, float Y) : x(X), y(Y) {};
	Vec2(sf::Vector2f vec) : x(vec.x), y(vec.y) {};

	Vec2 operator+(const Vec2& a);
	Vec2 operator-(const Vec2& a);
	Vec2 operator+=(const Vec2& a);
	Vec2 operator*(const float& a);
	Vec2 operator*(const Vec2& a);
	Vec2 operator/(const float& a);
	Vec2 operator/(const Vec2& a);
	bool operator==(const Vec2& a);

	sf::Vector2f getSFVector();

	Vec2 normalize();
	Vec2 magnitude(float ragius);
	Vec2 getInt() { return Vec2((int)(x +.5f), (int)(y+.5f)); };
	float length();

	float x;
	float y;
};

