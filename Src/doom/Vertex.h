#ifndef SK_VERTEX
#define SK_VERTEX

#include <list>

namespace doom
{
	class WadFile;
	class LineDef;

	class Vertex
	{
	public:
		short m_x;
		short m_z;	
		std::list<LineDef*> m_lineDefs;

		Vertex(short x, short z);
	};
};

#endif // SK_VERTEX