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
		int m_x;
		int m_z;	
		std::list<LineDef*> m_lineDefs;

		Vertex(int x, int z);
	};
};

#endif // SK_VERTEX