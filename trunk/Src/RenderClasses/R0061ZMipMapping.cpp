
#include "R0061ZMipMapping.h"
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

} R0061TexCustomLineData;
int R0061TexCustomLineGetCurrentXThenStep(R0061TexCustomLineData & data);
void R0061TexInitCustomLine(R0061TexCustomLineData & data, const Vec3f & v0, const Vec3f & v1,
                           const Vec3f & t0, const Vec3f & t1, bool highest);
void R0061TexCustomLineJumpSteps(R0061TexCustomLineData & data, int steps);


R0061ZMipMapping::R0061ZMipMapping(Scene *scene, Camera *camera, ViewPort *viewport)
:R000Virtual(scene, camera, viewport)
{
}

void R0061ZMipMapping::Render()
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
				if (cullingnormal.z >= 0) // TODO dotproduct with camera ?
					continue;

				// frustum culling
				if (      (v0.x < m_viewport->m_x && v1.x <m_viewport->m_x && v2.x < m_viewport->m_x)
						||(v0.y < m_viewport->m_y && v1.y <m_viewport->m_y && v2.y < m_viewport->m_y)
						||(v0.x >= m_viewport->m_x+m_viewport->m_w  && v1.x >= m_viewport->m_x+m_viewport->m_w  && v2.x >= m_viewport->m_x+m_viewport->m_w)
						||(v0.y >= m_viewport->m_y+m_viewport->m_h && v1.y >= m_viewport->m_y+m_viewport->m_h && v2.y >= m_viewport->m_y+m_viewport->m_h)
				   )
				{
					continue;
				}

				// Z-near culling
				if (v0.z < 0.001 && v1.z < 0.001 && v2.z < 0.001)
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

void R0061ZMipMapping::FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
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


void R0061ZMipMapping::FillOrderedPoly(Vec3f v0, Vec3f v1, Vec3f v2,
									Vec3f t0, Vec3f t1, Vec3f t2, Texture *t)
{	
	// Set of variables used for pixel texture mapping
	float alpha;
	float alphastep;
	Vec3f u0z0;
	Vec3f u1z1;
	float invz0;
	float invz1;
	Vec3f ualpha;
	Vec3f topalpha;
	float bottomalpha;
	Vec3f topstep;
	float bottomstep;
	// End of variables for texture mapping

	// Set of variables used for mipmapping
	float mmzstep;
	float mmz;
	int mmp; // divider, to get texture coordinates right in the filler loop
	unsigned int *tbitmap = t->m_mipmaps[0].m_bitmap; // The selected one, initially set for no-mipmapping
	unsigned int twidth  = t->m_mipmaps[0].m_width;
	unsigned int theight = t->m_mipmaps[0].m_height;
	// End if variables used for mipmapping

	float ratio1 = (v1.x-v0.x)/(v1.y-v0.y);
	float ratio2 = (v2.x-v0.x)/(v2.y-v0.y);
	R0061TexCustomLineData bres1, bres2;
	R0061TexInitCustomLine(bres1, v0, v1, t0, t1, ratio1>ratio2);
	R0061TexInitCustomLine(bres2, v0, v2, t0, t2, ratio2>ratio1);

	int y0 = (int) v0.y;
	int y1 = (int) v1.y;
	int y2 = (int) v2.y;
	int y, ymax;
	int x1, x2;
	bool second_part = false; // flag to know if we are in the second part of the triangle
	
	y = y0;
	ymax = y1;

	if (y < m_viewport->m_y)
	{
		R0061TexCustomLineJumpSteps(bres1, m_viewport->m_y - y);
		R0061TexCustomLineJumpSteps(bres2, m_viewport->m_y - y);
		y = m_viewport->m_y;
	}

	do {
		if (y >= ymax  &&  ymax != y2  &&  second_part == false) {
			R0061TexInitCustomLine(bres1, v1, v2, t1, t2, ratio1>ratio2);
			ymax = y2;
			second_part = true;
		}	
		x1 = R0061TexCustomLineGetCurrentXThenStep(bres1);
		x2 = R0061TexCustomLineGetCurrentXThenStep(bres2);

		alpha = 0.0f;
		if (x1<=x2) {
			u0z0 = bres1.ualpha / ((bres1.ualpha.z>0)?bres1.ualpha.z:-bres1.ualpha.z);
			u1z1 = bres2.ualpha / ((bres2.ualpha.z>0)?bres2.ualpha.z:-bres2.ualpha.z);
			invz0 = 1.0f / ((bres1.ualpha.z>0)?bres1.ualpha.z:-bres1.ualpha.z);
			invz1 = 1.0f / ((bres2.ualpha.z>0)?bres2.ualpha.z:-bres2.ualpha.z);
			ualpha = bres1.ualpha;
			mmz = bres1.ualpha.z;
			if (x2-x1 != 0)
				mmzstep = (bres2.ualpha.z-mmz) / (x2-x1);
			else
				mmzstep = 0;
		} else {
			int x = x1;
			x1 = x2;
			x2 = x;
			u0z0 = bres2.ualpha / ((bres2.ualpha.z>0)?bres2.ualpha.z:-bres2.ualpha.z);
			u1z1 = bres1.ualpha / ((bres1.ualpha.z>0)?bres1.ualpha.z:-bres1.ualpha.z);
			invz0 = 1.0f / ((bres2.ualpha.z>0)?bres2.ualpha.z:-bres2.ualpha.z);
			invz1 = 1.0f / ((bres1.ualpha.z>0)?bres1.ualpha.z:-bres1.ualpha.z);
			ualpha = bres2.ualpha;
			mmz = bres2.ualpha.z;
			mmzstep = (bres1.ualpha.z-mmz) / (x2-x1);
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
			mmz += mmzstep * (-x1);
			x1 = 0;
			// Recalculate starting point
			ualpha = topalpha / bottomalpha;
		}
		if (x2 >= m_viewport->m_w){
			x2 = m_viewport->m_w-1;
		}
		// End of Ugly X clipping

		// Interpolation every k (16) pixels
		Vec2f topstep16 = topstep*16.0f;
		float bottomstep16 = bottomstep*16.0f;
		Vec2f ualphastep;
		if (x2-x1>=16) {
			topalpha += topstep16;
			bottomalpha += bottomstep16;
			ualphastep = ((topalpha/bottomalpha) - ualpha) / 16.0f;
		} else {
			topalpha += topstep*(float)(x2-x1);
			bottomalpha += bottomstep*(float)(x2-x1);
			ualphastep = ((topalpha/bottomalpha) - ualpha) / (float)(x2-x1);
		}

		// Init mipmapping
		if (mmz<0.8) {
			mmp = 0;
		} else if (mmz<1.2) {
			mmp = 1;
		} else if (mmz<1.6) {
			mmp = 2;
		} else if (mmz<2.0) {
			mmp = 3;
		} else{
			mmp = 4;
		}
		tbitmap = t->m_mipmaps[mmp].m_bitmap;
		twidth = t->m_mipmaps[mmp].m_width;
		theight = t->m_mipmaps[mmp].m_height;
		// End of mipmapping 

		unsigned int *video = &((unsigned int*)m_viewport->m_screen->pixels)[(int) ( (m_viewport->m_y+y)*m_viewport->m_screen->pitch/4 + m_viewport->m_x+x1)];
		for (int g=16 ; x1 <= x2 ; x1++, g--)
		{
			if (ualpha.z > 0.001) // Ugly z-near culling
				*video = tbitmap[(((int)ualpha.y)>>mmp)*twidth + (((int)ualpha.x)>>mmp)];

			video++;
			mmz += mmzstep;
			
			// Quake perspective approximation (Not perspective correct)
			ualpha += ualphastep;
			if (g==0)
			{
				if (x2-x1>=16) {
					topalpha += topstep16;
					bottomalpha += bottomstep16;
					ualphastep = ((topalpha/bottomalpha) - ualpha) / 16.0f;
				} else {
					topalpha += topstep*(float)(x2-x1);
					bottomalpha += bottomstep*(float)(x2-x1);
					ualphastep = ((topalpha/bottomalpha) - ualpha) / (float)(x2-x1);
				}
				g=16;
				
				// Mipmapping recalculation (probably not in the quake approximation)
				if (mmz<0.8) {
					mmp = 0;
				} else if (mmz<1.2) {
					mmp = 1;
				} else if (mmz<1.6) {
					mmp = 2;
				} else if (mmz<2.0) {
					mmp = 3;
				} else{
					mmp = 4;
				}
				tbitmap = t->m_mipmaps[mmp].m_bitmap;
				twidth = t->m_mipmaps[mmp].m_width;
				theight = t->m_mipmaps[mmp].m_height;
				// End of mipmapping recalculation (probably not in the quake approximation)
			}
			// End of Quake perspective approximation (Not perspective correct)
		}

		y++;
	}while (y <= ymax  &&  y < m_viewport->m_y+m_viewport->m_h);
}

