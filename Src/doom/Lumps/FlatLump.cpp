
#include <stdlib.h>

#include "FlatLump.h"
#include "Lump.h"
#include "PlayPalLump.h"
#include "../Wad.h"

namespace skulls
{

	Flat::Flat(File & file, Lump & lump, Palettes & palettes)
	{
		auto oldPosition = file.GetPos();

		file.MoveTo(lump.m_position);

		// Create bitmap
		for (int pix=0 ; pix<4096 ; pix++)
			m_bitmap[pix] = palettes.m_palette[(unsigned char)file.ReadInt1()];

		file.MoveTo(oldPosition);
	}

};
