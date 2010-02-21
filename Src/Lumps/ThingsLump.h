
#ifndef SK_THINGSLUMP
#define SK_THINGSLUMP

#include "Lump.h"
#include "ThingLump.h"

namespace doom
{
	class ThingsLump : public Lump
	{
	protected:
		ThingLump * operator[](int index) const;

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_THINGSLUMP