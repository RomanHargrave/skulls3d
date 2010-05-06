
#include "R005Texture.h"
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

} TexBhData;
int TexBhGetCurrentXThenStep(TexBhData & data);
void TexInitBh(TexBhData & data, const Vec3f & v0, const Vec3f & v1,
			   const Vec3f & t0, const Vec3f & t1, bool highest);


R005Texture::R005Texture(Scene *scene, Camera *camera, ViewPort *viewport)
:R000Virtual(scene, camera, viewport)
{
}

void R005Texture::Render()
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
	
				// backface culling
				Vec3f cullingnormal = ((v1-v0).Cross((v2-v0)));
				if (cullingnormal.z > 0) // TODO dotproduct with camera ?
					continue;

				// frustum culling
				if (  (v0.x < 0 && v1.x <0 && v2.x < 0)
						||(v0.y < 0 && v1.y <0 && v2.y < 0)
						||(v0.x >= m_viewport->m_w  && v1.x >= m_viewport->m_w  && v2.x >= m_viewport->m_w)
						||(v0.y >= m_viewport->m_h && v1.y >= m_viewport->m_h && v2.y >= m_viewport->m_h)
						)
					continue;

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

				// Read texture coords
				Vec3f t0 = fan->m_texturebuffer[tex_i];
				t0.z = v0.z;
				Vec3f t1 = fan->m_texturebuffer[tex_i+1];
				t1.z = v1.z;
				Vec3f t2 = fan->m_texturebuffer[tex_i+2];
				t2.z = v2.z;

				// Fill poly
				FillPoly(v0, v1, v2, t0, t1, t2, mesh->m_texture);
			}
		}
		//mstack.PopMatrix(); // Remove world matrix (the mesh one)
		delete vertices;
	}
}

void R005Texture::FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
							 Vec3f t0, Vec3f t1, Vec3f t2, Texture *t)
{
	if (v0.y <= v1.y && v1.y <= v2.y)
		FillOrderedPoly(v0, v1, v2, t0, t1, t2, t);
	else if (v0.y <= v2.y && v2.y <= v1.y)
		FillOrderedPoly(v0, v2, v1, t0, t2, t1, t);
	else if (v1.y <= v0.y && v0.y <= v2.y)
		FillOrderedPoly(v1, v0, v2, t1, t0, t2, t);
	else if (v1.y <= v2.y && v2.y <= v0.y)
		FillOrderedPoly(v1, v2, v0, t1, t2, t0, t);
	else if (v2.y <= v0.y && v0.y <= v1.y)
		FillOrderedPoly(v2, v0, v1, t2, t0, t1, t);
	else if (v2.y <= v1.y && v1.y <= v0.y)
		FillOrderedPoly(v2, v1, v0, t2, t1, t0, t);
}


