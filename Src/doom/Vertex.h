#ifndef SK_VERTEX
#define SK_VERTEX

#include <list>

namespace skulls
{
	class Wad;
	class LineDef;

	class Vertex
	{
	public:
		Vertex(int x, int z);

	public:
		int m_x;
		int m_z;	
		std::list<std::shared_ptr<LineDef>> m_lineDefs;
	};
};

#endif // SK_VERTEX