/**
 * Returns the maximum X for this Y and steps to the next Y
 * whatever orientation (mostly vertical or mostly horizontal)
 */
int R0061TexCustomLineGetCurrentXThenStep(R0061TexCustomLineData & data)
{
	int result = data.x;

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
				} else {
					result = (int)(result+data.x)/2;
				}
				data.x += data.inc;
				data.firstline = false;
				return result;
			}
		}
		if (data.highest) return (int)( (result>data.x)?((int)(result+data.x)/2.0f):data.x );
		else return (int)( (result<data.x)?((int)(result+data.x)/2.0f):data.x );
	}
}

void R0061TexCustomLineJumpSteps(R0061TexCustomLineData & data, int steps)
{
	data.ualpha = data.topalpha / data.bottomalpha;
	data.topalpha += data.topstep * (float)steps;
	data.bottomalpha += data.bottomstep * steps;
	
	if (data.horizontal == false) {
		data.error += steps * data.deltaerr;
		if (data.error > 0.5f) {
			data.x     += (int)(data.error + 0.5);
			data.error -= (int)(data.error + 0.5);
		} else if (data.error < -0.5f) {
			data.x     += (int)(data.error - 0.5);
			data.error -= (int)(data.error - 0.5);
		}
	} else {
		int xsteps = (int) (steps / data.deltaerr);
		data.x += xsteps * data.inc;
		data.error += xsteps * data.deltaerr;
		int ysteps = (int)(data.error + 0.5);
		if (data.error > 0.5f) {
			data.error -= ysteps;
			data.firstline = false;
		}
		for  ( ; ysteps < steps ; ysteps++) {
			data.x+=data.inc;
			data.error += data.deltaerr;
			if (data.error > 0.5f)
				data.error -= 1.0f;
			data.firstline = false;
		}
	}
}

void R0061TexInitCustomLine(R0061TexCustomLineData & data, const Vec3f & v0, const Vec3f & v1,
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
	}
}
