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
		VertexesLump(Lump * lump);

		Vertex * operator[](int index) const;

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_VERTEXESLUMP