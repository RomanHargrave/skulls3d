
#include "R007Bilinear.h"
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

} R007TexCustomLineData;
int R007TexCustomLineGetCurrentXThenStep(R007TexCustomLineData & data);
void R007TexInitCustomLine(R007TexCustomLineData & data, const Vec3f & v0, const Vec3f & v1,
			   const Vec3f & t0, const Vec3f & t1, bool highest);
void R007TexCustomLineJumpSteps(R007TexCustomLineData & data, int steps);


R007Bilinear::R007Bilinear(Scene *scene, Camera *camera, ViewPort *viewport)
:R000Virtual(scene, camera, viewport)
{
}

void R007Bilinear::Render()
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
			Vec3f vec = mesh->m_vb[v];			
			vec = vec * mstackvertices.GetTopMatrix();
			vec.x = vec.x / ( (vec.z>0) ? (vec.z) : (-vec.z) );
			vec.y = vec.y / ( (vec.z>0) ? (vec.z) : (-vec.z) );
			vec = vec * m_viewport->m_viewportmatrix;
			vertices[v] = vec;
		}
		/*
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
				Vec3f normal = (mesh->m_vb[fan->m_indexbuffer[i]]-vb0)
								.Cross(mesh->m_vb[fan->m_indexbuffer[i-1]]-mesh->m_vb[fan->m_indexbuffer[0]]);
				normal = normal.Mul3x3(mstacknormals.GetTopMatrix());
				normal.Normalize();
				Vec3f frontvec;
				frontvec.z = 1;
				float normaldotproduct = normal.Dot(frontvec);
				if (normaldotproduct > 0) normaldotproduct = 0;
				unsigned char shade = (unsigned char) ((normaldotproduct * (-224.0f)) + 30.0f);

				// Read texture coords
				Vec3f t0 = fan->m_texturebuffer[tex_i];
				t0.z = v0.z;
				Vec3f t1 = fan->m_texturebuffer[tex_i+1];
				t1.z = v1.z;
				Vec3f t2 = fan->m_texturebuffer[tex_i+2];
				t2.z = v2.z;

				// Fill poly
				FillPoly(v0, v1, v2, t0, t1, t2, mesh->m_texture, shade);
			}
		}
		*/
		//mstack.PopMatrix(); // Remove world matrix (the mesh one)
		delete vertices;
	}
}

void R007Bilinear::FillPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
							 Vec3f t0, Vec3f t1, Vec3f t2, Tex *t, unsigned char shade)
{
	//ASSERT(shade >= 0.0f);
	//ASSERT(shade < 256.0f);
	if (v0.y <= v1.y && v1.y <= v2.y)
		FillOrderedPoly(v0, v1, v2, t0, t1, t2, t, shade);
	else if (v0.y <= v2.y && v2.y <= v1.y)
		FillOrderedPoly(v0, v2, v1, t0, t2, t1, t, shade);
	else if (v1.y <= v0.y && v0.y <= v2.y)
		FillOrderedPoly(v1, v0, v2, t1, t0, t2, t, shade);
	else if (v1.y <= v2.y && v2.y <= v0.y)
		FillOrderedPoly(v1, v2, v0, t1, t2, t0, t, shade);
	else if (v2.y <= v0.y && v0.y <= v1.y)
		FillOrderedPoly(v2, v0, v1, t2, t0, t1, t, shade);
	else if (v2.y <= v1.y && v1.y <= v0.y)
		FillOrderedPoly(v2, v1, v0, t2, t1, t0, t, shade);
}


