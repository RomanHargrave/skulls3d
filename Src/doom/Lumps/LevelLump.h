
#ifndef SK_Level
#define SK_Level

#include <list>
#include <vector>
#include "Lump.h"
#include "../Seg.h"
#include "..\LumpDictionary.h"
#include "..\..\File.h"
#include "..\Patches.h"
#include "PlayPalLump.h"
#include "..\Texture.h"
#include "..\Thing.h"

namespace skulls
{
	class Thing;
	class Vertex;
	class Sector;
	class SideDef;
	class LineDef;
	class Seg;
	class SSector;
	class Node;
	class LumpDictionary;

	class Level
	{
	public:
		std::vector<Thing> m_things;
		std::vector<std::shared_ptr<Vertex >> m_vertexes;
		std::vector<std::shared_ptr<LineDef>> m_lineDefs;
		std::vector<std::shared_ptr<SideDef>> m_sideDefs;
		std::vector<std::shared_ptr<Sector >> m_sectors;
		std::vector<std::shared_ptr<Seg    >> m_segs;
		std::vector<std::shared_ptr<SSector>> m_ssectors;
		//std::vector<SSector*> m_addssectors;
		std::list<std::shared_ptr<Seg>> m_dividers;
		
		skulls::Node *m_bspTree;

		Level::Level(File & file,
		             LumpDictionary & dictionary,
		             Patches & patches,
		             std::map<std::string,std::shared_ptr<Texture>> & textures,
		             Lump & lump,
		             Palettes & palettes);

		std::shared_ptr<Sector> GetSectorByTag(unsigned short tag);
	};
};

#endif // SK_Level