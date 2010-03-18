
#include <memory.h>
#include "SideDef.h"
#include "Sector.h"
#include "WadFile.h"
#include "Texture.h"
#include "lumps/LevelLump.h"

namespace doom
{
	SideDef::SideDef(WadFile *wadFile, LevelLump *level)
	{
		wadFile->ReadInt2((short*)&m_xoffset);
		wadFile->ReadInt2((short*)&m_yoffset);

		char texName[9];

		memset(texName, 0, 9);
		wadFile->ReadString(texName, 8);
		m_upperTexture = wadFile->GetTexture(texName);
		if (m_upperTexture != NULL)
		{
			int pos = wadFile->GetPos();
			m_upperTexture->Load();
			wadFile->MoveTo(pos);
		}

		memset(texName, 0, 9);
		wadFile->ReadString(texName, 8);
		m_lowerTexture = wadFile->GetTexture(texName);
		if (m_lowerTexture != NULL)
		{
			int pos = wadFile->GetPos();
			m_lowerTexture->Load();
			wadFile->MoveTo(pos);
		}

		memset(texName, 0, 9);
		wadFile->ReadString(texName, 8);
		m_middleTexture = wadFile->GetTexture(texName);
		if (m_middleTexture != NULL)
		{
			int pos = wadFile->GetPos();
			m_middleTexture->Load();
			wadFile->MoveTo(pos);
		}

		unsigned short sectorId;
		wadFile->ReadInt2((short*)&sectorId);
		m_sector = level->m_sectors[sectorId];
		m_sector->m_sideDefs.push_back(this);
	}
};
