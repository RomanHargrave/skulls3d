
#include <windows.h>
#include <d3d9.h>

#include "main.h"
#include "Cube.h"
#include "Vec2f.h"
#include "Vec3f.h"
#include "Tex.h"

Cube::Cube(float size, Tex *t)
{
	Mesh::Mesh();

	this->m_texture = t;
	this->m_vertexcount = 8;
	//if( FAILED( g_pd3dDevice->CreateVertexBuffer( 8*sizeof(CUSTOMVERTEX),
    //     0 /*Usage*/, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_vb, NULL ) ) )
	//	return E_FAIL;
	//CUSTOMVERTEX *vb = (CUSTOMVERTEX*) m_vb;
	m_vb = new CUSTOMVERTEX[8];
	m_vb[0].x=-size/2.0f; m_vb[0].y=-size/2.0f; m_vb[0].z=-size/2.0f;
	m_vb[1].x= size/2.0f; m_vb[1].y=-size/2.0f; m_vb[1].z=-size/2.0f;
	m_vb[2].x=-size/2.0f; m_vb[2].y= size/2.0f; m_vb[2].z=-size/2.0f;
	m_vb[3].x= size/2.0f; m_vb[3].y= size/2.0f; m_vb[3].z=-size/2.0f;
	m_vb[4].x=-size/2.0f; m_vb[4].y=-size/2.0f; m_vb[4].z= size/2.0f;
	m_vb[5].x= size/2.0f; m_vb[5].y=-size/2.0f; m_vb[5].z= size/2.0f;
	m_vb[6].x=-size/2.0f; m_vb[6].y= size/2.0f; m_vb[6].z= size/2.0f;
	m_vb[7].x= size/2.0f; m_vb[7].y= size/2.0f; m_vb[7].z= size/2.0f;
	
	//g_pd3dDevice->CreateVertexBuffer( 8*sizeof(CUSTOMVERTEX),
    //     0 /*Usage*/, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_vb, NULL );

	Vec2f t0, t1, t2, t3;
	t0.x= 0.01f;                         t0.y=-0.01f+t->m_mipmaps[0].m_height;
	t1.x=-0.01f+t->m_mipmaps[0].m_width; t1.y=-0.01f+t->m_mipmaps[0].m_height;
	t2.x= 0.01f;                         t2.y= 0.01f;
	t3.x=-0.01f+t->m_mipmaps[0].m_width; t3.y= 0.01f;

	Vec2f tb1[18];
	tb1[0]=t0; tb1[1]=t1; tb1[2]=t3;
	tb1[3]=t0; tb1[4]=t3; tb1[5]=t2; 
	tb1[6]=t0; tb1[7]=t1; tb1[8]=t3; 
	tb1[9]=t0; tb1[10]=t3; tb1[11]=t2; 
	tb1[12]=t0; tb1[13]=t1; tb1[14]=t3; 
	tb1[15]=t0; tb1[16]=t3; tb1[17]=t2; 

	unsigned int ib1[] = {0,1,3,2,6,4,5,1/**/};
	AddFan(ib1, tb1, sizeof(ib1)/sizeof(ib1[0]));
//*
	unsigned int ib2[] = {7,6,2,3,1,5,4,6};
	AddFan(ib2, tb1, sizeof(ib2)/sizeof(ib2[0]));
/**/
}
