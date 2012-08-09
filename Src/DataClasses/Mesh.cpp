
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdlib.h>
#include <memory.h>

#include "main.h"
#include "Mesh.h"
#include "Vec3f.h"
#include "Matrix4x4.h"

extern IDirect3D9 *g_pD3D;
extern IDirect3DDevice9 *g_pd3dDevice;

void WINAPI FillFuncMesh(D3DXVECTOR4* pOut, CONST D3DXVECTOR2* pTexCoord, CONST D3DXVECTOR2* pTexelSize, LPVOID pData);

Mesh::Mesh()
{
}

void Mesh::SetVertexBuffer(CUSTOMVERTEX *v, unsigned int count)
{
    // create a vertex buffer interface called v_buffer
    g_d3ddev->CreateVertexBuffer(count*sizeof(CUSTOMVERTEX),
                               0,
                               D3DFVF_CUSTOMVERTEX,
                               D3DPOOL_MANAGED,
                               &m_vbuffer,
                               NULL);
    // lock v_buffer and load the vertices into it
    VOID* pVoid;    // a void pointer
    m_vbuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, v, sizeof(v));
    m_vbuffer->Unlock();
}

void Mesh::SetTexture(Tex *t)
{
	D3DXCreateTexture(g_d3ddev, t->m_mipmaps[0].m_width, t->m_mipmaps[0].m_height,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_texture);
	D3DXFillTexture(m_texture, FillFuncMesh, t);
}

void Mesh::SetIndexBuffer(int *i, unsigned int count)
{
    // create a index buffer interface called i_buffer
    g_d3ddev->CreateIndexBuffer(6*sizeof(int),
                              0,
                              D3DFMT_INDEX16,
                              D3DPOOL_MANAGED,
							  &m_ibuffer,
                              NULL);
    // lock i_buffer and load the indices into it
    VOID* pVoid;    // a void pointer
    m_ibuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, i, sizeof(i));
    m_ibuffer->Unlock(); 
}

void WINAPI FillFuncMesh(D3DXVECTOR4* pOut, CONST D3DXVECTOR2* pTexCoord, CONST D3DXVECTOR2* pTexelSize, LPVOID pData)
{
	Tex * t = (Tex *) pData;
	int coord = t->m_mipmaps[0].m_width*((int)(pTexCoord->y*t->m_mipmaps[0].m_height)) + ((int)(pTexCoord->x*t->m_mipmaps[0].m_width));
	pOut->w = 0.9f;//(((gs_titleBackg->m_bitmap[coord] >> 24) & 0xFF) / 255.0f);
	pOut->x = ((t->m_mipmaps[0].m_bitmap[coord] >> 16) & 0xFF) / 255.0f;
	pOut->y = ((t->m_mipmaps[0].m_bitmap[coord] >>  8) & 0xFF) / 255.0f;
	pOut->z = ((t->m_mipmaps[0].m_bitmap[coord]      ) & 0xFF) / 255.0f;
}
