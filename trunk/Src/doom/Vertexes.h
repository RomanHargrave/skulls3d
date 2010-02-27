#ifndef SK_VERTEXES
#define SK_VERTEXES

#include "WadFile.h"

namespace doom
{
	class Vertexes
	{
	protected:
		short m_x;
		short m_y;		
	public:
		Vertexes(WadFile * wadFile, int offset);
	};
};

#endif // SK_VERTEXES