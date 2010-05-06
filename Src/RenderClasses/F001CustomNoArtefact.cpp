
#include <Math.h>
#include "F001CustomNoArtefact.h"
#include "ZInterpolator.h"

F001CustomNoArtefact::F001CustomNoArtefact(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2,
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

void F001CustomNoArtefact::FillPoly(unsigned char shade)
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
		qpixel.DisplaceStartingPoint(displacement);

		//ASSERT(qpixel.ualpha.x < 1250);
		//ASSERT(qpixel.ualpha.x > -1000);

		if (x2 >= m_viewport->m_x + m_viewport->m_w) {
			x2 = m_viewport->m_x + m_viewport->m_w - 1;
		}

		//ASSERT(x1 <= x2 || x1 >= m_viewport->m_x + m_viewport->m_w);
		//ASSERT(x1 >= m_viewport->m_x);

		// Init mipmapping
		ualphastep_x = (int)sqrt( ((qpixel.ualphastep.x*qpixel.ualphastep.x)/8) + ((qpixel.ualphastep.y*qpixel.ualphastep.y)/8) );
		mmp = 0;
		while (ualphastep_x > 0 && mmp+1 < MIPMAPS_COUNT) {
			mmp++;
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

			if (qpixel.ualpha.z > 0.001) // Ugly z-near culling
			{
				u = (((int)qpixel.ualpha.x)>>mmp);
				v = (((int)qpixel.ualpha.y)>>mmp);
				if (u > -1 && u < (int)twidth && v > -1 && v < (int)theight)
				{
					/*
					ASSERT((((int)qpixel.ualpha.x)>>mmp) >= -10);
					ASSERT((((int)qpixel.ualpha.x)>>mmp) < (int)twidth+10);
					ASSERT((((int)qpixel.ualpha.y)>>mmp) >= -10);
					ASSERT((((int)qpixel.ualpha.y)>>mmp) < (int)theight+10);
					/**/
					*video = tbitmap[v*twidth + u];
				}
			}
			video++;
			qpixel.Step();

			if (qpixel.quake == 0) {
				// Time to recalculate mipmapping
				ualphastep_x = (int)sqrt( ((qpixel.ualphastep.x*qpixel.ualphastep.x)/8) + ((qpixel.ualphastep.y*qpixel.ualphastep.y)/8) );
				mmp = 0;
				while (ualphastep_x > 0 && mmp+1 < MIPMAPS_COUNT) {
					mmp++;
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
