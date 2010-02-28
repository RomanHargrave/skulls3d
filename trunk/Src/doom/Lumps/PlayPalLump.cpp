
#include <stdlib.h>
#include "PlayPalLump.h"
#include "../WadFile.h"

namespace doom
{
	PlayPalLump::PlayPalLump(Lump * other)
		:Lump(other)
	{
		m_palettes = NULL;
	}

	int PlayPalLump::Load()
	{
		m_wadfile->MoveTo(m_position);

		m_palettes = new unsigned int*[14];
		for (int i=0 ; i<14 ; i++)
		{
			m_palettes[i] = new unsigned int[256];
			for (int x=0 ; x<256 ; x++)
			{
				unsigned char r,g,b;
				m_wadfile->ReadInt1((char*)&r);
				m_wadfile->ReadInt1((char*)&g);
				m_wadfile->ReadInt1((char*)&b);
				m_palettes[i][x] = (r<<16) | (g<<8) | b;
			}
		}

		return 0;
	}
	void PlayPalLump::UnLoad()
	{
	}
};
