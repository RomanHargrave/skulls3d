
#include "R0033Avg.h"

R0033Avg::R0033Avg(Scene *scene, Camera *camera, ViewPort *viewport)
:R0030FlatVirtual(scene, camera, viewport)
{
}

void R0033Avg::FillOrderedPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade)
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

		int fillstart = (int) ((x1+next_x1)/2);
		int fillend = (int) ((x2+next_x2)/2);
		if (fillstart > fillend)
		{
			int tmp = fillstart;
			fillstart = fillend;
			fillend = tmp;
		}

		m_viewport->HLine(fillstart, fillend, y, shade);

		x1 = next_x1;
		x2 = next_x2;

		if (y == y0 && y == y1) {
			ratio1 = (v2.x-v1.x) / (v2.y-v1.y);
			x1 = v1.x + ratio1 * (1-(v1.y-y1));
			x2 = x2 + ratio2 * (1-(v0.y-y0));
			ymax = (int) v2.y;
		} else if (y == y1) {
			ratio1 = (v2.x-v1.x) / (v2.y-v1.y);
			ymax = y2;
		}
	}
}
