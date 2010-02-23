
#include <stdlib.h>
#include "ThingLump.h"
#include "..\WadFile.h"

namespace doom
{
	ThingLump * ThingLump::Get(Lump * lump)
	{
		ThingLump * result = dynamic_cast<ThingLump*>(lump);
		if (result == NULL)
		{
			// Lump has not been transformed yet, transform it now
			result = new ThingLump(lump);
			// Update the dictionary reference and dispose of the old lump
			result->m_wadfile->SetLump(result);
		}	
		return result;
	}

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
