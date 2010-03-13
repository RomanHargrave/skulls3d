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
	public:
		Vertex(WadFile * wadFile, int offset);
	};
};

#endif // SK_VERTEX