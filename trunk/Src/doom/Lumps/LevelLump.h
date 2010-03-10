
#ifndef SK_LEVELLUMP
#define SK_LEVELLUMP

#include "Lump.h"
#include "ThingsLump.h"
#include "VertexesLump.h"
#include "LineDefsLump.h"

namespace doom
{
	class LevelLump : public Lump
	{
	public:
		ThingsLump * m_things;
		VertexesLump* m_vertexes;
		LineDefsLump* m_linedefs;
		LevelLump(Lump * lump);

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_LEVELLUMP