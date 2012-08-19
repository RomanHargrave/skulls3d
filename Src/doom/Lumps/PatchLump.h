
#ifndef SK_PATCHLUMP
#define SK_PATCHLUMP

#include <vector>
#include "Lump.h"
#include "..\..\File.h"
#include "PlayPalLump.h"

namespace skulls
{
	class Wad;

	class Patch
	{
	public:
		Patch(File &, Lump &, Palettes &);

	public:
		std::string name;
		unsigned short m_w, m_h;
		std::vector<unsigned int> m_bitmap;
	};
};

#endif // SK_PATCHLUMP