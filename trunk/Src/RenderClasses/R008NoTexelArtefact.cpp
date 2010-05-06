
#include "R008NoTexelArtefact.h"
#include "F001CustomNoArtefact.h"
#include "F002Bilinear.h"
#include "F003FakeBump.h"
#include "MatrixStack.h"
#include "Vec2f.h"

typedef struct
{
	bool horizontal;
	bool highest; // for horizontal lines whether it should return the highest or the lowest x
	float error, deltaerr;
	int x, pmax, inc;
	bool firstline;

	float alpha, alphastep;
	Vec3f u0z0, u1z1;
	float invz0, invz1;
	Vec3f ualpha;
	
	Vec3f topalpha, topstep;
	float bottomalpha, bottomstep;

} R008TexCustomLineData;
int R008TexCustomLineGetCurrentXThenStep(R008TexCustomLineData & data);
void R008TexInitCustomLine(R008TexCustomLineData & data, const Vec3f & v0, const Vec3f & v1,
			   const Vec3f & t0, const Vec3f & t1, bool highest);
void R008TexCustomLineJumpSteps(R008TexCustomLineData & data, int steps);


R008NoTexelArtefact::R008NoTexelArtefact(Scene *scene, Camera *camera, ViewPort *viewport)
:R000Virtual(scene, camera, viewport)
{
	m_zbuffer = new float[m_viewport->m_w*m_viewport->m_h];
}

