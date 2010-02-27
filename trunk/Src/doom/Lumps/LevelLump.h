
#ifndef SK_LEVELLUMP
#define SK_LEVELLUMP

#include "Lump.h"
#include "ThingsLump.h"
#include "VertexesLump.h"

namespace doom
{
	class LevelLump : public Lump
	{
	public:
		ThingsLump * m_things;
		VertexesLump* m_vertexes;

		static LevelLump * Get(Lump * lump);

		LevelLump(Lump * lump);

		virtual int Load();
		virtual void UnLoad();

		// TODO : This is for debug only, remove later
		virtual char* ToString();
	};
};

#endif // SK_LEVELLUMP