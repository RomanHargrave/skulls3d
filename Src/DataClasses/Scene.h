
#ifndef SWE_SCENE
#define SWE_SCENE

#include <list>

#include "Mesh.h"
#include "Matrix4x4.h"

class Scene
{
public:
	std::list<Mesh*> m_meshes;

	Scene();
	void AddMesh(Mesh *m);
};

#endif // SWE_SCENE