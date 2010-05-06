
#include "R002WireFrame.h"
#include "MatrixStack.h"


R002WireFrame::R002WireFrame(Scene *scene, Camera *camera, ViewPort *viewport)
:R000Virtual(scene, camera, viewport)
{
}

void R002WireFrame::Render()
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

		// Transforming vertices into screen coords
		Vec3f *vertices = new Vec3f[mesh->m_vertexcount];
		for (unsigned int v=0 ; v<mesh->m_vertexcount ; v++)
		{
			Vec3f vec = mesh->m_vertexbuffer[v];			
			vec = vec * finalmatrix;
			vec = vec / ( (vec.z>0) ? (vec.z) : (-vec.z) );
			vec = vec * m_viewport->m_viewportmatrix;
			vertices[v] = vec;
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
				Vec3f normal = (v1-v0).Cross(v2-v0);
				if (normal.z < 0)
				{
					// draw 3 lines
					Bresenham(v0,v1);
					Bresenham(v1,v2);
					Bresenham(v2,v0);
				}

				v1 = v2;
			}
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
				Vec3f normal;
				if ((i&0x1) == 0)
					normal = (v1-v0).Cross(v2-v0);
				else
					normal = (v2-v0).Cross(v1-v0);
				if (normal.z < 0) {
					// draw 3 lines
					Bresenham(v0,v1);
					Bresenham(v1,v2);
					Bresenham(v2,v0);
				}

				v0 = v1;
				v1 = v2;
			}
		}
	}
}

void R002WireFrame::Bresenham(const Vec3f & va, const Vec3f & vb)
{
	Vec3f v0, v1;

	if (va.x == vb.x)
	{
		// vertical line
		int y, ymax;
		if (va.y < vb.y)
		{
			y = (int) va.y;
			ymax = (int) vb.y;
		}
		else
		{
			y = (int) vb.y;
			ymax = (int) va.y;
		}
		int x = (int) va.x;
		for ( ; y<ymax ; y++)
		{
			m_viewport->ShowPoint(x,y, (unsigned char)200);
		}
		return;
	}

	if ((va.x-vb.x)*(va.x-vb.x) > (va.y-vb.y)*(va.y-vb.y))
	{
		// mostly horizontal line
		if (va.x < vb.x)
		{
			v0 = va;
			v1 = vb;
		}
		else
		{
			v0 = vb;
			v1 = va;
		}
	
		float error = 0;
		float deltaerr = (v1.y-v0.y) / (v1.x-v0.x);
		int y = (int) v0.y;
		int xmax = (int) v1.x;
		for (int x=(int)v0.x ; x<xmax ; x++)
		{
			m_viewport->ShowPoint(x,y, (unsigned char)200);
			error += deltaerr;
			if (error > 0.5f)
			{
				y ++;
				error -= 1.0f;
			}
			else if (error < -0.5f)
			{
				y --;
				error += 1.0f;
			}
		}
	}
	else
	{
		// mostly vertical line
		if (va.y < vb.y)
		{
			v0 = va;
			v1 = vb;
		}
		else
		{
			v0 = vb;
			v1 = va;
		}
	
		float error = 0;
		float deltaerr = (v1.x-v0.x) / (v1.y-v0.y);
		int x = (int) v0.x;
		int ymax = (int) v1.y;
		for (int y=(int)v0.y ; y<ymax ; y++)
		{
			m_viewport->ShowPoint(x,y, (unsigned char)200);
			error += deltaerr;
			if (error > 0.5f)
			{
				x ++;
				error -= 1.0f;
			}
			else if (error < -0.5f)
			{
				x --;
				error += 1.0f;
			}
		}
	}
}