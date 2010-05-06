
#ifndef SWE_F004ANTIALIASING
#define SWE_F004ANTIALIASING

#include "Vec3f.h"
#include "Tex.h"
#include "ViewPort.h"

/**
 * Poly Filler withour texture artefacts.
 */
class F004Antialiasing
{
	/*
	Vec3f v0, v1, v2;
	Vec3f t0, t1, t2;
	Texture *m_t;
	Texture *m_tb;
	ViewPort * m_viewport;
	*/

public:
	static void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
                         const Vec3f & t0, const Vec3f & t1, const Vec3f & t2,
						 bool neighb0visible, bool neighb1visible, bool neighb2visible, 
                         Tex *t, Tex *tb, ViewPort * vp,
	                     const Vec3f & facenormal, float *zbuffer);
	//void FillPoly(const Vec3f & facenormal, float *zbuffer);
};


#endif // SWE_F004ANTIALIASING
