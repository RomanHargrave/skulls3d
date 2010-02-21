
#include "ThingsLump.h"
#include "ThingLump.h"
#include "../WadFile.h"

namespace doom
{
	ThingLump* ThingsLump::operator[](int index) const
	{
		return dynamic_cast<ThingLump*> (m_wadfile->GetLump(index));
	}

	void ThingsLump::Load()
	{
	}
	void ThingsLump::UnLoad()
	{
	}
};