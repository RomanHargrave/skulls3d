
#include <Math.h>
#include "F002Bilinear.h"
#include "ZInterpolator.h"

F002Bilinear::F002Bilinear(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
                                           const Vec3f & t0, const Vec3f & t1, const Vec3f & t2,
										   Tex *t, ViewPort * vp)
{
	this->t = t;
	this->m_viewport = vp;
	if (v0.y <= v1.y && v1.y <= v2.y) {
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		this->t0 = t0;
		this->t1 = t1;
		this->t2 = t2;
	} else if (v0.y <= v2.y && v2.y <= v1.y) {
		this->v0 = v0;
		this->v1 = v2;
		this->v2 = v1;
		this->t0 = t0;
		this->t1 = t2;
		this->t2 = t1;
	} else if (v1.y <= v0.y && v0.y <= v2.y) {
		this->v0 = v1;
		this->v1 = v0;
		this->v2 = v2;
		this->t0 = t1;
		this->t1 = t0;
		this->t2 = t2;
	} else if (v1.y <= v2.y && v2.y <= v0.y) {
		this->v0 = v1;
		this->v1 = v2;
		this->v2 = v0;
		this->t0 = t1;
		this->t1 = t2;
		this->t2 = t0;
	} else if (v2.y <= v0.y && v0.y <= v1.y) {
		this->v0 = v2;
		this->v1 = v0;
		this->v2 = v1;
		this->t0 = t2;
		this->t1 = t0;
		this->t2 = t1;
	} else if (v2.y <= v1.y && v1.y <= v0.y) {
		this->v0 = v2;
		this->v1 = v1;
		this->v2 = v0;
		this->t0 = t2;
		this->t1 = t1;
		this->t2 = t0;
	}
}