void R005Texture::FillOrderedPoly(Vec3f v0, Vec3f v1, Vec3f v2,
									Vec3f t0, Vec3f t1, Vec3f t2, Texture *t)
{	
	// Set of variables used for pixel texture mapping
	float alpha;
	float alphastep;
	Vec2f u0z0;
	Vec2f u1z1;
	float invz0;
	float invz1;
	Vec2f ualpha;
	Vec2f topalpha;
	float bottomalpha;
	Vec2f topstep;
	float bottomstep;
	// End of variables for texture mapping

	float ratio1 = (v1.x-v0.x)/(v1.y-v0.y);
	float ratio2 = (v2.x-v0.x)/(v2.y-v0.y);
	TexBhData bres1, bres2;
	TexInitBh(bres1, v0, v1, t0, t1, ratio1>ratio2);
	TexInitBh(bres2, v0, v2, t0, t2, ratio2>ratio1);

	int y0 = (int) v0.y;
	int y1 = (int) v1.y;
	int y2 = (int) v2.y;
	int y, ymax;
	int x1, x2;
	bool second_part = false; // flag to know if we are in the second part of the triangle
	
	y = y0;
	ymax = y1;

	// Ugly Y clipping
	// TODO : recalculate triangles instead
	if (ymax >= m_viewport->m_h)
		ymax = m_viewport->m_h - 1;
	// End of Ugly Y clipping

	do {
		if (y >= ymax  &&  ymax != y2  &&  second_part == false) {
			TexInitBh(bres1, v1, v2, t1, t2, ratio1>ratio2);
			ymax = y2;
			second_part = true;
			// Ugly Y clipping
			// TODO : recalculate triangles instead
			if (ymax >= m_viewport->m_h)
				ymax = m_viewport->m_h - 1;
			// End of Ugly Y clipping
		}	
		x1 = TexBhGetCurrentXThenStep(bres1);
		x2 = TexBhGetCurrentXThenStep(bres2);

		if (y >= 0)
		{
			alpha = 0.0f;
			if (x1<=x2) {
				u0z0 = bres1.ualpha / ((bres1.ualpha.z>0)?bres1.ualpha.z:-bres1.ualpha.z);
				u1z1 = bres2.ualpha / ((bres2.ualpha.z>0)?bres2.ualpha.z:-bres2.ualpha.z);
				invz0 = 1.0f / ((bres1.ualpha.z>0)?bres1.ualpha.z:-bres1.ualpha.z);
				invz1 = 1.0f / ((bres2.ualpha.z>0)?bres2.ualpha.z:-bres2.ualpha.z);
				ualpha = bres1.ualpha;
			} else {
				int x = x1;
				x1 = x2;
				x2 = x;
				u0z0 = bres2.ualpha / ((bres2.ualpha.z>0)?bres2.ualpha.z:-bres2.ualpha.z);
				u1z1 = bres1.ualpha / ((bres1.ualpha.z>0)?bres1.ualpha.z:-bres1.ualpha.z);
				invz0 = 1.0f / ((bres2.ualpha.z>0)?bres2.ualpha.z:-bres2.ualpha.z);
				invz1 = 1.0f / ((bres1.ualpha.z>0)?bres1.ualpha.z:-bres1.ualpha.z);
				ualpha = bres2.ualpha;
			}
			alphastep = 1.0f / (x2-x1);

			topalpha = u0z0;
			bottomalpha = invz0;
			topstep = (u1z1-u0z0)*alphastep;
			bottomstep = alphastep*(invz1-invz0);

			// Ugly X clipping
			// TODO: recalculate triangle instead
			if (x1<0){
				topalpha += topstep*((float)-x1);
				bottomalpha += bottomstep*(-x1);
				x1 = 0;
				// Recalculate starting point
				ualpha = topalpha / bottomalpha;
			}
			if (x2 >= m_viewport->m_w){
				x2 = m_viewport->m_w-1;
			}
			// End of Ugly X clipping

			//*
			Vec2f topstep64 = topstep*16.0f;
			float bottomstep64 = bottomstep*16.0f;
			Vec2f ualphastep;
			if (x2-x1>=16) {
				topalpha += topstep64;
				bottomalpha += bottomstep64;
				ualphastep = ((topalpha/bottomalpha) - ualpha) / 16.0f;
			} else {
				topalpha += topstep*(float)(x2-x1);
				bottomalpha += bottomstep*(float)(x2-x1);
				ualphastep = ((topalpha/bottomalpha) - ualpha) / (float)(x2-x1);
			}
			/**/
			unsigned int *video = &((unsigned int*)m_viewport->m_screen->pixels)[(int) ( y*m_viewport->m_screen->pitch/4 + x1)];
			for (int g=16 ; x1 <= x2 ; x1++, g--)
			{
				//ualpha = topalpha / bottomalpha;
				//topalpha += topstep;
				//bottomalpha += bottomstep;

				*video = t->m_mipmaps[0].m_bitmap[((int)ualpha.y)*t->m_mipmaps[0].m_width + (int)ualpha.x];
				video++;
				
				//*
				if (g==0)
				{
					if (x2-x1>=16) {
						topalpha += topstep64;
						bottomalpha += bottomstep64;
						ualphastep = ((topalpha/bottomalpha) - ualpha) / 16.0f;
					} else {
						topalpha += topstep*(float)(x2-x1);
						bottomalpha += bottomstep*(float)(x2-x1);
						ualphastep = ((topalpha/bottomalpha) - ualpha) / (float)(x2-x1);
					}
					g=16;
				}
				else
				{
					ualpha += ualphastep;
				}
				/**/
			}
		}

		y++;
	}while (y <= ymax);
}

