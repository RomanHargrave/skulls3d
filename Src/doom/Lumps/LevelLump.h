
#ifndef SK_LEVELLUMP
#define SK_LEVELLUMP

#include <vector>
#include "Lump.h"

namespace doom
{
	class Thing;
	class Vertex;
	class Sector;
	class SideDef;
	class LineDef;

	class LevelLump : public Lump
	{
	public:
		std::vector<Thing*> m_things;
		std::vector<Vertex*> m_vertexes;
		std::vector<LineDef*> m_lineDefs;
		std::vector<SideDef*> m_sideDefs;
		std::vector<Sector*> m_sectors;

		LevelLump(Lump * lump);

		virtual int Load();
		virtual void UnLoad();

		Sector* GetSectorByTag(unsigned short tag);
	};
};

#endif // SK_LEVELLUMP