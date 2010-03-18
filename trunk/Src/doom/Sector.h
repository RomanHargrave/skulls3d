
#ifndef SK_SECTOR
#define SK_SECTOR

#include <list>

namespace doom
{
	class WadFile;
	class FlatLump;
	class SideDef;

	class Sector
	{
	public:
		short m_floorHeight;
		short m_ceilingHeight;
		FlatLump *m_floorTexture;
		FlatLump *m_ceilingTexture;
		unsigned short m_lightLevel;
		unsigned short m_type;
		unsigned short m_tag;
		std::list<SideDef*> m_sideDefs;

		Sector(WadFile *wadFile);
	};
};

#endif // SK_SECTOR