/**
 * Returns the maximum X for this Y and steps to the next Y
 * whatever orientation (mostly vertical or mostly horizontal)
 */
int TexBhGetCurrentXThenStep(TexBhData & data)
{
	int result = data.x;
/*	
	data.ualpha = (data.u0z0*(1.0f-data.alpha) + data.u1z1*data.alpha)
	            / (data.invz0*(1.0f-data.alpha) + data.invz1*data.alpha);
	data.alpha += data.alphastep;
*/
	data.ualpha = data.topalpha / data.bottomalpha;
	data.topalpha += data.topstep;
	data.bottomalpha += data.bottomstep;

	if (data.horizontal == false) {
		data.error += data.deltaerr;
		if (data.error > 0.5f) {
			data.x ++;
			data.error -= 1.0f;
		} else if (data.error < -0.5f) {
			data.x --;
			data.error += 1.0f;
		}
		return result;
	} else {
		for ( ; data.x != data.pmax ; data.x+=data.inc) {
			data.error += data.deltaerr;
			if (data.error > 0.5f) {
				data.error -= 1.0f;
				if (data.firstline) {
					//result = data.x;
				} else {
					result = (int)(result+data.x)/2;
				}
				//if (data.highest) result = (result>data.x)?result:data.x;
				//else result = (result<data.x)?result:data.x;
				data.x += data.inc;
				data.firstline = false;
				return result;
			}
		}
		if (data.highest) return (int)( (result>data.x)?((int)(result+data.x)/2.0f):data.x );
		else return (int)( (result<data.x)?((int)(result+data.x)/2.0f):data.x );
	}
}

void TexInitBh(TexBhData & data, const Vec3f & v0, const Vec3f & v1,
			   const Vec3f & t0, const Vec3f & t1, bool highest)
{
	data.error = 0.0f;
	data.x = (int) v0.x;
	data.highest = highest;
	data.firstline = true;

	data.alpha = 0.0f;
	data.alphastep = 1.0f / (((int)v1.y) - ((int)v0.y));
	data.u0z0 = t0 / ((v0.z>0)?v0.z:-v0.z);
	data.u1z1 = t1 / ((v1.z>0)?v1.z:-v1.z);
	data.invz0 = 1.0f / ((v0.z>0)?v0.z:-v0.z);
	data.invz1 = 1.0f / ((v1.z>0)?v1.z:-v1.z);

	data.topalpha = data.u0z0;
	data.bottomalpha = data.invz0;
	data.topstep = (data.u1z1-data.u0z0)*data.alphastep;
	data.bottomstep = data.alphastep*(data.invz1-data.invz0);

	if ((v0.x-v1.x)*(v0.x-v1.x) > (v0.y-v1.y)*(v0.y-v1.y)) {
		data.horizontal = true;
		data.pmax = (int) v1.x;
		if (v0.x < v1.x) {
			data.deltaerr = ((int)v1.y-(int)v0.y) * 1.0f / ((int)v1.x-(int)v0.x);
			data.inc = 1;
		} else {
			data.deltaerr = ((int)v1.y-(int)v0.y) * 1.0f / ((int)v0.x-(int)v1.x);
			data.inc = -1;
		}
	} else {
		data.horizontal = false;
		data.deltaerr = ((int)v1.x-(int)v0.x) * 1.0f / ((int)v1.y-(int)v0.y);
		data.pmax = (int) v1.y;
		data.inc = 1;
	}
}