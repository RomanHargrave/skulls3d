
#include "ThingsLump.h"
#include "ThingLump.h"
#include "../WadFile.h"

namespace Doom
{
	ThingLump& ThingsLump::operator[](int index) const
	{
		return dynamic_cast<ThingLump&> (m_wadfile->GetLump(index));
	}
};