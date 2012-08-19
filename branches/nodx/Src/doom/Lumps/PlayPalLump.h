
#ifndef SK_PLAYPALLUMP
#define SK_PLAYPALLUMP

#include <vector>
#include "Lump.h"
#include "..\..\File.h"

namespace skulls
{
	class Palettes
	{
	public:
		Palettes(File &, Lump &);

	public:
		std::vector<unsigned int> m_palette;
	};
};

#endif // SK_PLAYPALLUMP