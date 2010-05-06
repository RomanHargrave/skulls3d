
#ifndef SWE_R007BILINEAR
#define SWE_R007BILINEAR

#include "R000Virtual.h"

/**
 * Texturing Renderer with Bilinear Filtering
 */
class R007Bilinear : public R000Virtual
{
public:
	R007Bilinear(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
	void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
				  Vec3f t0, Vec3f t1, Vec3f t2, Tex *t, unsigned char shade);
	void FillOrderedPoly(Vec3f v0, Vec3f  v1, Vec3f v2, Vec3f t0, Vec3f  t1, Vec3f t2, Tex *t, unsigned char shade);
};

#endif // SWE_R007BILINEAR