
#include "main.h"
#include "RectangleTriangle.h"
#include "Vec2f.h"
#include "Vec3f.h"
#include "Tex.h"

RectangleTriangle::RectangleTriangle(float xlen, float ylen, Tex *t)
{
	Mesh::Mesh();
	CUSTOMVERTEX *vb = (CUSTOMVERTEX*) m_vb;

	this->m_texture = t;
	this->m_vertexcount = 3;
	this->m_vb = new CUSTOMVERTEX[3];
	vb[0].x=0.0f; vb[0].y=0.0f; vb[0].z=0.0f;
	vb[1].x=xlen; vb[1].y=0.0f; vb[1].z=0.0f;
	vb[2].x=0.0f; vb[2].y=ylen; vb[2].z=0.0f;
	unsigned int ib1[] = {0,1,2};
	
	Vec2f t0, t1, t2, t3;
	t0.x= 0.01f;                         t0.y=-0.01f+t->m_mipmaps[0].m_height;
	t1.x=-0.01f+t->m_mipmaps[0].m_width; t1.y=-0.01f+t->m_mipmaps[0].m_height;
	t2.x= 0.01f;                         t2.y= 0.01f;
	t3.x=-0.01f+t->m_mipmaps[0].m_width; t3.y= 0.01f;

	Vec2f tb1[3];
	tb1[0]=t0; tb1[1]=t1; tb1[2]=t2;
	AddFan(ib1, tb1, sizeof(ib1)/sizeof(ib1[0]));
}
