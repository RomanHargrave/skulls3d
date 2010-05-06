
#ifndef SWE_R010MULTICORE
#define SWE_R010MULTICORE

#include "R000Virtual.h"

/**
 * Texturing with antialiasing
 */
class R010MultiCore : public R000Virtual
{
public:
	float *m_zbuffer;
	R010MultiCore(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
	void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
				  Vec3f t0, Vec3f t1, Vec3f t2, Tex *t, float shade);
	void FillOrderedPoly(Vec3f v0, Vec3f  v1, Vec3f v2, Vec3f t0, Vec3f  t1, Vec3f t2, Tex *t, float shade);
};

#endif // SWE_R010MULTICORE