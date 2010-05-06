
#ifndef SWE_F003FAKEBUMP
#define SWE_F003FAKEBUMP

#include "Vec3f.h"
#include "Tex.h"
#include "ViewPort.h"

/**
 * Poly Filler withour texture artefacts.
 */
class F003FakeBump
{
	Vec3f v0, v1, v2;
	Vec3f t0, t1, t2;
	Tex *m_t;
	Tex *m_tb;
	ViewPort * m_viewport;

public:
	F003FakeBump(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
                 const Vec3f & t0, const Vec3f & t1, const Vec3f & t2,
                 Tex *t, Tex *tb, ViewPort * vp);
	void FillPoly(const Vec3f & facenormal, float *zbuffer);
};


#endif // SWE_F003FAKEBUMP
