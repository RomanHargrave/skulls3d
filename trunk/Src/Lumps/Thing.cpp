
#include <stdlib.h>
#include "ThingLump.h"
#include "..\WadFile.h"

namespace doom
{
	ThingLump::ThingLump(Lump * lump)
		:Lump(lump)
	{
	}

	int ThingLump::Load()
	{
		return 0;
	}
	void ThingLump::UnLoad()
	{
	}
};
