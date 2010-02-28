
#include <stdio.h>
#include <memory.h>
#include "PatchLump.h"
#include "PlayPalLump.h"
#include "../WadFile.h"

namespace doom
{
	PatchLump::PatchLump(Lump * lump)
		:Lump(lump)
	{
		// Debug
		printf("%s is a Patch Lump\n", m_name);
	}
	PatchLump::~PatchLump()
	{
		if (m_texture != NULL)
			delete m_texture;
	}
	

	int PatchLump::Load()
	{
		m_wadfile->MoveTo(m_position);
		m_wadfile->ReadInt2((short*)&m_w);
		m_wadfile->ReadInt2((short*)&m_h);
		m_wadfile->Skip(4); // Remaining of the header

		// Create bitmap
		m_texture = new unsigned int[m_w*m_h];
		// TODO: set to transparent
		memset(m_texture, m_w*m_h, 0);

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
						return 1;
					}
					unsigned char color;
					m_wadfile->ReadInt1((char*)&color);
					m_texture[(row+pix)*m_w + i] = m_wadfile->m_palettes->m_palette[color];
				}
				m_wadfile->Skip(1);
			}while(1);
		}

		delete col_offset;
		return 0;
	}
	void PatchLump::UnLoad()
	{
		Lump::UnLoad();
		if (m_texture != NULL)
			delete m_texture;
	}
};