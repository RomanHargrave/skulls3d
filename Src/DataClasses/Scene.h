
#ifndef SWE_SCENE
#define SWE_SCENE

#include "Mesh.h"
#include "Matrix4x4.h"

class Scene
{
public:
	Mesh *m_meshes[10000];
	int m_meshCount;

	Scene();
	void AddMesh(Mesh *m);
};

#endif // SWE_SCENE