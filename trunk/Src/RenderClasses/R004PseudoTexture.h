
#ifndef SWE_R004PSEUDOTEXTURE
#define SWE_R004PSEUDOTEXTURE

#include "R000Virtual.h"

/**
 * Texturing Renderer, generates texture on the fly (no bitmap)
 */
class R004PseudoTexture : public R000Virtual
{
public:
	R004PseudoTexture(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
	void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade);
	void FillOrderedPoly(Vec3f v0, Vec3f  v1, Vec3f v2, Vec3f t0, Vec3f  t1, Vec3f t2, unsigned char shade);
};

#endif // SWE_SWE_R004PSEUDOTEXTURE