void R008NoTexelArtefact::Render()
{
	m_viewport->Clear();
	MatrixStack mstackvertices;
	MatrixStack mstacknormals;

	// TODO: sort meshes

	mstackvertices.PushMatrix(m_camera->m_projectionmatrix);
	mstackvertices.PushMatrix(m_camera->m_viewmatrix);
	mstacknormals.PushMatrix(m_camera->m_viewmatrix);

	for (int i=m_viewport->m_w*m_viewport->m_h-1 ; i>=0 ; i--)
		m_zbuffer[i] = 99999999999.0f;

	for (int i=0 ; i<m_scene->m_meshCount ; i++)
	{
		Mesh *mesh = m_scene->m_meshes[i];
		mstackvertices.PushMatrix(mesh->m_worldmatrix);
		mstacknormals.PushMatrix(mesh->m_worldmatrix);

		// Transforming vertices into screen coords
		Vec3f *vertices = new Vec3f[mesh->m_vertexcount];
		for (unsigned int v=0 ; v<mesh->m_vertexcount ; v++)
		{
			Vec3f vec = mesh->m_vb[v];			
			vec = mstackvertices.GetTopMatrix() * vec;
			vec.x = vec.x / ( (vec.z>0) ? (vec.z) : (-vec.z) );
			vec.y = vec.y / ( (vec.z>0) ? (vec.z) : (-vec.z) );
			vec = m_viewport->m_viewportmatrix * vec;
			vertices[v] = vec;
		}
		/*
		// STRIPS
		// Reading index buffer and drawing lines
		for (unsigned int s=0 ; s<mesh->m_stripcount ; s++)
		{
			Strip *strip = mesh->m_strips[s];
			Vec3f v0,v1,v2;
			v0 = vertices[strip->m_indexbuffer[0]];
			v1 = vertices[strip->m_indexbuffer[1]];
			unsigned int tex_i=0;
			for (unsigned int i=2 ; i<strip->m_indexcount ; i++, v0=v1, v1=v2, tex_i+=3)
			{
				v2 = vertices[strip->m_indexbuffer[i]];

				// frustum culling
				if (      (v0.x < m_viewport->m_x && v1.x <m_viewport->m_x && v2.x < m_viewport->m_x)
						||(v0.y < m_viewport->m_y && v1.y <m_viewport->m_y && v2.y < m_viewport->m_y)
						||(v0.x >= m_viewport->m_x+m_viewport->m_w  && v1.x >= m_viewport->m_x+m_viewport->m_w  && v2.x >= m_viewport->m_x+m_viewport->m_w)
						||(v0.y >= m_viewport->m_y+m_viewport->m_h && v1.y >= m_viewport->m_y+m_viewport->m_h && v2.y >= m_viewport->m_y+m_viewport->m_h)
				   )
				{
					continue;
				}
	
				// backface culling
				Vec3f cullingnormal;
				if ((i&0x1)==0)
					cullingnormal = ((v1-v0).Cross((v2-v0)));
				else
					cullingnormal = ((v2-v0).Cross((v1-v0)));
				if (cullingnormal.z >= 0) // TODO dotproduct with camera ?
					continue;

				// Z-near culling
				if (v0.z < 0.001 && v1.z < 0.001 && v2.z < 0.001)
					continue;

				// Calculate flat shading
				Vec3f normal;
				if ((i&0x1)==0)
					normal = (mesh->m_vertexbuffer[strip->m_indexbuffer[i]]-mesh->m_vertexbuffer[strip->m_indexbuffer[i-2]])
							.Cross(mesh->m_vertexbuffer[strip->m_indexbuffer[i-1]]-mesh->m_vertexbuffer[strip->m_indexbuffer[i-2]]);
				else
					normal = (mesh->m_vertexbuffer[strip->m_indexbuffer[i-1]]-mesh->m_vertexbuffer[strip->m_indexbuffer[i-2]])
							.Cross(mesh->m_vertexbuffer[strip->m_indexbuffer[i]]-mesh->m_vertexbuffer[strip->m_indexbuffer[i-2]]);
				normal = normal.Mul3x3(mstacknormals.GetTopMatrix());
				normal.Normalize();
				
				//Vec3f frontvec;// = mesh->m_vertexbuffer[strip->m_indexbuffer[i-2]].Mul3x3(mstacknormals.GetTopMatrix());
				//frontvec.z = 1;
				//float normaldotproduct = normal.Dot(frontvec);
				//if (normaldotproduct > 0) normaldotproduct = 0;
				//unsigned char shade = (unsigned char) ((normaldotproduct * (-224.0f)) + 30.0f);

				// Read texture coords
				Vec3f t0 = strip->m_texturebuffer[tex_i];
				t0.z = v0.z;
				Vec3f t1 = strip->m_texturebuffer[tex_i+1];
				t1.z = v1.z;
				Vec3f t2 = strip->m_texturebuffer[tex_i+2];
				t2.z = v2.z;

				// Fill poly
				F003FakeBump f(v0, v1, v2, t0, t1, t2, mesh->m_texture, mesh->m_bumpmap, m_viewport);
				f.FillPoly(normal, m_zbuffer);
			}
		}

		// FANS
		// Reading index buffer and drawing lines
		for (unsigned int s=0 ; s<mesh->m_fancount ; s++)
		{
			Strip *fan = mesh->m_fans[s];
			Vec3f v0,v1,v2;
			v0 = vertices[fan->m_indexbuffer[0]];
			v1 = vertices[fan->m_indexbuffer[1]];
			unsigned int tex_i=0;
			for (unsigned int i=2 ; i<fan->m_indexcount ; i++, v1=v2, tex_i+=3)
			{
				v2 = vertices[fan->m_indexbuffer[i]];

				// frustum culling
				if (      (v0.x < m_viewport->m_x && v1.x <m_viewport->m_x && v2.x < m_viewport->m_x)
						||(v0.y < m_viewport->m_y && v1.y <m_viewport->m_y && v2.y < m_viewport->m_y)
						||(v0.x >= m_viewport->m_x+m_viewport->m_w  && v1.x >= m_viewport->m_x+m_viewport->m_w  && v2.x >= m_viewport->m_x+m_viewport->m_w)
						||(v0.y >= m_viewport->m_y+m_viewport->m_h && v1.y >= m_viewport->m_y+m_viewport->m_h && v2.y >= m_viewport->m_y+m_viewport->m_h)
				   )
				{
					continue;
				}
	
				// backface culling
				Vec3f cullingnormal = ((v1-v0).Cross((v2-v0)));
				if (cullingnormal.z >= 0) // TODO dotproduct with camera ?
					continue;

				// Z-near culling
				if (v0.z < 0.001 && v1.z < 0.001 && v2.z < 0.001)
					continue;

				// Calculate flat shading
				Vec3f normal = (mesh->m_vertexbuffer[fan->m_indexbuffer[i]]-mesh->m_vertexbuffer[fan->m_indexbuffer[0]])
								.Cross(mesh->m_vertexbuffer[fan->m_indexbuffer[i-1]]-mesh->m_vertexbuffer[fan->m_indexbuffer[0]]);
				normal = normal.Mul3x3(mstacknormals.GetTopMatrix());
				normal.Normalize();
				
				//Vec3f frontvec;
				//frontvec.z = 1;
				//float normaldotproduct = normal.Dot(frontvec);
				//if (normaldotproduct > 0) normaldotproduct = 0;
				//unsigned char shade = (unsigned char) ((normaldotproduct * (-224.0f)) + 30.0f);

				// Read texture coords
				Vec3f t0 = fan->m_texturebuffer[tex_i];
				t0.z = v0.z;
				Vec3f t1 = fan->m_texturebuffer[tex_i+1];
				t1.z = v1.z;
				Vec3f t2 = fan->m_texturebuffer[tex_i+2];
				t2.z = v2.z;

				// Fill poly
				F003FakeBump f(v0, v1, v2, t0, t1, t2, mesh->m_texture, mesh->m_bumpmap, m_viewport);
				f.FillPoly(normal, m_zbuffer);
			}
		}
		*/
		mstackvertices.PopMatrix(); // Remove world matrix (the mesh one)
		mstacknormals.PopMatrix();
		delete [] vertices;
	}
}