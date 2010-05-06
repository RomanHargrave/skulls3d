
#ifndef SWE_R0031CUSTOM
#define SWE_R0031CUSTOM

#include "R0030FlatVirtual.h"

class R0031Custom : public R0030FlatVirtual
{
public:
	R0031Custom(Scene *scene, Camera *camera, ViewPort *viewport);
	void FillOrderedPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade);
};

#endif // SWE_R0031CUSTOM