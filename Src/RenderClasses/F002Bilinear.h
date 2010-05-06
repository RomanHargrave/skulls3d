
#ifndef SWE_F002BILINEAR
#define SWE_F002BILINEAR

#include "Vec3f.h"
#include "Tex.h"
#include "ViewPort.h"

/**
 * Poly Filler withour texture artefacts.
 */
class F002Bilinear
{
	Vec3f v0, v1, v2;
	Vec3f t0, t1, t2;
	Tex *t;
	ViewPort * m_viewport;

public:
	F002Bilinear(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
                         const Vec3f & t0, const Vec3f & t1, const Vec3f & t2,
                         Tex *t, ViewPort * vp);
	void FillPoly(unsigned char shade, float *zbuffer);
};


#endif // SWE_F002BILINEAR
