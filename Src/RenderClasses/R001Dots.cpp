
#include <windows.h>

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "R001Dots.h"
#include "MatrixStack.h"

R001Dots::R001Dots(Scene *scene, Camera *camera, ViewPort *viewport)
:R000Virtual(scene, camera, viewport)
{
}


void R001Dots::Render()
{
	m_viewport->Clear();
	MatrixStack mstack;

	// TODO: sort meshes

	mstack.PushMatrix(m_camera->m_viewmatrix);

	for (int i=0 ; i<m_scene->m_meshCount ; i++)
	{
		Mesh *mesh = m_scene->m_meshes[i];
		mstack.PushMatrix(mesh->m_worldmatrix);

		Matrix4x4 finalmatrix = mstack.GetTopMatrix();

		for (unsigned int s=0 ; s<mesh->m_stripcount ; s++)
		{
			Strip *strip = mesh->m_strips[s];
			for (unsigned int v=0 ; v<strip->m_indexcount ; v++)
			{
				Vec3f vec = mesh->m_vertexbuffer[ strip->m_indexbuffer[v] ];
				
				vec = vec * finalmatrix;
				if (vec.z > 0)
				{
					vec = vec / vec.z;
					vec = vec * m_viewport->m_viewportmatrix;
					m_viewport->ShowPoint((int)vec.x, (int)vec.y, (unsigned char)200);
				}
			}
		}
		mstack.PopMatrix();
	}
}
