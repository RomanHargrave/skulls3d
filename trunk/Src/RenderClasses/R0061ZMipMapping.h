
#ifndef SWE_R0061ZMIPMAPPING
#define SWE_R0061ZMIPMAPPING

#include "R000Virtual.h"

/**
 * Texturing Renderer with flawed MipMapping
 * Mipmap selected depending on Z, this is wrong.
 */
class R0061ZMipMapping : public R000Virtual
{
public:
	R0061ZMipMapping(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
	void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
				  Vec3f t0, Vec3f t1, Vec3f t2, Texture *t);
	void FillOrderedPoly(Vec3f v0, Vec3f  v1, Vec3f v2, Vec3f t0, Vec3f  t1, Vec3f t2, Texture *t);
};

#endif // SWE_R0061ZMIPMAPPING
