
#include <windows.h>
#include <d3d9.h>
#include <stdlib.h>
#include <memory.h>

#include "main.h"
#include "Mesh.h"
#include "Vec3f.h"
#include "Matrix4x4.h"

extern IDirect3D9 *g_pD3D;
extern IDirect3DDevice9 *g_pd3dDevice;

Mesh::Mesh()
{
	this->m_strips = 0;
	this->m_fans = 0;
	this->m_vertexcount = 0;
	this->m_worldmatrix.SetIdentity();
	this->m_texture = 0;
	this->m_bumpmap = 0;

	this->m_stripcount = 0;
	this->m_striptricount = 0;
	this->m_stripnorms = NULL;
	this->m_stripneighbours = NULL;

	this->m_fancount = 0;
	this->m_fantricount = 0;
	this->m_fannorms = NULL;
	this->m_fanneighbours = NULL;
}

void Mesh::SetVertexBuffer(Vec3f *v, unsigned int count)
{
	//if( FAILED( g_pd3dDevice->CreateVertexBuffer( count*sizeof(CUSTOMVERTEX),
    //     0 /*Usage*/, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_vb, NULL ) ) )
	//	return E_FAIL;
	//g_pd3dDevice->CreateVertexBuffer( count*sizeof(CUSTOMVERTEX),
    //     0 /*Usage*/, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_vb, NULL );

	//CUSTOMVERTEX *vb = (CUSTOMVERTEX*) m_vb;
	m_vb = new CUSTOMVERTEX[count];
	for (unsigned int i=0 ; i<count ; i++) {
		m_vb[i].x = v->x;
		m_vb[i].y = v->y;
		m_vb[i].z = v->z;
	}
	this->m_vertexcount = count;
}

void Mesh::SetTexture(Tex *t)
{
	this->m_texture = t;
}
void Mesh::SetBumpMap(Tex *bm)
{
	this->m_bumpmap = bm;
}

void Mesh::AddStrip(unsigned int *indexbuffer, Vec2f *texturebuffer, unsigned int indexcount)
{
	this->m_strips = (Strip**) realloc(this->m_strips, (this->m_stripcount+1) * sizeof(Strip*));
	this->m_strips[this->m_stripcount] = new Strip();
	this->m_strips[this->m_stripcount]->m_indexbuffer = new unsigned int[indexcount];
	this->m_strips[this->m_stripcount]->m_texturebuffer = new Vec2f[3*(indexcount-2)];
	this->m_strips[this->m_stripcount]->m_indexcount = indexcount;
	memcpy(this->m_strips[this->m_stripcount]->m_indexbuffer, indexbuffer, indexcount*sizeof(unsigned int));
	for (unsigned int i=0 ; i<3*(indexcount-2) ; i++)
		this->m_strips[this->m_stripcount]->m_texturebuffer[i] = texturebuffer[i];	
	this->m_stripcount++;
}

void Mesh::AddFan(unsigned int *indexbuffer, Vec2f *texturebuffer, unsigned int indexcount)
{
	this->m_fans = (Strip**) realloc(this->m_fans, (this->m_fancount+1) * sizeof(Strip*));
	this->m_fans[this->m_fancount] = new Strip();
	this->m_fans[this->m_fancount]->m_indexbuffer = new unsigned int[indexcount];
	this->m_fans[this->m_fancount]->m_texturebuffer = new Vec2f[3*(indexcount-2)];
	this->m_fans[this->m_fancount]->m_indexcount = indexcount;
	memcpy(this->m_fans[this->m_fancount]->m_indexbuffer, indexbuffer, indexcount*sizeof(unsigned int));
	for (unsigned int i=0 ; i<3*(indexcount-2) ; i++)
		this->m_fans[this->m_fancount]->m_texturebuffer[i] = texturebuffer[i];
	this->m_fancount++;
}

/**
 * Precalculates normals and finds neighbour triangles
 */
