
#ifndef SWE_VEC3F
#define SWE_VEC3F

#include "main.h"
#include "Matrix4x4.h"

class Vec2f;

class Vec3f
{
public:
	float x,y,z;

	Vec3f();
	Vec3f(const Vec2f & other);
	Vec3f(const CUSTOMVERTEX &other);
	Vec3f(float x, float z);

	float Len() const;
	void Normalize();
	float Dot(const Vec3f &other) const;
	Vec3f Cross(const Vec3f &other) const;
	Vec3f operator*(const Matrix4x4 & m) const;
	Vec3f operator/(float f) const;
	Vec3f operator*(float f) const;
	Vec3f operator+(const Vec3f & other) const;
	Vec3f operator+=(const Vec3f & other);
	Vec3f operator-(const Vec3f & other) const;
	Vec3f operator/(const Vec3f & other) const;
	Vec3f Mul3x3(const Matrix4x4 & m) const;
};

#endif // SWE_VEC3f
