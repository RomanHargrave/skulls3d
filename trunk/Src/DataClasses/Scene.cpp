
#include "Scene.h"

Scene::Scene()
{
}

void Scene::AddMesh(Mesh *m)
{
	m_meshes.push_back(m);
}