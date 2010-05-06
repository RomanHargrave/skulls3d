
#ifndef SWE_R009ANTIALIASING
#define SWE_R009ANTIALIASING

#include "R000Virtual.h"

/**
 * Texturing with antialiasing
 */
class R009Antialiasing : public R000Virtual
{
public:
	float *m_zbuffer;
	R009Antialiasing(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
	void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
				  Vec3f t0, Vec3f t1, Vec3f t2, Tex *t, float shade);
	void FillOrderedPoly(Vec3f v0, Vec3f  v1, Vec3f v2, Vec3f t0, Vec3f  t1, Vec3f t2, Tex *t, float shade);
};

#endif // SWE_R009ANTIALIASING