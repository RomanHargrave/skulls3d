
#ifndef SWE_R002WIREFRAME
#define SWE_R002WIREFRAME

#include "R000Virtual.h"

/**
 * WireFrame Renderer
 */
class R002WireFrame : public R000Virtual
{
public:
	R002WireFrame(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
	void Bresenham(const Vec3f & v0, const Vec3f & v1);
};

#endif // SWE_R002WIREFRAME
