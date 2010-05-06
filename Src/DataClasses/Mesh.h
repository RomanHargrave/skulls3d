
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
	CUSTOMVERTEX *m_vb;
	unsigned int m_vertexcount;
	Matrix4x4 m_worldmatrix;
	Tex *m_texture;
	Tex *m_bumpmap;

	Strip **m_strips;          // array of index buffers (triangle strips)
	unsigned int m_stripcount; // number of strips
	Vec3f *m_stripnorms;          // 1 normal per triangle in strips
	unsigned int m_striptricount; // number of triangles in strips

	Strip **m_fans;            // array of index buffers (triangle strips)
	unsigned int m_fancount;   // number of fans
	Vec3f *m_fannorms;            // 1 normal per triangle in strips
	unsigned int m_fantricount;   // number of triangles in fans

	// 3 neighbours per triangle
	// Indices are triangles in m_stripnorms and m_fannorms
	// Indices consider m_stripnorms and m_fannorms as one array: if i>m_striptricount,
	// i -= m_striptricount, effectively becoming an index in m_fantricount
	unsigned int *m_stripneighbours;
	unsigned int *m_fanneighbours;

	Mesh();
	void SetVertexBuffer(Vec3f *v, unsigned int count);
	void SetTexture(Tex *t);
	void AddStrip(unsigned int *indexbuffer, Vec2f *texturebuffer, unsigned int indexcount);
	void AddFan(unsigned int *indexbuffer, Vec2f *texturebuffer, unsigned int indexcount);
	void SetBumpMap(Tex *t);
	void Prepare();
	unsigned int FindNeighbour(unsigned int vindex1, unsigned int vindex2, unsigned int triangle);
};

#endif SWE_MESH
