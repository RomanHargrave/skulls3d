
#ifndef SK_FLAT
#define SK_FLAT

#include "Lump.h"
#include "..\..\File.h"
#include "PlayPalLump.h"

namespace skulls
{
	class Flat
	{
	public:
		Flat(File & file, Lump & lump, Palettes & palettes);

	public:
		unsigned int m_bitmap[4096];
	};
};

#endif // SK_FLAT