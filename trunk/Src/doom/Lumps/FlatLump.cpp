
#include <stdlib.h>

#include "FlatLump.h"
#include "Lump.h"
#include "PlayPalLump.h"
#include "../WadFile.h"

namespace doom
{
	FlatLump::FlatLump(Lump * other)
		:Lump(other)
	{
		m_bitmap = NULL;
	}

	bool FlatLump::Load()
	{
		if (m_bitmap != NULL)
			return true; //Already loaded
		
		m_wadfile->MoveTo(m_position);

		// Create bitmap
		m_bitmap = new unsigned int[4096];

		for (int pix=0 ; pix<4096 ; pix++)
		{
			unsigned char color;
			m_wadfile->ReadInt1((char*)&color);
			m_bitmap[pix] = m_wadfile->m_palettes->m_palette[color];
		}
		return true;
	}
	
	void FlatLump::UnLoad()
	{
		Lump::UnLoad();

		if (m_bitmap != NULL)
		{
			delete m_bitmap;
			m_bitmap = NULL;
		}
	}
};
