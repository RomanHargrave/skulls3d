
#include "ThingsLump.h"
#include "../Thing.h"
#include "../WadFile.h"

namespace doom
{
	ThingsLump::ThingsLump(Lump * lump)
		:Lump(lump)
	{
	}
	ThingsLump::~ThingsLump()
	{
	}

	Thing* ThingsLump::operator[](int index) const
	{
		return m_things[index];
	}

	int ThingsLump::Load()
	{
		if (m_things.size() != 0)
			return 0; //Already loaded

		// Each Thing has 10 bytes of data
		int count = m_size/10;
		m_things.resize(count);

		//printf("%d Things\n", count);

		for (int i=0 ; i<count ; i++)
		{
			m_things[i] = new Thing(m_wadfile, m_position+10*i);
		}

		return 0;
	}
	void ThingsLump::UnLoad()
	{
		Lump::UnLoad();

		m_things.clear();
	}
};