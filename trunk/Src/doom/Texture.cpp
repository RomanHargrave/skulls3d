
#include <stdlib.h>
#include "Texture.h"
#include "WadFile.h"
#include "lumps/PatchLump.h"

namespace doom
{
	Texture::Texture(WadFile * wadfile, int position)
	{
		m_name = NULL;
		m_bitmap = NULL;
		m_wadfile = wadfile;
		m_position = position;

		// Load name
		int pos = wadfile->GetPos();
		wadfile->MoveTo(position);
		char buffer[9];
		memset(buffer, 0, 9);
		m_wadfile->ReadString(buffer, 8);
		m_name = _strdup(buffer);
		printf("Texture %s\n", m_name);
		wadfile->MoveTo(pos);
	}

	int Texture::Load()
	{
		if (m_bitmap != NULL)
			return 0; // Already loaded

		m_wadfile->MoveTo(m_position);

		// Skipping name, already read
		m_wadfile->Skip(8);

		//Skipping flag
		m_wadfile->Skip(4);

		m_wadfile->ReadInt2((short*)&m_w);
		m_wadfile->ReadInt2((short*)&m_h);

		//Skipping columndirectory
		m_wadfile->Skip(4);

		short PatchLump_count;
		m_wadfile->ReadInt2(&PatchLump_count);

		m_bitmap = new unsigned int[m_w*m_h];

		// Read all PatchLumpes and merge them into m_bitmap
		for (int i=0 ; i<PatchLump_count ; i++)
		{
			short orig_x, orig_y;
			m_wadfile->ReadInt2(&orig_x);
			m_wadfile->ReadInt2(&orig_y);
			
			unsigned short PatchLump_index;
			m_wadfile->ReadInt2((short*)&PatchLump_index);

			short stepdir, colormap;
			m_wadfile->ReadInt2(&stepdir);
			m_wadfile->ReadInt2(&colormap);

			// TODO: We can save memory when there is only one PatchLump
			// and its orig_x and orig_y are 0. In this case this texture
			// is exactly the same as the PatchLump, so m_bitmap may point
			// directly at the PatchLump's bitmap
			// But beware of the UnLoad, a texture shall not free a PatchLump's bitmap

			if (PatchLump_index < 0 || PatchLump_index > m_wadfile->m_patches.size())
				continue;

			int file_pos = m_wadfile->GetPos();
			PatchLump * lump = m_wadfile->m_patches[PatchLump_index];
			if (lump == NULL)
				continue;
			m_wadfile->MoveTo(file_pos);

			//printf("\t%s\n", PatchLump->m_name);

			MergePatchLumpIntoTexture(lump, orig_x, orig_y);
		}
		return 0;
	}
	void Texture::UnLoad()
	{
		// Keep the name so the texture can be found in wadfile->m_textures

		if (m_bitmap != NULL)
		{
			delete m_bitmap;
			m_bitmap = NULL;
		}
	}
	
	void Texture::MergePatchLumpIntoTexture(PatchLump * PatchLump, short orig_x, short orig_y)
	{
		PatchLump->Load();
		for (int j=0 ; j<PatchLump->m_h ; j++)
		{
			for (int i=0 ; i<PatchLump->m_w ; i++)
			{
				int color = PatchLump->m_bitmap[j*PatchLump->m_w + i];
				short new_x = orig_x + i;
				short new_y = orig_y + j;
				if (new_x < 0  ||  new_x >= this->m_w
				 || new_y < 0  ||  new_y >= this->m_h )
					continue;
				this->m_bitmap[new_y*this->m_w + new_x] = color;
			}
		}
	}
};