void F002Bilinear::FillPoly(unsigned char shade, float *zbuffer)
{
	int y0 = (int) floor(v0.y);
	int y1 = (int) floor(v1.y);
	int y2 = (int) floor(v2.y);
	int xa = (int) floor(v0.x);
	int xb = (int) floor(v1.x);
	int xc = (int) floor(v2.x);
	bool line2_on_right; // true if line2 is on the right and we must choose its highest x value

	if (y0==y2) return; // All on 1 scanline, not worth drawing

	int y, ymax;
	float x1f, x2f; // reference points for the (u,v)s
	float ratio1, ratio2;
	ZInterpolator zi1, zi2;
	QInterpolator qpixel;
	float displacement;

	// Init lines
	displacement = y0+0.5f - v0.y;
	ratio2 = (v2.x-v0.x) / (v2.y-v0.y); // never div#0 because y0!=y2
	zi2.Init(ZInterpolator::VERTICAL, v0, v2, t0, t2);
	zi2.DisplaceStartingPoint(displacement);
	x2f = v0.x + ratio2*displacement;

	if (y1 <= m_viewport->m_y || y0 == y1) {
		// Start drawing at y1
		if (y1 <= m_viewport->m_y) {
			// Skip first half of triangle
			zi2.DisplaceStartingPoint((float)(y1-y0));
			x2f = v0.x + ratio2*(y1-y0);
		}
		displacement = y1+0.5f - v1.y;
		ratio1 = (v2.x-v1.x) / (v2.y-v1.y); // never div#0 because y1!=y2
		zi1.Init(ZInterpolator::VERTICAL, v1, v2, t1, t2);
		zi1.DisplaceStartingPoint(displacement);
		x1f = v1.x + ratio1*displacement;
		line2_on_right = ratio2 < ratio1;
		y = y1;
		ymax = y2;
	} else {
		// Normal case (y0 != y1)
		ratio1 = (v1.x-v0.x) / (v1.y-v0.y); // never div#0 because y1!=y0
		zi1.Init(ZInterpolator::VERTICAL, v0, v1, t0, t1);
		zi1.DisplaceStartingPoint(displacement);
		x1f = v0.x + ratio1*displacement;
		line2_on_right = ratio2 > ratio1;
		y = y0;
		ymax = y1;
	}

	//if (ratio1-ratio2 < 0.01 && ratio1-ratio2 > -0.01) {
	//	// Triangle too small, not worth drawing;
	//	return;
	//}

	if (y < m_viewport->m_y) {
		zi1.DisplaceStartingPoint((float)(m_viewport->m_y-y));
		zi2.DisplaceStartingPoint((float)(m_viewport->m_y-y));
		x1f += ratio1 * (m_viewport->m_y-y);
		x2f += ratio2 * (m_viewport->m_y-y);
		y = m_viewport->m_y;
	}

	while (y <= ymax  &&  y < m_viewport->m_y + m_viewport->m_h)
	{
		int x1, x2;
		
		// Set of variables used for mipmapping
		int ualphastep_x;
		int mmp; // divider, to get texture coordinates right in the filler loop
		int mmpe; // 1<<mmp
		unsigned int *tbitmap;
		unsigned int twidth;
		unsigned int theight;
		unsigned int *video;
		// End if variables used for mipmapping

		if (line2_on_right) {			
			if (ratio2>0) {
				if (y == y2) {
					x2 = xc;
				} else {
					x2 = (int) (x2f + ratio2*0.5f);
				}
			} else {
				if (y == y0) {
					x2 = xa;
				} else {
					x2 = (int) (x2f - ratio2*0.5f);
				}
			}
			if (ratio1<0) {
				if (y == y1) {
					x1 = xb;
				} else if (y == y2) {
					x1 = xc;
				} else {
					x1 = (int) (x1f + ratio1*0.5f);
				}
			} else {
				if (y == y0) {
					x1 = xa;
				} else if (y == y1) {
					x1 = xb;
				} else {
					x1 = (int) (x1f - ratio1*0.5f);
				}
			}
		} else {
			if (ratio2<0) {
				if (y == y2) {
					x1 = xc;
				} else {
					x1 = (int) (x2f + ratio2*0.5f);
				}
			} else {
				if (y == y0) {
					x1 = xa;
				} else {
					x1 = (int) (x2f - ratio2*0.5f);
				}
			}
			if (ratio1>0) {
				if (y == y1) {
					x2 = xb;
				} else if (y == y2) {
					x2 = xc;
				} else {
					x2 = (int) (x1f + ratio1*0.5f);
				}
			} else {
				if (y == y0) {
					x2 = xa;
				} else if (y == y1) {
					x2 = xb;
				} else {
					x2 = (int) (x1f - ratio1*0.5f);
				}
			}
		}
		
		Vec3f linev0(x1f, zi1.ualpha.z);
		Vec3f linev1(x2f, zi2.ualpha.z);
		if (x1f <= x2f) {
			qpixel.Init(ZInterpolator::HORIZONTAL, linev0, linev1, zi1.ualpha, zi2.ualpha);
			displacement = x1+0.5f - linev0.x;
		} else {
			qpixel.Init(ZInterpolator::HORIZONTAL, linev1, linev0, zi2.ualpha, zi1.ualpha);
			displacement = x1+0.5f - linev1.x;
		}
		if (x1 < m_viewport->m_x) {
			displacement += (float)(m_viewport->m_x - x1);
			x1 = m_viewport->m_x;
		}
		qpixel.DisplaceStartingPoint(displacement/*, x2-x1*/);

		//ASSERT(qpixel.ualpha.x < 1250);
		//ASSERT(qpixel.ualpha.x > -1000);

		if (x2 >= m_viewport->m_x + m_viewport->m_w) {
			x2 = m_viewport->m_x + m_viewport->m_w - 1;
		}

		//ASSERT(x1 <= x2 || x1 >= m_viewport->m_x + m_viewport->m_w);
		//ASSERT(x1 >= m_viewport->m_x);

		// Init mipmapping
		//ualphastep_x = (int)sqrt( ((qpixel.ualphastep.x*qpixel.ualphastep.x)/8) + ((qpixel.ualphastep.y*qpixel.ualphastep.y)/8) );
		ualphastep_x = abs((int)qpixel.ualphastep.x);
		//ualphastep_x = abs((int)( (((qpixel.topalpha+qpixel.topstep)/(qpixel.bottomalpha+qpixel.bottomstep))-qpixel.ualpha).x ));
		mmp = 0;
		mmpe = 1;
		while (ualphastep_x > 1 && mmp+1 < MIPMAPS_COUNT) {
			mmp++;
			mmpe <<= 1;
			ualphastep_x >>= 1;
		}
		tbitmap = t->m_mipmaps[mmp].m_bitmap;
		twidth = t->m_mipmaps[mmp].m_width;
		theight = t->m_mipmaps[mmp].m_height;
		// End of mipmapping init

		video = &((unsigned int*)m_viewport->m_screen->pixels)[(int) ( y*m_viewport->m_screen->pitch/4 + x1)];

		for ( ; x1 <= x2 ; x1++ )
		{
			int u, v;
			int r, g, b;
			float actualx, actualy;
			float nextx, nexty;
			float z = qpixel.ualpha.z;
			if (linev0.z < linev1.z) {
				if (z < linev0.z) {
					z = linev0.z;
				} else if (z > linev1.z) {
					z = linev1.z;
				}
			} else {
				if (z < linev1.z) {
					z = linev1.z;
				} else if (z > linev0.z) {
					z = linev0.z;
				}
			}
			if (z > 0.001 // Ugly z-near culling
				&& z < zbuffer[y*m_viewport->m_w+x1])
			{
				zbuffer[y*m_viewport->m_w+x1] = z;

				actualx = (qpixel.ualpha.x) / mmpe - 0.5f;
				actualy = (qpixel.ualpha.y) / mmpe - 0.5f;

				if (actualx > -1 && actualx < (int)twidth && actualy > -1 && actualy < (int)theight)
				{
					u = ((int)actualx);
					v = ((int)actualy);
				
					//* Bilinear filtering
					actualx = u+1 - actualx;
					nextx = 1 - actualx;
					actualy = v+1 - actualy;
					nexty = 1 - actualy;
				
					if (u+1>=(int)twidth)
						u=(int)twidth-2;
					if (v+1>=(int)theight)
						v=(int)theight-2;

					v *= twidth;
					unsigned int *directbitmap = &tbitmap[u+v];
					r = (int)( (((*(directbitmap         ))>>16)&0xFF) * actualx*actualy
							  +(((*(directbitmap+twidth  ))>>16)&0xFF) * actualx*nexty
							  +(((*(directbitmap       +1))>>16)&0xFF) * nextx*actualy
							  +(((*(directbitmap+twidth+1))>>16)&0xFF) * nextx*nexty );
					g = (int)( (((*(directbitmap         ))>> 8)&0xFF) * actualx*actualy
							  +(((*(directbitmap+twidth  ))>> 8)&0xFF) * actualx*nexty
							  +(((*(directbitmap       +1))>> 8)&0xFF) * nextx*actualy
							  +(((*(directbitmap+twidth+1))>> 8)&0xFF) * nextx*nexty );
					b = (int)(  ((*(directbitmap         ))     &0xFF) * actualx*actualy
							  + ((*(directbitmap+twidth  ))     &0xFF) * actualx*nexty
							  + ((*(directbitmap       +1))     &0xFF) * nextx*actualy
							  + ((*(directbitmap+twidth+1))     &0xFF) * nextx*nexty );
					/**/

					/* No Bilinear Filtering
					v *= twidth;
					r = tbitmap[u+v]>>16;
					g = (tbitmap[u+v]>>8)&0xFF;
					b = tbitmap[u+v]&0xFF;
					/**/

					// Camera lighting
					r = ((r*shade) << 8) & 0xFF0000;
					g =  (g*shade)       & 0xFF00;
					b = ((b*shade) >> 8) & 0xFF;
					// End of camera lighting
					*video = r|g|b;
				}
			}

			video++;
			qpixel.Step();

			if (qpixel.quake == 0)
			{
				// Time to recalculate mipmapping
				//ualphastep_x = (int)sqrt( ((qpixel.ualphastep.x*qpixel.ualphastep.x)/8) + ((qpixel.ualphastep.y*qpixel.ualphastep.y)/8) );
				ualphastep_x = abs((int)qpixel.ualphastep.x);
				//ualphastep_x = abs((int)( (((qpixel.topalpha+qpixel.topstep)/(qpixel.bottomalpha+qpixel.bottomstep))-qpixel.ualpha).x ));
				mmp = 0;
				mmpe = 1;
				while (ualphastep_x > 1 && mmp+1 < MIPMAPS_COUNT) {
					mmp++;
					mmpe <<= 1;
					ualphastep_x >>= 1;
				}
				tbitmap = t->m_mipmaps[mmp].m_bitmap;
				twidth = t->m_mipmaps[mmp].m_width;
				theight = t->m_mipmaps[mmp].m_height;
			}
			
		}

		x1f += ratio1;
		x2f += ratio2;
		zi1.Step();
		zi2.Step();
		y++;
		if (y == y1 && y1 != y2) {
			displacement = y1+0.5f - v1.y;
			ratio1 = (v2.x-v1.x) / (v2.y-v1.y); // never div#0 because y1!=y2
			//if (ratio1-ratio2 < 0.01 && ratio1-ratio2 > -0.01) {
			//	// Triangle too small, not worth drawing;
			//	return;
			//}
			zi1.Init(ZInterpolator::VERTICAL, v1, v2, t1, t2);
			zi1.DisplaceStartingPoint(displacement);
			x1f = v1.x + ratio1*displacement;
			ymax = y2;
		}
	}
}