
#ifndef SK_THINGLUMP
#define SK_THINGLUMP

#include "Lump.h"

namespace doom
{
	class ThingLump : public Lump
	{
		static ThingLump * ThingLump::Get(Lump * lump);
			
		ThingLump(Lump * lump);

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_THINGLUMP