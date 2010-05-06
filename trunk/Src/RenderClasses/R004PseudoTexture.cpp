
#include "R004PseudoTexture.h"
#include "MatrixStack.h"


R004PseudoTexture::R004PseudoTexture(Scene *scene, Camera *camera, ViewPort *viewport)
:R000Virtual(scene, camera, viewport)
{
}

void R004PseudoTexture::Render()
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
		//mstackvertices.PushMatrix(mesh->m_worldmatrix);
		//mstacknormals.PushMatrix(mesh->m_worldmatrix);

		// Transforming vertices into screen coords
		Vec3f *vertices = new Vec3f[mesh->m_vertexcount];
		for (unsigned int v=0 ; v<mesh->m_vertexcount ; v++)
		{
			Vec3f vec = mesh->m_vertexbuffer[v];			
			vec = vec * mstackvertices.GetTopMatrix();
			vec.x = vec.x / ( (vec.z>0) ? (vec.z) : (-vec.z) );
			vec.y = vec.y / ( (vec.z>0) ? (vec.z) : (-vec.z) );
			vec = vec * m_viewport->m_viewportmatrix;
			vertices[v] = vec;
		}

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
				Vec3f cullingnormal = ((v1-v0).Cross((v2-v0)));
				if (cullingnormal.z < 0) // TODO dotproduct with camera ?
				{
					// Calculate flat shading
					Vec3f normal = (mesh->m_vertexbuffer[strip->m_indexbuffer[i]]-mesh->m_vertexbuffer[strip->m_indexbuffer[0]])
									.Cross(mesh->m_vertexbuffer[strip->m_indexbuffer[i-1]]-mesh->m_vertexbuffer[strip->m_indexbuffer[0]]);
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
		//mstack.PopMatrix(); // Remove world matrix (the mesh one)
		delete vertices;
	}
}

void R004PseudoTexture::FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade)
{
	Vec3f t0, t1, t2;
	t1.x = 9.99f;
	t2.y = 9.99f;

	if (v0.y <= v1.y && v1.y <= v2.y)
		FillOrderedPoly(v0, v1, v2, t0, t1, t2, shade);
	else if (v0.y <= v2.y && v2.y <= v1.y)
		FillOrderedPoly(v0, v2, v1, t0, t2, t1, shade);
	else if (v1.y <= v0.y && v0.y <= v2.y)
		FillOrderedPoly(v1, v0, v2, t1, t0, t2, shade);
	else if (v1.y <= v2.y && v2.y <= v0.y)
		FillOrderedPoly(v1, v2, v0, t1, t2, t0, shade);
	else if (v2.y <= v0.y && v0.y <= v1.y)
		FillOrderedPoly(v2, v0, v1, t2, t0, t1, shade);
	else if (v2.y <= v1.y && v1.y <= v0.y)
		FillOrderedPoly(v2, v1, v0, t2, t1, t0, shade);
}


