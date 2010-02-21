
#include "ThingsLump.h"
#include "ThingLump.h"
#include "../WadFile.h"

namespace doom
{
	ThingLump* ThingsLump::operator[](int index) const
	{
		return dynamic_cast<ThingLump*> (m_wadfile->GetLump(index));
	}

	int ThingsLump::Load()
	{
		return 0;
	}
	void ThingsLump::UnLoad()
	{
	}
};