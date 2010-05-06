
#include "R0030FlatVirtual.h"
#include "MatrixStack.h"


R0030FlatVirtual::R0030FlatVirtual(Scene *scene, Camera *camera, ViewPort *viewport)
:R000Virtual(scene, camera, viewport)
{
}

void R0030FlatVirtual::Render()
{
	m_viewport->Clear();
	MatrixStack mstackvertices;
	MatrixStack mstacknormals;

	// TODO: sort meshes

	mstackvertices.PushMatrix(m_camera->m_projectionmatrix);
	mstackvertices.PushMatrix(m_camera->m_viewmatrix);
	mstacknormals.PushMatrix(m_camera->m_viewmatrix);

	for (int i=0 ; i<m_scene->m_meshCount ; i++)
	{
		Mesh *mesh = m_scene->m_meshes[i];
		mstackvertices.PushMatrix(mesh->m_worldmatrix);
		mstacknormals.PushMatrix(mesh->m_worldmatrix);

		// Transforming vertices into screen coords
		Vec3f *vertices = new Vec3f[mesh->m_vertexcount];
		for (unsigned int v=0 ; v<mesh->m_vertexcount ; v++)
		{
			Vec3f vec = mesh->m_vertexbuffer[v];			
			vec = vec * mstackvertices.GetTopMatrix();
			vec = vec / ( (vec.z>0) ? (vec.z) : (-vec.z) );
			vec = vec * m_viewport->m_viewportmatrix;
			vertices[v] = vec;
		}

		// STRIPS
		// Reading index buffer and drawing lines
		for (unsigned int s=0 ; s<mesh->m_stripcount ; s++)
		{
			Strip *strip = mesh->m_strips[s];
			Vec3f v0,v1,v2;
			v0 = vertices[strip->m_indexbuffer[0]];
			v1 = vertices[strip->m_indexbuffer[1]];
			for (unsigned i=2 ; i<strip->m_indexcount ; i++)
			{
				v2 = vertices[strip->m_indexbuffer[i]];
	
				// backface culling
				Vec3f cullingnormal;
				if ( (i&0x1) == 0)
					cullingnormal = ((v1-v0).Cross((v2-v0)));
				else 
					cullingnormal = ((v2-v0).Cross((v1-v0)));
				if (cullingnormal.z < 0) // TODO dotproduct with camera ?
				{
					// Calculate flat shading
					Vec3f normal;
					if ( (i&0x1) == 0)
						normal = (mesh->m_vertexbuffer[strip->m_indexbuffer[i]]-mesh->m_vertexbuffer[strip->m_indexbuffer[i-2]])
						         .Cross(mesh->m_vertexbuffer[strip->m_indexbuffer[i-1]]-mesh->m_vertexbuffer[strip->m_indexbuffer[i-2]]);
					else
						normal = (mesh->m_vertexbuffer[strip->m_indexbuffer[i-1]]-mesh->m_vertexbuffer[strip->m_indexbuffer[i-2]])
						         .Cross(mesh->m_vertexbuffer[strip->m_indexbuffer[i]]-mesh->m_vertexbuffer[strip->m_indexbuffer[i-2]]);
					normal = normal.Mul3x3(mstacknormals.GetTopMatrix());
					normal.Normalize();
					Vec3f frontvec;
					frontvec.z = 1;
					float normaldotproduct = normal.Dot(frontvec);
					if (normaldotproduct > 0) normaldotproduct = 0;
					unsigned char shade = (unsigned char) ((normaldotproduct * (-200.0f)) + 55.0f);

					// Fill poly
					FillPoly(v0, v1, v2, shade);
				}

				v0 = v1;
				v1 = v2;
			}
		}

		// FANS
		// Reading index buffer and drawing lines
		for (unsigned int f=0 ; f<mesh->m_fancount ; f++)
		{
			Strip *fan = mesh->m_fans[f];
			Vec3f v0,v1,v2;
			v0 = vertices[fan->m_indexbuffer[0]];
			v1 = vertices[fan->m_indexbuffer[1]];
			for (unsigned i=2 ; i<fan->m_indexcount ; i++)
			{
				v2 = vertices[fan->m_indexbuffer[i]];
	
				// backface culling
				Vec3f cullingnormal = ((v1-v0).Cross((v2-v0)));
				if (cullingnormal.z < 0) // TODO dotproduct with camera ?
				{
					// Calculate flat shading
					Vec3f normal = (mesh->m_vertexbuffer[fan->m_indexbuffer[i]]-mesh->m_vertexbuffer[fan->m_indexbuffer[0]])
									.Cross(mesh->m_vertexbuffer[fan->m_indexbuffer[i-1]]-mesh->m_vertexbuffer[fan->m_indexbuffer[0]]);
					normal = normal.Mul3x3(mstacknormals.GetTopMatrix());
					normal.Normalize();
					Vec3f frontvec;
					frontvec.z = 1;
					float normaldotproduct = normal.Dot(frontvec);
					if (normaldotproduct > 0) normaldotproduct = 0;
					unsigned char shade = (unsigned char) ((normaldotproduct * (-200.0f)) + 55.0f);

					// Fill poly
					FillPoly(v0, v1, v2, shade);
				}

				v1 = v2;
			}
		}
		mstackvertices.PopMatrix(); // Remove world matrix (the mesh one)
		mstacknormals.PopMatrix();
		delete vertices;
	}
}

void R0030FlatVirtual::FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade)
{
	if (v0.y <= v1.y && v1.y <= v2.y)
		FillOrderedPoly(v0, v1, v2, shade);
	else if (v0.y <= v2.y && v2.y <= v1.y)
		FillOrderedPoly(v0, v2, v1, shade);
	else if (v1.y <= v0.y && v0.y <= v2.y)
		FillOrderedPoly(v1, v0, v2, shade);
	else if (v1.y <= v2.y && v2.y <= v0.y)
		FillOrderedPoly(v1, v2, v0, shade);
	else if (v2.y <= v0.y && v0.y <= v1.y)
		FillOrderedPoly(v2, v0, v1, shade);
	else if (v2.y <= v1.y && v1.y <= v0.y)
		FillOrderedPoly(v2, v1, v0, shade);
}
