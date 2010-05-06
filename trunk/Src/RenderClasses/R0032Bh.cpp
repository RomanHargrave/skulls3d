
#include "R0032Bh.h"


typedef struct
{
	bool horizontal;
	bool highest; // for horizontal lines whether it should give the highest or the lowest x
	float error;
	float deltaerr;
	int x, y;
	int pmax;
	int inc;
} BhData;
int BhGetCurrentXThenStep(BhData & data);
void InitBh(BhData & data, const Vec3f & v0, const Vec3f & v1, bool highest);



R0032Bh::R0032Bh(Scene *scene, Camera *camera, ViewPort *viewport)
:R0030FlatVirtual(scene, camera, viewport)
{
}

void R0032Bh::FillOrderedPoly(const Vec3f & v0, const Vec3f & v1, const Vec3f & v2, unsigned char shade)
{
	float ratio1 = (v1.x-v0.x)/(v1.y-v0.y);
	float ratio2 = (v2.x-v0.x)/(v2.y-v0.y);

	BhData bres1, bres2;
	InitBh(bres1, v0, v1, ratio1>ratio2);
	InitBh(bres2, v0, v2, ratio2>ratio1);

	int y0 = (int) v0.y;
	int y1 = (int) v1.y;
	int y2 = (int) v2.y;
	int y, ymax;

	y = y0;
	ymax = y1;
	do {
		int x1 = BhGetCurrentXThenStep(bres1);
		int x2 = BhGetCurrentXThenStep(bres2);
		if (x1>x2) {
			int x = x1;
			x1 = x2;
			x2 = x;
		}
		m_viewport->HLine(x1, x2, y, shade);
		y++;
		if (y >= ymax  &&  ymax != y2) {
			InitBh(bres1, v1, v2, ratio1>ratio2);
			ymax = y2;
		}	
	}while (y <= ymax);
}


/**
 * Returns the maximum X for this Y and steps to the next Y
 * whatever orientation (mostly vertical or mostly horizontal)
 */
int BhGetCurrentXThenStep(BhData & data)
{
	if (data.horizontal == false) {
		int result = data.x;

		data.error += data.deltaerr;
		if (data.error > 0.5f) {
			data.x ++;
			data.error -= 1.0f;
		} else if (data.error < -0.5f) {
			data.x --;
			data.error += 1.0f;
		}
		data.y++;
		return result;
	} else {
		int beginning = data.x;
		for ( ; data.x != data.pmax ; data.x+=data.inc) {
			data.error += data.deltaerr;
			if (data.error > 0.5f) {
				data.error -= 1.0f;
				data.x += data.inc;
				data.y ++;
				if (data.highest) return (beginning>data.x)?beginning:data.x-data.inc;
				else return (beginning<data.x)?beginning:data.x-data.inc;
			}
		}
		if (data.highest) return (beginning>data.x)?beginning:data.x;
		else return (beginning<data.x)?beginning:data.x;
	}
}

void InitBh(BhData & data, const Vec3f & v0, const Vec3f & v1, bool highest)
{
	data.error = 0.0f;
	data.x = (int) v0.x;
	data.y = (int) v0.y;
	data.highest = highest;

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
		if (v0.y < v1.y) {
			data.deltaerr = ((int)v1.x-(int)v0.x) * 1.0f / ((int)v1.y-(int)v0.y);
			data.pmax = (int) v1.y;
			data.inc = 1;
		}
	}
}