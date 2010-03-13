
#include <stdlib.h>
#include "Texture.h"
#include "WadFile.h"
#include "lumps/PatchesLump.h"
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

		short patch_count;
		m_wadfile->ReadInt2(&patch_count);

		m_bitmap = new unsigned int[m_w*m_h];

		// Read all patches and merge them into m_bitmap
		for (int i=0 ; i<patch_count ; i++)
		{
			short orig_x, orig_y;
			m_wadfile->ReadInt2(&orig_x);
			m_wadfile->ReadInt2(&orig_y);
			
			short patch_index;
			m_wadfile->ReadInt2(&patch_index);

			short stepdir, colormap;
			m_wadfile->ReadInt2(&stepdir);
			m_wadfile->ReadInt2(&colormap);

			// TODO: We can save memory when there is only one patch
			// and its orig_x and orig_y are 0. In this case this texture
			// is exactly the same as the patch, so m_bitmap may point
			// directly at the patch's bitmap
			// But beware of the UnLoad, a texture shall not free a patch's bitmap

			int file_pos = m_wadfile->GetPos();
			PatchLump * patch = (*m_wadfile->m_patches)[patch_index];
			if (patch == NULL)
				continue;
			patch->Load();
			m_wadfile->MoveTo(file_pos);

			//printf("\t%s\n", patch->m_name);

			MergePatchIntoTexture(patch, orig_x, orig_y);
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
	
	void Texture::MergePatchIntoTexture(PatchLump * patch, short orig_x, short orig_y)
	{
		for (int j=0 ; j<patch->m_h ; j++)
		{
			for (int i=0 ; i<patch->m_w ; i++)
			{
				int color = patch->m_bitmap[j*patch->m_w + i];
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