void R004PseudoTexture::FillOrderedPoly(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f t0, Vec3f t1, Vec3f t2, unsigned char shade)
{
	if (v0.y == v1.y)
		return;
	if (v2.y == v1.y)
		return;
	if (v0.y == v2.y)
		return;

	int y0 = (int)v0.y;
	int y1 = (int)v1.y;
	int y2 = (int)v2.y;


	float alpha1step = 1.0f / (v1.y - v0.y + 1);
	float alpha2step = 1.0f / (v2.y - v0.y + 1);
	float alpha1 = 0.0f;
	float alpha2 = 0.0f;
	Vec3f u0z0_1 = t0;
	u0z0_1 = u0z0_1/ v0.z;
	Vec3f u0z0_2 = t0;
	u0z0_2 = u0z0_2/ v0.z;
	Vec3f u1z1_1 = t1;
	Vec3f u1z1_2 = t2;
	u1z1_1 = u1z1_1/ v1.z;
	u1z1_2 = u1z1_2/ v2.z;
	float inv_z0_1 = 1/v0.z;
	float inv_z0_2 = 1/v0.z;
	float inv_z1_1 = 1/v1.z;
	float inv_z1_2 = 1/v2.z;


	float ratio1 = (v1.x-v0.x) / (v1.y-v0.y);
	float ratio2 = (v2.x-v0.x) / (v2.y-v0.y);
	int ymax = y1;
	float x1 = v0.x;
	float x2 = v0.x;

	for (int y=y0 ; y<=ymax ; y++)
	{
		float next_x1, next_x2;
		if (y == y0 && y == y1) {
			next_x1 = x1;
			next_x2 = x2;
		} else if (y == y1) {
			// last line of first part
			next_x1 = v1.x;
			next_x2 = x2 + ratio2;	
		} else if (y == y0) {
			// first line
			next_x1 = x1 + ratio1 * (1-(v0.y-y0));
			next_x2 = x2 + ratio2 * (1-(v0.y-y0));
		} else if (y == y2) {
			next_x1 = x1 + ratio1 * (v2.y-y2);
			next_x2 = x2 + ratio2 * (v2.y-y2);
		} else {
			// general case
			next_x1 = x1 + ratio1;
			next_x2 = x2 + ratio2;
		}

		Vec3f ualpha1 = (u0z0_1*(1-alpha1) + u1z1_1*alpha1) / (inv_z0_1*(1-alpha1) + inv_z1_1*alpha1);
		Vec3f ualpha2 = (u0z0_2*(1-alpha2) + u1z1_2*alpha2) / (inv_z0_2*(1-alpha2) + inv_z1_2*alpha2);

		int fillstart = (int) ((x1+next_x1)/2);
		int fillend = (int) ((x2+next_x2)/2);
		if (fillstart > fillend)
		{
			int tmp = fillstart;
			fillstart = fillend;
			fillend = tmp;
			Vec3f ualphatmp = ualpha1;
			ualpha1 = ualpha2;
			ualpha2 = ualphatmp;
		}

		m_viewport->HLine(fillstart, fillend, y, shade);
		//*
		if ( (((((int)ualpha1.y)*10) + ((int)ualpha1.x)) % 2) == 0 ) {
		//if ( (((int)ualpha1.y) % 2) == 0 ) {
			m_viewport->ShowPoint(fillstart, y, (unsigned char)0);
		} else {
			m_viewport->ShowPoint(fillstart, y, (unsigned char)255);
		}
		if ( (((((int)ualpha2.y)*10) + ((int)ualpha2.x)) % 2) == 0 ) {
		//if ( (((int)ualpha2.y) % 2) == 0 ) {
			m_viewport->ShowPoint(fillend, y, (unsigned char)0);
		} else {
			m_viewport->ShowPoint(fillend, y, (unsigned char)255);
		}
		/**/

/*
		float alpha = 0;
		float alphastep = 1.0f / (fillend-fillstart+1);
		Vec3f u0z0 = ualpha1/ualpha1.z;
		Vec3f u1z1 = ualpha2/ualpha2.z;
		float invz0 = 1 / ualpha1.z;
		float invz1 = 1 / ualpha2.z;
		Vec3f ualpha;
		for ( ; fillstart <= fillend ; fillstart++)
		{
			ualpha = (u0z0*(1-alpha) + u1z1*alpha) / (invz0*(1-alpha) + invz1*alpha);
			if ( (((((int)ualpha.y)*10) + ((int)ualpha.x)) % 2) == 0 ) {
				m_viewport->ShowPoint(fillstart, y, 0);
			} else {
				m_viewport->ShowPoint(fillstart, y, 255);
			}
			alpha += alphastep;
		}
*/
		

		x1 = next_x1;
		x2 = next_x2;

		alpha1 += alpha1step;
		alpha2 += alpha2step;

		if (y == y0 && y == y1) {
			ratio1 = (v2.x-v1.x) / (v2.y-v1.y);
			x1 = v1.x + ratio1 * (1-(v1.y-y1));
			x2 = x2 + ratio2 * (1-(v0.y-y0));
			ymax = (int) v2.y;
		} else if (y == y1) {
			ratio1 = (v2.x-v1.x) / (v2.y-v1.y);
			ymax = y2;
			alpha1step = 1.0f / (v2.y - v1.y);
			alpha1 = 0.0f;
			u0z0_1 = t1;
			u0z0_1 = u0z0_1/ v1.z;
			u1z1_1 = t2;
			u1z1_1 = u1z1_1/ v2.z;
		}
	}
}
