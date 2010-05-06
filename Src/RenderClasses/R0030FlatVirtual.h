
#ifndef SWE_R0030FLATVIRTUAL
#define SWE_R0030FLATVIRTUAL

#include "R000Virtual.h"

/**
 * Flat Renderer, no shading
 */
class R0030FlatVirtual : public R000Virtual
{
public:
	R0030FlatVirtual(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
	void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade);
	virtual void FillOrderedPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade) = 0;
};

#endif // SWE_R0030FLATVIRTUAL
