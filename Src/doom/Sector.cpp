
#include <memory.h>
#include "Sector.h"
#include "WadFile.h"
#include "lumps/FlatLump.h"
#include "lumps/Lump.h"

namespace doom
{
	Sector::Sector(WadFile *wadFile)
	{
		wadFile->ReadInt2(&m_floorHeight);
		wadFile->ReadInt2(&m_ceilingHeight);

		char texName[9];
		
		memset(texName, 0, 9);
		wadFile->ReadString(texName, 8);
		m_floorTexture = wadFile->GetLump((FlatLump*)wadFile->Get(texName));
		if (m_floorTexture != NULL)
			m_floorTexture->Load();

		memset(texName, 0, 9);
		wadFile->ReadString(texName, 8);
		m_ceilingTexture = wadFile->GetLump((FlatLump*)wadFile->Get(texName));
		if (m_ceilingTexture != NULL)
			m_ceilingTexture->Load();

		wadFile->ReadInt2((short*)&m_lightLevel);
		wadFile->ReadInt2((short*)&m_type);
		wadFile->ReadInt2((short*)&m_tag);
	}
};
