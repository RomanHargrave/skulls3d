
#include "Seg.h"
#include "Vertex.h"

namespace doom
{
	Seg::Seg(Vertex *vert1, Vertex *vert2, bool dir)
	{
		v1 = vert1;
		v2 = vert2;
		direction = dir;
	}
};
