
#ifndef SWE_VEC2F
#define SWE_VEC2F

class Vec3f;

class Vec2f
{
public:
	float x,y;

	Vec2f();
	Vec2f(const Vec3f & other);

	Vec2f & operator=(const Vec3f & other);
	Vec2f operator/(float f) const;
	Vec2f operator*(float f) const;
	Vec2f operator+(const Vec2f & other) const;
	Vec2f operator+=(const Vec2f & other);
	Vec2f operator-(const Vec2f & other) const;
	Vec2f operator/(const Vec2f & other) const;
};

#endif // SWE_VEC2F
