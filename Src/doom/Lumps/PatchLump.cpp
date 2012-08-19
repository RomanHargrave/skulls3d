
#include <stdio.h>
#include <memory.h>
#include "PatchLump.h"
#include "../Wad.h"
#include "Lump.h"
#include "PlayPalLump.h"

namespace skulls
{
	Patch::Patch(File & file, Lump & lump, Palettes & palettes)
	{
		file.MoveTo(lump.m_position);

		m_w = file.ReadInt2();
		m_h = file.ReadInt2();
		file.Skip(4); // Remaining of the header

		// Create bitmap
		m_bitmap.resize(m_w*m_h, 0xFF000000);

		std::vector<unsigned int> col_offset(m_w, 0);
		for (int i=0 ; i<m_w ; i++)
			col_offset[i] = file.ReadInt4();

		unsigned char row, len;
		for (int i=0 ; i<m_w ; i++)
		{
			file.MoveTo(lump.m_position+col_offset[i]);
			do
			{
				row = file.ReadInt1();
				if (row == 0xFF)
					break; // end of the column
				len = file.ReadInt1();
				file.Skip(1);
				for (int pix=0 ; pix<len ; pix++)
				{
					if ((row+pix) >= m_h)
					{
						return;
					}
					unsigned char color;
					color = file.ReadInt1();
					m_bitmap[(row+pix)*m_w + i] = palettes.m_palette[color];
				}
				file.Skip(1);
			}while(1);
		}
	}

};