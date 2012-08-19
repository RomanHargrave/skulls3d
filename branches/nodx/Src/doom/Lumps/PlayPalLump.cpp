
#include <stdlib.h>
#include "PlayPalLump.h"
#include "../Wad.h"

namespace skulls
{
	Palettes::Palettes(File & file, Lump & lump)
	{
		file.MoveTo(lump.m_position);

		m_palette.resize(256, 0);
		for (int x=0 ; x<256 ; x++)
			m_palette[x] = (((unsigned char)file.ReadInt1())<<16) | (((unsigned char)file.ReadInt1())<<8) | (unsigned char)file.ReadInt1();
	}

};
