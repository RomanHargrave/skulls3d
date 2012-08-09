
#ifndef SWE_MESH
#define SWE_MESH

#include <windows.h>
#include <d3d9.h>
#include "Vec3f.h"
#include "Vec2f.h"
#include "Matrix4x4.h"
#include "Tex.h"

class Mesh;

typedef struct strip
{
public:
	unsigned int *m_indexbuffer;
	Vec2f *m_texturebuffer;
	unsigned int m_indexcount;
} Strip;


class Mesh
{
public:
	LPDIRECT3DVERTEXBUFFER9 m_vbuffer;
	LPDIRECT3DINDEXBUFFER9 m_ibuffer;
	LPDIRECT3DTEXTURE9 m_texture;

	Mesh();
	void SetTexture(Tex *t);
	void SetVertexBuffer(CUSTOMVERTEX *v, unsigned int count);
	void SetIndexBuffer(int *i, unsigned int count);
};

#endif SWE_MESH
