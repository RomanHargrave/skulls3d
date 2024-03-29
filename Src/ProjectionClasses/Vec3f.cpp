
#include <Math.h>
#include "Vec3f.h"
#include "Vec2f.h"

Vec3f::Vec3f()
{
	x = y = z = 0;
}
Vec3f::Vec3f(const Vec2f & other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = 0;
}
Vec3f::Vec3f(const CUSTOMVERTEX & other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}
Vec3f::Vec3f(float x, float z)
{
	this->x = x;
	this->z = z;
}

float Vec3f::Len() const
{
	return (float) sqrt(x*x+y*y+z*z);
}

void Vec3f::Normalize()
{
	float len = Len();
	x /= len;
	y /= len;
	z /= len;
}

float Vec3f::Dot(const Vec3f &other) const
{
	return x*other.x + y*other.y + z*other.z;
}

Vec3f Vec3f::Cross(const Vec3f &other) const
{
	Vec3f result;
	result.x = y*other.z - z*other.y;
	result.y = z*other.x - x*other.z;
	result.z = x*other.y - y*other.x;
	return result;
}

Vec3f Vec3f::operator-(const Vec3f & other) const
{
	Vec3f result;
	result.x = this->x - other.x;
	result.y = this->y - other.y;
	result.z = this->z - other.z;
	return result;
}

Vec3f Vec3f::operator * (const Matrix4x4 & m) const
{
	Vec3f result;
	result.x = m.m_data[0]*x + m.m_data[1]*y + m.m_data[2]*z + m.m_data[3];
	result.y = m.m_data[4]*x + m.m_data[5]*y + m.m_data[6]*z + m.m_data[7];
	result.z = m.m_data[8]*x + m.m_data[9]*y + m.m_data[10]*z + m.m_data[11];
	return result;
}

Vec3f Vec3f::Mul3x3(const Matrix4x4 & m) const
{
	Vec3f result;
	result.x = m.m_data[0]*x + m.m_data[1]*y + m.m_data[2]*z;
	result.y = m.m_data[4]*x + m.m_data[5]*y + m.m_data[6]*z;
	result.z = m.m_data[8]*x + m.m_data[9]*y + m.m_data[10]*z;
	return result;
}

Vec3f Vec3f::operator/(float f) const
{
	Vec3f result;
	result.x = this->x / f;
	result.y = this->y / f;
	result.z = this->z / f;
	return result;
}

Vec3f Vec3f::operator*(float f) const
{
	Vec3f result;
	result.x = this->x * f;
	result.y = this->y * f;
	result.z = this->z * f;
	return result;
}

Vec3f Vec3f::operator+(const Vec3f & other) const
{
	Vec3f result;
	result.x = this->x + other.x;
	result.y = this->y + other.y;
	result.z = this->z + other.z;
	return result;
}
Vec3f Vec3f::operator+=(const Vec3f & other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vec3f Vec3f::operator/(const Vec3f & other) const
{
	Vec3f result;
	result.x = this->x / other.x;
	result.y = this->y / other.y;
	result.z = this->z + other.z;
	return result;
}