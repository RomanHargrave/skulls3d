
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>

#include "main.h"
#include "InGame.h"
#include "doom/WadFile.h"
#include "doom/lumps/PatchLump.h"
#include "doom/lumps/LevelLump.h"
#include "doom/SideDef.h"
#include "doom/LineDef.h"
#include "doom/Sector.h"
#include "doom/Texture.h"
#include "doom/Thing.h"
#include "Mesh.h"



InGame::InGame(doom::LevelLump * level)
{
	m_level = level;
	m_scene = new Scene();

	int count = (unsigned int) level->m_sideDefs.size();
	for (int i=0 ; i<count ; i++)
	{
		doom::SideDef *sideDef = level->m_sideDefs[i];
		doom::Vertex *v0, *v1;
		doom::Sector *sector, *otherSector;
		sector = sideDef->m_sector;
		otherSector = NULL;
		if (sideDef == sideDef->m_lineDef->m_rightSideDef)
		{
			v0 = sideDef->m_lineDef->m_start_vtx;
			v1 = sideDef->m_lineDef->m_end_vtx;
			if (sideDef->m_lineDef->m_leftSideDef != NULL)
				otherSector = sideDef->m_lineDef->m_leftSideDef->m_sector;
		}
		else
		{
			v1 = sideDef->m_lineDef->m_start_vtx;
			v0 = sideDef->m_lineDef->m_end_vtx;
			if (sideDef->m_lineDef->m_rightSideDef != NULL)
				otherSector = sideDef->m_lineDef->m_rightSideDef->m_sector;
		}
		if (otherSector == NULL)
		{	
			if (sideDef->m_middleTexture == NULL)
				continue;
			Mesh *mesh = CreateMesh(*v0, *v1,
			                        sector->m_floorHeight,
			                        sector->m_ceilingHeight,
			                        new Tex(sideDef->m_middleTexture->m_bitmap, sideDef->m_middleTexture->m_w, sideDef->m_middleTexture->m_h)
			                       );
			m_scene->AddMesh(mesh);
		}
		else
		{
			if (sector->m_floorHeight < otherSector->m_floorHeight)
			{
				if (sideDef->m_lowerTexture == NULL)
					continue;
				Mesh *mesh = CreateMesh(*v0, *v1,
										sector->m_floorHeight,
										otherSector->m_ceilingHeight,
										new Tex(sideDef->m_lowerTexture->m_bitmap, sideDef->m_lowerTexture->m_w, sideDef->m_lowerTexture->m_h)
									   );
				m_scene->AddMesh(mesh);
			}
			if (sector->m_ceilingHeight > otherSector->m_ceilingHeight)
			{
				if (sideDef->m_upperTexture == NULL)
					continue;
				Mesh *mesh = CreateMesh(*v0, *v1,
										otherSector->m_floorHeight,
										sector->m_ceilingHeight,
										new Tex(sideDef->m_upperTexture->m_bitmap, sideDef->m_upperTexture->m_w, sideDef->m_upperTexture->m_h)
									   );
				m_scene->AddMesh(mesh);
			}
		}
	}
}

void InGame::Render()
{
    g_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	g_d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    g_d3ddev->BeginScene();

    g_d3ddev->SetFVF(D3DFVF_CUSTOMVERTEX);

	// find player spawn spot coordinates
	float x = (float) m_level->m_things[0]->m_x;
	float z = (float) m_level->m_things[0]->m_z;

    // set the view transform
    D3DXMATRIX matView;    // the view transform matrix
    D3DXMatrixLookAtLH(&matView,
    &D3DXVECTOR3 (x, 0.0f,   z),    // the camera position
    &D3DXVECTOR3 (0.0f, 0.0f, 1.0f),      // the look-at position
    &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction
    g_d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

    // set the projection transform
    D3DXMATRIX matProjection;    // the projection transform matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               0.01f,   // the near view-plane
                               10000.0f);    // the far view-plane
    g_d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); // set the projection

    // set the world transform
    //static float index = 0.0f; index+=0.01f; // an ever-increasing float value
    //D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
    //D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix
    //g_d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY));    // set the world transform

	for (std::list<Mesh*>::iterator it=m_scene->m_meshes.begin() ; it!=m_scene->m_meshes.end() ; it++)
	{
		Mesh *mesh = (Mesh*)(*it);
		g_d3ddev->SetStreamSource(0, mesh->m_vbuffer, 0, sizeof(CUSTOMVERTEX));
		g_d3ddev->SetIndices(mesh->m_ibuffer);
		g_d3ddev->SetTexture( 0, mesh->m_texture );
		g_d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	}

    g_d3ddev->EndScene(); 
    g_d3ddev->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI InGame::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

Mesh* InGame::CreateMesh(doom::Vertex v0, doom::Vertex v1, int low, int high, Tex *tex)
{
	float wallwidth = (float)sqrt((float)(v1.m_x-v0.m_x)*(v1.m_x-v0.m_x) + (v1.m_z-v0.m_z)*(v1.m_z-v0.m_z));
	float wallheight = (float)high - low;

	float umax = wallwidth / tex->m_mipmaps[0].m_width;
	float vmax = wallheight / tex->m_mipmaps[0].m_height;

	Mesh *mesh = new Mesh();
	mesh->SetTexture(tex);
	CUSTOMVERTEX *vb = new CUSTOMVERTEX[4];
	vb[0].x = (float) v0.m_x; vb[0].y = (float) low;  vb[0].z = (float) v0.m_z;
	vb[0].u = 0.0f; vb[0].v = vmax;
	vb[1].x = (float) v1.m_x; vb[1].y = (float) low;  vb[1].z = (float) v1.m_z;
	vb[1].u = umax; vb[1].v = vmax;
	vb[2].x = (float) v0.m_x; vb[2].y = (float) high; vb[2].z = (float) v0.m_z;
	vb[2].u = 0.0f; vb[2].v = 0.0f;
	vb[3].x = (float) v1.m_x; vb[3].y = (float) high; vb[3].z = (float) v1.m_z;
	vb[3].u = umax; vb[3].v = 0.0f;
	mesh->SetVertexBuffer(vb, 4);

	int *ib = new int[6];
	ib[0] = 0; ib[1] = 1; ib[2] = 3;
	ib[3] = 0; ib[4] = 3; ib[5] = 2;
	mesh->SetIndexBuffer(ib, 6);

	return mesh;
}