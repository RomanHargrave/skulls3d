
#ifndef SK_SECTOR
#define SK_SECTOR

#include <list>
#include "LumpDictionary.h"
#include "Lumps\PlayPalLump.h"
#include "Lumps\FlatLump.h"
#include "Sidedef.h"

namespace skulls
{

	class Sector
	{
	public:
		Sector::Sector(File & file, LumpDictionary & dictionary, Palettes & palettes);

	public:
		short m_floorHeight;
		short m_ceilingHeight;
		std::shared_ptr<Flat> m_floorTexture;
		std::shared_ptr<Flat> m_ceilingTexture;
		unsigned short m_lightLevel;
		unsigned short m_type;
		unsigned short m_tag;
		std::list<std::shared_ptr<SideDef>> m_sideDefs;
	};
};

#endif // SK_SECTOR
