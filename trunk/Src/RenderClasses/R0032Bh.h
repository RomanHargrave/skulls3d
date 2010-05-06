
#ifndef SWE_R0032BH
#define SWE_R0032BH

#include "R0030FlatVirtual.h"

class R0032Bh : public R0030FlatVirtual
{
public:
	R0032Bh(Scene *scene, Camera *camera, ViewPort *viewport);
	void FillOrderedPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade);
};

#endif // SWE_R0032BH