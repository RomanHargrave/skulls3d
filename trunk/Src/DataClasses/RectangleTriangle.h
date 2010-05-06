
#ifndef SWE_RECTANGLETRIANGLE
#define SWE_RECTANGLETRIANGLE

#include "Mesh.h"
#include "Tex.h"

class RectangleTriangle : public Mesh
{
public:
	RectangleTriangle(float xlen, float ylen, Tex *t);
};

#endif // SWE_RECTANGLETRIANGLE
