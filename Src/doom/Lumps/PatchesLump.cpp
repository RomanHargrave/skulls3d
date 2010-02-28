
#include "PatchesLump.h"
#include "PatchLump.h"
#include "PlayPalLump.h"
#include "../WadFile.h"

namespace doom
{
	PatchesLump::PatchesLump(Lump * lump)
		:Lump(lump)
	{
	}
	PatchesLump::~PatchesLump()
	{
	}

	PatchLump* PatchesLump::operator[](int index) const
	{
		return m_patches[index];
	}

	int PatchesLump::Load(PlayPalLump * palettes)
	{
		// Each PatchLump has 10 bytes of data
		int count;
		m_wadfile->MoveTo(m_position);
		m_wadfile->ReadInt4(&count);
		
		m_patches.resize(count);

		printf("%d Patches\n", count);

		char name[9];
		name[8] = 0;
		for (int i=0 ; i<count ; i++)
		{
			m_wadfile->ReadString(name, 8);
			m_patches[i] = m_wadfile->GetLump((PatchLump*)m_wadfile->Get(name));
		}
		for (int i=0 ; i<count ; i++)
		{
			if (m_patches[i] != NULL)
				m_patches[i]->Load(palettes);
		}

		return 0;
	}
	void PatchesLump::UnLoad()
	{
	}
};