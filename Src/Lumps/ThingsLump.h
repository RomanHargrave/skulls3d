
#ifndef SK_THINGSLUMP
#define SK_THINGSLUMP

#include "Lump.h"
#include "ThingLump.h"

namespace Doom
{
	class ThingsLump : public Lump
	{
	protected:
		ThingLump& operator[](int index) const;
	};
};

#endif // SK_THINGSLUMP