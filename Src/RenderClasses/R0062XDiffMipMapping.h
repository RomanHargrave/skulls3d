
#ifndef SWE_R0062XDIFFMIPMAPPING
#define SWE_R0062XDIFFMIPMAPPING

#include "R000Virtual.h"

/**
 * Texturing Renderer with flawed MipMapping
 * Mipmap selected depending on Z, this is wrong.
 */
class R0062XDiffMipMapping : public R000Virtual
{
public:
	R0062XDiffMipMapping(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
	void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
				  Vec3f t0, Vec3f t1, Vec3f t2, Texture *t);
	void FillOrderedPoly(Vec3f v0, Vec3f  v1, Vec3f v2, Vec3f t0, Vec3f  t1, Vec3f t2, Texture *t);
};

#endif // SWE_R0062XDIFFMIPMAPPING
