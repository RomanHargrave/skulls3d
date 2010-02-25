
#include "ThingsLump.h"
#include "../Thing.h"
#include "../WadFile.h"

namespace doom
{
	ThingsLump::ThingsLump(Lump * lump)
		:Lump(lump)
	{
	}
	Thing* ThingsLump::operator[](int index) const
	{
		// TODO : maybe index won't be global in m_wadfile->m_lumps
		// but local to this THINGS collection
		return m_things[index];
	}

	int ThingsLump::Load()
	{
		// Each ThingLump has 10 bytes of data
		int count = m_size/10;
		m_things.resize(count);

		printf("%d Things\n", count);

		for (int i=0 ; i<count ; i++)
		{
			m_things[i] = new Thing(m_wadfile, m_position+10*i);
		}

		return 0;
	}
	void ThingsLump::UnLoad()
	{
	}
};