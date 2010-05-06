
#ifndef SWE_RENDERER
#define SWE_RENDERER

#include "Vec3f.h"
#include "Tex.h"
#include "Scene.h"
#include "Camera.h"
#include "ViewPort.h"

class Renderer
{
private:
	void FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
				  Vec3f t0, Vec3f t1, Vec3f t2, Tex *t, float shade);
	void FillOrderedPoly(Vec3f v0, Vec3f  v1, Vec3f v2, Vec3f t0, Vec3f  t1, Vec3f t2, Tex *t, float shade);

public:
	Scene *m_scene;
	Camera *m_camera;
	ViewPort *m_viewport;
	float *m_zbuffer;

	Renderer(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
};

#endif // SWE_RENDERER