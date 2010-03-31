
#include <stdio.h>
#include <memory.h>
#include "PatchLump.h"
#include "../WadFile.h"
#include "Lump.h"
#include "PlayPalLump.h"

namespace doom
{
	PatchLump::PatchLump(void *referencer, Lump *other)
		:Lump(referencer, other)
	{
		m_bitmap = NULL;
	}

	bool PatchLump::Load()
	{
		if (m_bitmap != NULL)
			return true; // Already loaded

		m_wadfile->MoveTo(m_position);

		m_wadfile->ReadInt2((short*)&m_w);
		m_wadfile->ReadInt2((short*)&m_h);
		m_wadfile->Skip(4); // Remaining of the header

		// Create bitmap
		m_bitmap = new unsigned int[m_w*m_h];
		// TODO: set to transparent
		for (int j=0 ; j<m_h ; j++)
			for (int i=0 ; i<m_w ; i++)
				m_bitmap[j*m_w + i] = 0xFF000000; // alpha = 255

		unsigned int *col_offset = new unsigned int[m_w];
		for (int i=0 ; i<m_w ; i++)
			m_wadfile->ReadInt4((int*)&col_offset[i]);

		unsigned char row, len;
		for (int i=0 ; i<m_w ; i++)
		{
			m_wadfile->MoveTo(m_position+col_offset[i]);
			do
			{
				m_wadfile->ReadInt1((char*)&row);
				if (row == 0xFF)
					break; // end of the column
				m_wadfile->ReadInt1((char*)&len);
				m_wadfile->Skip(1);
				for (int pix=0 ; pix<len ; pix++)
				{
					if ((row+pix) >= m_h)
					{
						delete col_offset;
						goto end;
					}
					unsigned char color;
					m_wadfile->ReadInt1((char*)&color);
					m_bitmap[(row+pix)*m_w + i] = m_wadfile->m_palettes->m_palette[color];
				}
				m_wadfile->Skip(1);
			}while(1);
		}
		end:
		delete col_offset;
		return true;
	}

	void PatchLump::UnLoad()
	{
		if (m_bitmap != NULL)
		{
			delete m_bitmap;
			m_bitmap = NULL;
		}
	}

	PatchLump::~PatchLump()
	{
		UnLoad();
	}
};