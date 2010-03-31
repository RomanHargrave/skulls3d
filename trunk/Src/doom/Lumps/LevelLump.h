
#ifndef SK_LEVELLUMP
#define SK_LEVELLUMP

#include <list>
#include <vector>
#include "Lump.h"
#include "../Seg.h"

namespace doom
{
	class Thing;
	class Vertex;
	class Sector;
	class SideDef;
	class LineDef;
	class Seg;
	class SSector;
	class Node;

	class LevelLump : public Lump
	{
	public:
		std::vector<Thing*> m_things;
		std::vector<Vertex*> m_vertexes;
		std::vector<LineDef*> m_lineDefs;
		std::vector<SideDef*> m_sideDefs;
		std::vector<Sector*> m_sectors;
		std::vector<Seg*> m_segs;
		std::vector<SSector*> m_ssectors;
		//std::vector<SSector*> m_addssectors;
		std::list<Seg*> m_dividers;
		doom::Node *m_bspTree;

		LevelLump(void *referencer, Lump * lump);

		virtual bool Load();
		virtual void UnLoad();

		Sector* GetSectorByTag(unsigned short tag);
	};
};

#endif // SK_LEVELLUMP