
#include <stdlib.h>
#include "Texture.h"
#include "Wad.h"
#include "lumps/PatchLump.h"
#include "..\File.h"
#include "Patches.h"

namespace skulls
{

	Texture::Texture(File & file, int position, Patches & patches)
		:m_bitmap(nullptr)
	{
		// Load name
		int pos = file.GetPos();
		file.MoveTo(position);

		name = file.ReadString(8);

		file.Skip(4); // skip flags

		file.ReadInt2((short*)&m_w);
		file.ReadInt2((short*)&m_h);

		//Skipping columndirectory
		file.Skip(4);

		short patchLump_count;
		file.ReadInt2(&patchLump_count);

		m_bitmap = new unsigned int[m_w*m_h];

		// Read all PatchLumpes and merge them into m_bitmap
		for (int i=0 ; i<patchLump_count ; i++)
		{
			short orig_x, orig_y;
			file.ReadInt2(&orig_x);
			file.ReadInt2(&orig_y);
			
			short patchLump_index;
			file.ReadInt2((short*)&patchLump_index);

			short stepdir, colormap;
			file.ReadInt2(&stepdir);
			file.ReadInt2(&colormap);

			// TODO: We can save memory when there is only one PatchLump
			// and its orig_x and orig_y are 0. In this case this texture
			// is exactly the same as the PatchLump, so m_bitmap may point
			// directly at the PatchLump's bitmap
			// But beware of the UnLoad, a texture shall not free a PatchLump's bitmap

			if (patchLump_index < 0 || patchLump_index > (short)patches.Size())
				continue;

			int file_pos = file.GetPos();
			std::shared_ptr<Patch> patch = patches.Get(patchLump_index);

			//printf("\t%s\n", PatchLump->m_name);

			MergePatchLumpIntoTexture(patch, orig_x, orig_y);
			file.MoveTo(file_pos);
		}
	}
	
	Texture::~Texture()
	{
		if (m_bitmap != nullptr)
			delete m_bitmap;
	}
	
	void Texture::MergePatchLumpIntoTexture(std::shared_ptr<Patch> & patch, short orig_x, short orig_y)
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