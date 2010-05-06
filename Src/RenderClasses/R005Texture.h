
#ifndef SWE_R005TEXTURE
#define SWE_R005TEXTURE

#include "R000Virtual.h"
#include "Texture.h"

/**
 * Texturing Renderer with Bresenham algorithm for lines
 * Uses real texture (with a bitmap)
 */
class R005Texture : public R000Virtual
{
public:
	R005Texture(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
	void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
				  Vec3f t0, Vec3f t1, Vec3f t2, Texture *t);
	void FillOrderedPoly(Vec3f v0, Vec3f  v1, Vec3f v2, Vec3f t0, Vec3f  t1, Vec3f t2, Texture *t);
};

#endif // SWE_R005TEXTURE