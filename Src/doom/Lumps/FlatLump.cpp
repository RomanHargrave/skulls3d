
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
		m_texture = NULL;
	}

	int FlatLump::Load()
	{
		if (m_texture != NULL)
			return 0; //Already loaded
		
		m_wadfile->MoveTo(m_position);

		// Create bitmap
		m_texture = new unsigned int[4096];

		for (int pix=0 ; pix<4096 ; pix++)
		{
			unsigned char color;
			m_wadfile->ReadInt1((char*)&color);
			m_texture[pix] = m_wadfile->m_palettes->m_palette[color];
		}
		return 0;
	}
	
	void FlatLump::UnLoad()
	{
		Lump::UnLoad();

		if (m_texture != NULL)
			delete m_texture;
		m_texture = NULL;
	}
};
