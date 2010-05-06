
#include "Scene.h"

Scene::Scene()
{
	m_meshCount = 0;
}

void Scene::AddMesh(Mesh *m)
{
	m->Prepare();
	this->m_meshes[this->m_meshCount] = m;
	this->m_meshCount++;
}