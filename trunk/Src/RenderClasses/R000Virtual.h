
#ifndef SWE_RENDERER
#define SWE_RENDERER

#include "Camera.h"
#include "Scene.h"
#include "ViewPort.h"

/**
 * Default Renderer
 * Straight, no projection, only points
 */
class R000Virtual
{
public:
	Scene *m_scene;
	Camera *m_camera;
	ViewPort *m_viewport;

	R000Virtual(Scene *scene, Camera *camera, ViewPort *viewport);

	virtual void Render() = 0;
};

#endif // SWE_RENDERER