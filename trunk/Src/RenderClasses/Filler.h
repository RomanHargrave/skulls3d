
#ifndef SWE_FILLER
#define SWE_FILLER

#include "Vec3f.h"
#include "Tex.h"
#include "ViewPort.h"


class Filler
{
public:
	static void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
                         const Vec3f & t0, const Vec3f & t1, const Vec3f & t2,
						 bool neighb0visible, bool neighb1visible, bool neighb2visible, 
                         Tex *t, Tex *tb, ViewPort * vp,
	                     const Vec3f & facenormal, float *zbuffer);
};


#endif // SWE_FILLER
