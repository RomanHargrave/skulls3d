
#ifndef SWE_R0033AVG
#define SWE_R0033AVG

#include "R0030FlatVirtual.h"

class R0033Avg : public R0030FlatVirtual
{
public:
	R0033Avg(Scene *scene, Camera *camera, ViewPort *viewport);
	void FillOrderedPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade);
};

#endif // SWE_R0033AVG