void R007Bilinear::FillOrderedPoly(Vec3f v0, Vec3f v1, Vec3f v2,
									Vec3f t0, Vec3f t1, Vec3f t2, Tex *t, unsigned char shade)
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
	int ualphastep_x;
	int mmp; // divider, to get texture coordinates right in the filler loop
	unsigned int *tbitmap = t->m_mipmaps[0].m_bitmap; // The selected one, initially set for no-mipmapping
	unsigned int twidth  = t->m_mipmaps[0].m_width;
	unsigned int theight = t->m_mipmaps[0].m_height;
	// End if variables used for mipmapping

	// Set of variables used for bilinear filtering
	int r,g,b;
	float /*prevx, prevy, */actualx, actualy, nextx, nexty;
	int u, v;
	// End of variables used for bilinear filtering

	float ratio1 = (v1.x-v0.x)/(v1.y-v0.y);
	float ratio2 = (v2.x-v0.x)/(v2.y-v0.y);
	R007TexCustomLineData bres1, bres2;
	R007TexInitCustomLine(bres1, v0, v1, t0, t1, ratio1>ratio2);
	R007TexInitCustomLine(bres2, v0, v2, t0, t2, ratio2>ratio1);

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
		int diff;
		if (y1 >= m_viewport->m_y)
			diff = m_viewport->m_y - y;
		else
			diff = y1 - y0;
		R007TexCustomLineJumpSteps(bres1, diff);
		R007TexCustomLineJumpSteps(bres2, diff);
		y += diff;
	}

	do {
		if (y >= ymax  &&  ymax != y2  &&  second_part == false) {
			R007TexInitCustomLine(bres1, v1, v2, t1, t2, ratio1>ratio2);
			if (y < m_viewport->m_y)
			{
				R007TexCustomLineJumpSteps(bres1, m_viewport->m_y - y);
				R007TexCustomLineJumpSteps(bres2, m_viewport->m_y - y);
				y = m_viewport->m_y;
			}
			ymax = y2;
			second_part = true;
		}	
		x1 = R007TexCustomLineGetCurrentXThenStep(bres1);
		x2 = R007TexCustomLineGetCurrentXThenStep(bres2);

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
		if (x1<m_viewport->m_x){
			topalpha += topstep*((float)m_viewport->m_x-x1);
			bottomalpha += bottomstep*(m_viewport->m_x-x1);
			x1 = m_viewport->m_x;
			// Recalculate starting point
			ualpha = topalpha / bottomalpha;
		}
		if (x2 >= m_viewport->m_x + m_viewport->m_w){
			x2 = m_viewport->m_x + m_viewport->m_w-1;
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
		ualphastep_x = (int)ualphastep.x;
		mmp = 0;
		while (ualphastep_x > 1 && mmp+1 < MIPMAPS_COUNT) {
			mmp++;
			ualphastep_x >>= 1;
		}
		tbitmap = t->m_mipmaps[mmp].m_bitmap;
		twidth = t->m_mipmaps[mmp].m_width;
		theight = t->m_mipmaps[mmp].m_height;
		// End of mipmapping init

		unsigned int *video = &((unsigned int*)m_viewport->m_screen->pixels)[(int) ( y*m_viewport->m_screen->pitch/4 + x1)];
		for (int quake=16 ; x1 <= x2 ; x1++, quake--)
		{
			if (ualpha.z > 0.001) // Ugly z-near culling
			{
				//* // MUF SOLUTION
				actualx = ualpha.x/(1<<mmp);
				actualy = ualpha.y/(1<<mmp);
				
				u = ((int)actualx);
				v = ((int)actualy);

				actualx = u+1 - actualx;
				actualy = v+1 - actualy;
				nextx = 1 - actualx;
				nexty = 1 - actualy;

				if (u+1>=(int)twidth) u=(int)twidth-2;
				if (v+1>=(int)theight) v=(int)theight-2;

				v *= twidth;

				r = (int)( (tbitmap[v        + u  ]>>16) * actualx*actualy
				   +(tbitmap[v+twidth + u  ]>>16) * actualx*nexty
				   +(tbitmap[v        + u+1]>>16) * nextx*actualy
				   +(tbitmap[v+twidth + u+1]>>16) * nextx*nexty );
				g = (int)( ((tbitmap[v        + u  ]>>8)&0xFF) * actualx*actualy
				   +((tbitmap[v+twidth + u  ]>>8)&0xFF) * actualx*nexty
				   +((tbitmap[v        + u+1]>>8)&0xFF) * nextx*actualy
				   +((tbitmap[v+twidth + u+1]>>8)&0xFF) * nextx*nexty );
				b = (int)( (tbitmap[v        + u  ]&0xFF) * actualx*actualy
				   +(tbitmap[v+twidth + u  ]&0xFF) * actualx*nexty
				   +(tbitmap[v        + u+1]&0xFF) * nextx*actualy
				   +(tbitmap[v+twidth + u+1]&0xFF) * nextx*nexty );

				// Camera shading
				r = ((r*shade) << 8) & 0xFF0000;
				g = (g*shade) & 0xFF00;
				b = ((b*shade) >> 8) & 0xFF;
				// End of camera shading
				*video = r|g|b;
				/* END OF MUF SOLUTION */

				/* // GIBBS SOLUTION 
				// reusing variables
				actualx = ualpha.x/(1<<mmp);
				actualy = ualpha.y/(1<<mmp);

				ASSERT(actualx >= 0);
				ASSERT(actualx < twidth);
				ASSERT(actualy >= 0);
				ASSERT(actualy < theight);

				u = ((int)actualx);
				v = ((int)actualy);
				actualx = actualx - u;
				actualy = actualy - v;
				v *= twidth;
				// end of variable reuse
				prevx   =  0.5f - actualx; if (prevx<0) prevx=0;
				prevy   =  0.5f - actualy; if (prevy<0) prevy=0;
				nextx   = -0.5f + actualx; if (nextx<0) nextx=0;
				nexty   = -0.5f + actualy; if (nexty<0) nexty=0;
				
				actualx = 1 - prevx - nextx;
				actualy = 1 - prevy - nexty;
				if (v == 0  &&  prevy > 0) v+=(int)twidth;
				else if (v >= (int) ((theight-1)*twidth)  &&  nexty > 0) v-=(int)twidth;
				if (u == 0  &&  prevx > 0) u++;
				else if (u >= (int) twidth-1  &&  nextx > 0) u--;
				ASSERT(v > 0 || prevy == 0);
				ASSERT(v < (int) ((theight-1)*twidth) || nexty == 0);
				ASSERT(u > 0 || prevx == 0);
				ASSERT(u < (int) (twidth-1) || nextx == 0);

				//*
				float actualx_actualy = actualx*actualy;
				int tvu = tbitmap[v + u];
				r = (int)( (tvu >> 16) * actualx_actualy );
				g = (int)( ((tvu >> 8)&0xFF) * actualx_actualy );
				b = (int)( (tvu & 0xFF) * actualx_actualy );
				if (prevx > 0)
				{
					float prevx_actualy = prevx*actualy;
					int tv_u = tbitmap[v + u-1];
					r += (int)( (tv_u    >> 16) * prevx_actualy );
					g += (int)( ((tv_u >> 8)   &0xFF) * prevx_actualy );
					b += (int)( (tv_u    & 0xFF) * prevx_actualy );
					if (prevy > 0)
					{
						float actualx_prevy = actualx*prevy;
						float prevx_prevy = prevx*prevy;
						int t_vu = tbitmap[v-twidth + u];
						int t_v_u = tbitmap[v-twidth + u-1];
						r += (int)( (t_vu    >> 16) * actualx_prevy
							 +(t_v_u   >> 16) * prevx_prevy
							);
						g += (int)(((t_vu >> 8)   &0xFF) * actualx_prevy
							 +((t_v_u >> 8)  &0xFF) * prevx_prevy
							);
						b += (int)((t_vu    & 0xFF) * actualx_prevy
							 +(t_v_u   & 0xFF) * prevx_prevy
							);
					}
					else
					{
						float actualx_nexty = actualx*nexty;
						float prevx_nexty = prevx*nexty;
						int t__vu = tbitmap[v+twidth + u];
						int t__v_u = tbitmap[v+twidth + u-1];
						r += (int)( (t__vu   >> 16) * actualx_nexty
							 +(t__v_u  >> 16) * prevx_nexty
							);
						g += (int)( ((t__vu >> 8)  &0xFF) * actualx_nexty
							 +((t__v_u >> 8) &0xFF) * prevx_nexty
							);
						b += (int)( (t__vu   & 0xFF) * actualx_nexty
							 +(t__v_u  & 0xFF) * prevx_nexty
							);
					}
				}
				else
				{
					float nextx_actualy = nextx*actualy;
					int tv__u = tbitmap[v + u+1];
					r += (int)( (tv__u   >> 16) * nextx_actualy );
					g += (int)( ((tv__u >> 8)  &0xFF) * nextx_actualy );
					b += (int)( (tv__u   & 0xFF) * nextx_actualy );
					if (prevy > 0)
					{
						float actualx_prevy = actualx*prevy;
						float nextx_prevy = nextx*prevy;
						int t_vu = tbitmap[v-twidth + u];
						int t_v__u = tbitmap[v-twidth + u+1];
						r += (int)( (t_vu    >> 16) * actualx_prevy
							 +(t_v__u  >> 16) * nextx_prevy
							);
						g += (int)( ((t_vu >> 8)   &0xFF) * actualx_prevy
							 +((t_v__u >> 8) &0xFF) * nextx_prevy
							);
						b += (int)( (t_vu    & 0xFF) * actualx_prevy
							 +(t_v__u  & 0xFF) * nextx_prevy
							);
					}
					else
					{
						float actualx_nexty = actualx*nexty;
						float nextx_nexty = nextx*nexty;
						int t__vu = tbitmap[v+twidth + u];
						int t__v__u = tbitmap[v+twidth + u+1];
						r += (int)( (t__vu   >> 16) * actualx_nexty
							 +(t__v__u >> 16) * nextx_nexty
							);
						g += (int)( ((t__vu >> 8)  &0xFF) * actualx_nexty
							 +((t__v__u >> 8)&0xFF) * nextx_nexty
							);
						b += (int)( (t__vu   & 0xFF) * actualx_nexty
							 +(t__v__u & 0xFF) * nextx_nexty
							);
					}
				}
				// Camera shading
				r = ((r*shade) << 8) & 0xFF0000;
				g = (g*shade) & 0xFF00;
				b = ((b*shade) >> 8) & 0xFF;
				// End of camera shading
				*video = r|g|b;
				/* END OF GIBBS SOLUTION */

				/*
				int actualx_actualy = (int) (actualx*actualy*256);
				int prevx_actualy = (int) (prevx*actualy*256);
				int nextx_actualy = (int) (nextx*actualy*256);
				int actualx_nexty = (int) (actualx*nexty*256);
				int actualx_prevy = (int) (actualx*prevy*256);
				int nextx_nexty = (int) (nextx*nexty*256);
				int prevx_prevy = (int) (prevx*prevy*256);
				int prevx_nexty = (int) (prevx*nexty*256);
				int nextx_prevy = (int) (nextx*prevy*256);
				int tvu = tbitmap[v + u];
				int tv_u = tbitmap[v + u-1];
				int tv__u = tbitmap[v + u+1];
				int t__vu = tbitmap[v+twidth + u];
				int t_vu = tbitmap[v-twidth + u];
				int t__v__u = tbitmap[v+twidth + u+1];
				int t_v_u = tbitmap[v-twidth + u-1];
				int t__v_u = tbitmap[v+twidth + u-1];
				int t_v__u = tbitmap[v-twidth + u+1];
				r = (int)( (tvu >> 16) * actualx_actualy
				     +(tv_u    >> 16) * prevx_actualy
				     +(tv__u   >> 16) * nextx_actualy
				     +(t__vu   >> 16) * actualx_nexty
				     +(t_vu    >> 16) * actualx_prevy
				     +(t__v__u >> 16) * nextx_nexty
				     +(t_v_u   >> 16) * prevx_prevy
				     +(t__v_u  >> 16) * prevx_nexty
				     +(t_v__u  >> 16) * nextx_prevy
					);
				g = (int)( ((tvu >> 8)&0xFF) * actualx_actualy
				     +((tv_u >> 8)   &0xFF) * prevx_actualy
				     +((tv__u >> 8)  &0xFF) * nextx_actualy
				     +((t__vu >> 8)  &0xFF) * actualx_nexty
				     +((t_vu >> 8)   &0xFF) * actualx_prevy
				     +((t__v__u >> 8)&0xFF) * nextx_nexty
				     +((t_v_u >> 8)  &0xFF) * prevx_prevy
				     +((t__v_u >> 8) &0xFF) * prevx_nexty
				     +((t_v__u >> 8) &0xFF) * nextx_prevy
					);
				b = (int)( (tvu & 0xFF) * actualx_actualy
				     +(tv_u    & 0xFF) * prevx_actualy
				     +(tv__u   & 0xFF) * nextx_actualy
				     +(t__vu   & 0xFF) * actualx_nexty
				     +(t_vu    & 0xFF) * actualx_prevy
				     +(t__v__u & 0xFF) * nextx_nexty
				     +(t_v_u   & 0xFF) * prevx_prevy
				     +(t__v_u  & 0xFF) * prevx_nexty
				     +(t_v__u  & 0xFF) * nextx_prevy
					);
				// Camera shading
				r = (int) (r*shade);
				g = (int) (g*shade);
				b = (int) (b*shade);
				// End of camera shading
				*video = ((r&0xFF00)<<8)|(g&0xFF00)|((b&0xFF00)>>8);
				/**/
			}

			video++;

			// Quake perspective approximation (Not perspective correct)
			ualpha += ualphastep;
			if (quake==0)
			{
				quake=16;
				ualpha = topalpha / bottomalpha;
				if (x2-x1>=16) {
					topalpha += topstep16;
					bottomalpha += bottomstep16;
					ualphastep = ((topalpha/bottomalpha) - ualpha) / 16.0f;
				} else {
					topalpha += topstep*(float)(x2-x1);
					bottomalpha += bottomstep*(float)(x2-x1);
					ualphastep = ((topalpha/bottomalpha) - ualpha) / (float)(x2-x1);
				}
				
				// Mipmapping recalculation (probably not in the quake approximation)
				ualphastep_x = (int)ualphastep.x;
				mmp = 0;
				while (ualphastep_x > 1 && mmp+1 < MIPMAPS_COUNT) {
					mmp++;
					ualphastep_x >>= 1;
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
int R007TexCustomLineGetCurrentXThenStep(R007TexCustomLineData & data)
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
				if (!data.firstline)
					result = (int)(result+data.x)/2;
				data.x += data.inc;
				data.firstline = false;
				return result;
			}
		}
		if (data.highest) return (int)( (result>data.x)?((int)(result+data.x)/2.0f):data.x );
		else return (int)( (result<data.x)?((int)(result+data.x)/2.0f):data.x );
	}
}

void R007TexCustomLineJumpSteps(R007TexCustomLineData & data, int steps)
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

void R007TexInitCustomLine(R007TexCustomLineData & data, const Vec3f & v0, const Vec3f & v1,
			   const Vec3f & t0, const Vec3f & t1, bool highest)
{
	data.error = 0.0f;
	data.x = (int) v0.x;
	data.highest = highest;
	data.firstline = true;

	data.alpha = 0.0f;
	data.alphastep = 1.0f / (((int)v1.y) - ((int)v0.y));
	data.u0z0 = t0 / ((v0.z>0)?v0.z:-v0.z); // TODO: Find a solution that works for case when z<0
	data.u1z1 = t1 / ((v1.z>0)?v1.z:-v1.z); // TODO: Better frustum culling will do (cut faces down to frustum)
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