void Mesh::Prepare()
{
	// Triangle count in strips, then in fans
	// then index withing triangle total count used to set normals and neighbours
	unsigned int triangle = 0;

	// Counting triangles in strips
	for (unsigned int s=0 ; s<m_stripcount ; s++)
		triangle += m_strips[s]->m_indexcount - 2;
	this->m_striptricount = triangle;
	if (triangle != 0) {
		this->m_stripnorms = (Vec3f*) malloc(sizeof(Vec3f) * triangle);
		this->m_stripneighbours = (unsigned int *) malloc(sizeof(unsigned int) * triangle * 3);
	}

	triangle = 0;

	// Counting triangles in fans
	for (unsigned int f=0 ; f<m_fancount ; f++)
		triangle += m_fans[f]->m_indexcount - 2;
	this->m_fantricount = triangle;
	if (triangle != 0) {
		this->m_fannorms = (Vec3f*) malloc(sizeof(Vec3f) * triangle);
		this->m_fanneighbours = (unsigned int *) malloc(sizeof(unsigned int) * triangle * 3);
	}

	triangle = 0;
	CUSTOMVERTEX *vb = (CUSTOMVERTEX*) m_vb;
	// Finding neighbours for triangles in strips
	for (unsigned int s=0 ; s<m_stripcount ; s++)
	{
		Strip *strip = m_strips[s];

		// Preca normals for strips
		Vec3f v0,v1,v2;
		unsigned int vindex0, vindex1, vindex2;
		v0 = vb[strip->m_indexbuffer[0]];
		v1 = vb[strip->m_indexbuffer[1]];
		vindex0 = strip->m_indexbuffer[0];
		vindex1 = strip->m_indexbuffer[1];
		for (unsigned int i=2 ; i<strip->m_indexcount ; i++, triangle++, v0=v1, v1=v2, vindex0=vindex1, vindex1=vindex2)
		{
			v2 = vb[strip->m_indexbuffer[i]];
			vindex2 = strip->m_indexbuffer[i];
			
			m_stripnorms[triangle] = (((i&0x1)==0)?((v2-v0).Cross(v1-v0)):((v1-v0).Cross(v2-v0)));
			m_stripnorms[triangle].Normalize();
			
			// Neighbour 0
			if (i != 2) {
				m_stripneighbours[triangle*3] = triangle-1;
			} else {
				m_stripneighbours[triangle*3] = FindNeighbour(vindex0, vindex1, triangle);
			}

			// Neighbour 1
			if (i+1!=strip->m_indexcount) {
				m_stripneighbours[triangle*3+1] = triangle+1;
			} else {
				m_stripneighbours[triangle*3+1] = FindNeighbour(vindex1, vindex2, triangle);
			}

			// Neighbour 2
			m_stripneighbours[triangle*3+2] = FindNeighbour(vindex2, vindex0, triangle);
		}
	}
	
	// Finding neighbours for triangles in fans
	for (unsigned int s=0 ; s<m_fancount ; s++)
	{
		Strip *fan = m_fans[s];

		// Preca normals for strips
		Vec3f v0,v1,v2;
		unsigned int vindex0, vindex1, vindex2;
		v0 = vb[fan->m_indexbuffer[0]];
		v1 = vb[fan->m_indexbuffer[1]];
		vindex0 = fan->m_indexbuffer[0];
		vindex1 = fan->m_indexbuffer[1];
		for (unsigned int i=2 ; i<fan->m_indexcount ; i++, triangle++, v1=v2, vindex1=vindex2)
		{
			v2 = vb[fan->m_indexbuffer[i]];
			vindex2 = fan->m_indexbuffer[i];
			
			m_fannorms[triangle] = (v2-v0).Cross(v1-v0);
			m_fannorms[triangle].Normalize();
			
			// Neighbour 0
			if (i!=2) {
				m_fanneighbours[triangle*3] = triangle-1;
			} else {
				m_fanneighbours[triangle*3] = FindNeighbour(vindex0, vindex1, triangle);
			}

			// Neighbour 1
			m_fanneighbours[triangle*3+1] = FindNeighbour(vindex1, vindex2, triangle);
			
			// Neighbour 2
			if (i+1!=fan->m_indexcount) {
				m_fanneighbours[triangle*3+2] = triangle+1;
			} else {
				m_fanneighbours[triangle*3+2] = FindNeighbour(vindex2, vindex0, triangle);
			}
		}
	}
}

/**
 * Find triangle assiciated with both vertex, but not being the given triangle
 */
unsigned int Mesh::FindNeighbour(unsigned int vindex1, unsigned int vindex2, unsigned int isnot)
{
	unsigned int tri = 0;

	for (unsigned int s=0 ; s<m_stripcount ; s++)
	{
		Strip *strip = m_strips[s];
		unsigned int vi0, vi1, vi2;
		vi0 = strip->m_indexbuffer[0];
		vi1 = strip->m_indexbuffer[1];
		for (unsigned int i=2 ; i<strip->m_indexcount ; i++, vi0=vi1, vi1=vi2, tri++)
		{
			vi2 = strip->m_indexbuffer[i];

			if (tri == isnot) continue;
			if (vindex1 == vi0) {
				if (vindex2 == vi1 || vindex2 == vi2) {
					return tri;
				}
			} else if (vindex1 == vi1) {
				if (vindex2 == vi0 || vindex2 == vi2) {
					return tri;
				}
			} else if (vindex1 == vi2) {
				if (vindex2 == vi0 || vindex2 == vi1) {
					return tri;
				}
			}
		}
	}
	for (unsigned int s=0 ; s<m_fancount ; s++)
	{
		Strip *fan = m_fans[s];
		unsigned int vi0, vi1, vi2;
		vi0 = fan->m_indexbuffer[0];
		vi1 = fan->m_indexbuffer[1];
		for (unsigned int i=2 ; i<fan->m_indexcount ; i++, vi1=vi2, tri++)
		{
			vi2 = fan->m_indexbuffer[i];

			if (tri == isnot) continue;
			if (vindex1 == vi0) {
				if (vindex2 == vi1 || vindex2 == vi2) {
					return tri;
				}
			} else if (vindex1 == vi1) {
				if (vindex2 == vi0 || vindex2 == vi2) {
					return tri;
				}
			} else if (vindex1 == vi2) {
				if (vindex2 == vi0 || vindex2 == vi1) {
					return tri;
				}
			}
		}
	}
	return -1;
}
