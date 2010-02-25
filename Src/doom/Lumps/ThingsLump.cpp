
#include "ThingsLump.h"
#include "../Thing.h"
#include "../WadFile.h"

namespace doom
{
	ThingsLump::ThingsLump(Lump * lump)
		:Lump(lump)
	{
		// Each ThingLump has 10 bytes of data
		m_things.resize(m_size/10);
	}
	Thing* ThingsLump::operator[](int index) const
	{
		// TODO : maybe index won't be global in m_wadfile->m_lumps
		// but local to this THINGS collection
		return m_things[index];
	}

	int ThingsLump::Load()
	{
		return 0;
	}
	void ThingsLump::UnLoad()
	{
	}
};