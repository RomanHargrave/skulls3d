#ifndef SK_VERTEX
#define SK_VERTEX

#include <list>
#include "ReferencedObject.h"

namespace doom
{
	class WadFile;
	class LineDef;

	class Vertex : public ReferencedObject
	{
	public:
		int m_x;
		int m_z;	
		std::list<LineDef*> m_lineDefs;

		Vertex(void *referencer, int x, int z);

		virtual bool Load();
		virtual void UnLoad();
	};
};

#endif // SK_VERTEX