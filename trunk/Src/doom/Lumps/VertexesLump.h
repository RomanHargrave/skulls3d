#ifndef SK_VERTEXESLUMP
#define SK_VERTEXESLUMP

#include <vector>
#include "Lump.h"
#include "../Vertex.h"

namespace doom
{
	class VertexesLump : public Lump
	{
	protected:
		std::vector<Vertex*> m_vertexes;

	public:
		short size;

		VertexesLump(Lump * lump);
		Vertex * Get(int index);		
		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_VERTEXESLUMP