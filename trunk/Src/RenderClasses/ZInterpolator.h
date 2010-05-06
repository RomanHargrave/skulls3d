
#ifndef SWE_ZINTERPOLATOR
#define SWE_ZINTERPOLATOR

#include "Vec3f.h"

class ZInterpolator
{
public:
	float alpha, alphastep;
	Vec3f u0z0, u1z1;
	float invz0, invz1;
	Vec3f ualpha;	
	Vec3f topalpha, topstep;
	float bottomalpha, bottomstep;

	enum MajorType
	{
		VERTICAL,
		HORIZONTAL
	};

	ZInterpolator();
	virtual void Init(const MajorType & mt, const Vec3f & v0, const Vec3f & v1, const Vec3f & t0, const Vec3f & t1);
	inline void DisplaceStartingPoint(const float & move) {
		topalpha += topstep * move;
		bottomalpha += bottomstep * move;
		ualpha = topalpha / bottomalpha;
	}
	inline void Step() {
		topalpha += topstep;
		bottomalpha += bottomstep;
		ualpha = topalpha / bottomalpha;
	}
};

// ZInterpolator with Quake optimization
class QInterpolator : public ZInterpolator
{
public:
	Vec3f ualphastep;
	unsigned int quake;

	inline void DisplaceStartingPoint(const float & move) {
		topalpha += topstep * move;
		bottomalpha += bottomstep * move;
		ualpha = topalpha / bottomalpha;
		
		topstep = topstep * 16.0f;
		bottomstep = bottomstep * 16.0f;
		topalpha += topstep;
		bottomalpha += bottomstep;
		ualphastep = ((topalpha/bottomalpha) - ualpha) / 16.0f;
		quake = 15;
	}
	inline void DisplaceStartingPoint(const float & move, int stepcount) {
		if (stepcount > 16)
			stepcount = 16;
		topalpha += topstep * move;
		bottomalpha += bottomstep * move;
		ualpha = topalpha / bottomalpha;
		topstep = topstep * (float)stepcount;
		bottomstep = bottomstep * (float)stepcount;
		topalpha += topstep;
		bottomalpha += bottomstep;
		ualphastep = ((topalpha/bottomalpha) - ualpha) / (float)stepcount;
		quake = stepcount - 1;
	}
	inline void Step() {
		if (quake == 0) {
			ualpha = topalpha / bottomalpha;
			topalpha += topstep;
			bottomalpha += bottomstep;
			ualphastep = ((topalpha/bottomalpha) - ualpha) / 16.0f;
			quake = 15;
		} else {
			ualpha += ualphastep;
			quake--;
		}
	}
};

#endif // SWE_ZINTERPOLATOR
