#ifndef SK_VERTEXESLUMP
#define SK_VERTEXESLUMP

#include <vector>
#include "Lump.h"
#include "../Vertexes.h"

namespace doom
{
	class VertexesLump : public Lump
	{
	protected:
		std::vector<Vertexes*> m_vertexes;

	public:
		VertexesLump(Lump * lump);

		Vertexes * operator[](int index) const;

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_VERTEXESLUMP