#ifndef SK_VERTEX
#define SK_VERTEX

#include "WadFile.h"

namespace doom
{
	class Vertex
	{
	public:
		short m_x;
		short m_y;	

		Vertex();
		Vertex(short x, short y);
	};
};

#endif // SK_VERTEX