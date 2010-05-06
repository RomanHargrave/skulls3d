
#include <Math.h>
#include "Vec2f.h"
#include "Vec3f.h"

Vec2f::Vec2f()
{
	x = y = 0;
}

Vec2f::Vec2f(const Vec3f & other)
{
	this->x = other.x;
	this->y = other.y;
}

Vec2f & Vec2f::operator=(const Vec3f & other)
{
	this->x = other.x;
	this->y = other.y;
	return *this;
}

Vec2f Vec2f::operator-(const Vec2f & other) const
{
	Vec2f result;
	result.x = this->x - other.x;
	result.y = this->y - other.y;
	return result;
}

Vec2f Vec2f::operator/(float f) const
{
	Vec2f result;
	result.x = this->x / f;
	result.y = this->y / f;
	return result;
}

Vec2f Vec2f::operator*(float f) const
{
	Vec2f result;
	result.x = this->x * f;
	result.y = this->y * f;
	return result;
}

Vec2f Vec2f::operator+(const Vec2f & other) const
{
	Vec2f result;
	result.x = this->x + other.x;
	result.y = this->y + other.y;
	return result;
}
Vec2f Vec2f::operator+=(const Vec2f & other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vec2f Vec2f::operator/(const Vec2f & other) const
{
	Vec2f result;
	result.x = this->x / other.x;
	result.y = this->y / other.y;
	return result;
}