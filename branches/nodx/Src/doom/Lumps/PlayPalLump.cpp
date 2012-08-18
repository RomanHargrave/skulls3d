
#include <stdlib.h>
#include "PlayPalLump.h"
#include "../WadFile.h"

namespace doom
{
	PlayPalLump::PlayPalLump(Lump * other)
		:Lump(other)
	{
		m_palette = NULL;
	}

	bool PlayPalLump::Load()
	{
		if (m_palette != NULL)
			return true; // Already loaded

		m_wadfile->MoveTo(m_position);

		m_palette = new unsigned int[256];
		for (int x=0 ; x<256 ; x++)
		{
			unsigned char r,g,b;
			m_wadfile->ReadInt1((char*)&r);
			m_wadfile->ReadInt1((char*)&g);
			m_wadfile->ReadInt1((char*)&b);
			m_palette[x] = (r<<16) | (g<<8) | b;
		}

		return true;
	}
	void PlayPalLump::UnLoad()
	{
		Lump::UnLoad();

		if (m_palette != NULL)
		{
			delete m_palette;
			m_palette = NULL;
		}
	}
};
