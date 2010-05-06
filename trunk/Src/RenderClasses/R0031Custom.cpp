
#include "R0031Custom.h"


R0031Custom::R0031Custom(Scene *scene, Camera *camera, ViewPort *viewport)
:R0030FlatVirtual(scene, camera, viewport)
{
}

void R0031Custom::FillOrderedPoly(const Vec3f & va, const Vec3f & vb, const Vec3f & vc, unsigned char shade)
{
	Vec3f v0 = va;
	Vec3f v1 = vb;
	Vec3f v2 = vc;
	v0.x += 0.5f; v0.y += 0.5f; v0.z += 0.5f;
	v1.x += 0.5f; v1.y += 0.5f; v1.z += 0.5f;
	v2.x += 0.5f; v2.y += 0.5f; v2.z += 0.5f;

	int y = (int) v0.y;
	int ymax = (int) v1.y; // go to v1 then to v2
	float x1 = v0.x; // start at v0 and go toward v1
	float x2 = v0.x; // start at v0 and go toward v2
	float x1mov; // displacement for x1
	float x2mov; // displacement for x2
	
	if ((int)v0.y == (int)v2.y)
	{
		if (v0.x <= v2.x)
			for (int x=(int)v0.x ; x<=(int)v2.x ; x++)
				m_viewport->ShowPoint(x,y, shade);
		else
			for (int x=(int)v2.x ; x<=(int)v0.x ; x++)
				m_viewport->ShowPoint(x,y, shade);
		return;
	}

	if ((int)v0.y != (int)v1.y)
	{
		x1mov = (v1.x-v0.x)/((int)v1.y-(int)v0.y);
		x2mov = (v2.x-v0.x)/((int)v2.y-(int)v0.y);
		x1 += x1mov/2;
		x2 += x2mov/2;
		for ( ; y<ymax ; y++)
		{
			if (y >= this->m_viewport->m_h)
				break;
			if (y>=0)
			{
				int x = (int) ((x1<x2)?x1:x2);
				int xmax = (int) ((x1<x2)?x2:x1);
				if (x<0) x=0;
				if (xmax>=m_viewport->m_w) xmax = m_viewport->m_w-1;
				
				for ( ; x<=xmax ; x++)
					m_viewport->ShowPoint(x,y, shade);
			}
			x1 += x1mov;
			x2 += x2mov;
		}
		x1 -= x1mov/2;
		x2 -= x2mov/2;
	}
	else
	{
		if (v0.x <= v1.x)
			for (int x=(int)v0.x ; x<=(int)v1.x ; x++)
				m_viewport->ShowPoint(x,y, shade);
		else
			for (int x=(int)v1.x ; x<=(int)v0.x ; x++)
				m_viewport->ShowPoint(x,y, shade);
		x1 = v1.x;
		x2 = v0.x;
	}

	
	if ((int)v1.y != (int)v2.y)
	{
		x1mov = (v2.x-v1.x)/((int)v2.y-(int)v1.y);
		x2mov = (v2.x-v0.x)/((int)v2.y-(int)v0.y);
		ymax = (int) v2.y;

		if (y>=0 && y<this->m_viewport->m_h)
		{
			int x = (int) ((x1<x2)?x1:x2);
			int xmax = (int) ((x1<x2)?x2:x1);
			if (x<0) x=0;
			if (xmax>=m_viewport->m_w) xmax = m_viewport->m_w-1;
			
			for ( ; x<=xmax ; x++)
				m_viewport->ShowPoint(x,y, shade);

			y++;
			x1 += x1mov/2;
			x2 += x2mov/2;
		}

		for ( ; y<=ymax ; y++)
		{
			if (y >= this->m_viewport->m_h)
				break;
			if (y>=0)
			{
				int x = (int) ((x1<x2)?x1:x2);
				int xmax = (int) ((x1<x2)?x2:x1);
				if (x<0) x=0;
				if (xmax>=m_viewport->m_w) xmax = m_viewport->m_w-1;
				
				for ( ; x<=xmax ; x++)
					m_viewport->ShowPoint(x,y, shade);
			}
			x1 += x1mov;
			x2 += x2mov;
		}
	}
	else
	{
		if (v1.x <= v2.x)
			for (int x=(int)v1.x ; x<=(int)v2.x ; x++)
				m_viewport->ShowPoint(x,y, shade);
		else
			for (int x=(int)v2.x ; x<=(int)v1.x ; x++)
				m_viewport->ShowPoint(x,y, shade);
	}
}
