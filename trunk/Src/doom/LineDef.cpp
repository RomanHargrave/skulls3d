
#include <stdlib.h>
#include "LineDef.h"
#include "SideDef.h"
#include "WadFile.h"
#include "Vertex.h"
#include "lumps/LevelLump.h"

namespace doom
{
	LineDef::LineDef(WadFile * wadFile, LevelLump *level)
	{
		unsigned short dummy;
		wadFile->ReadInt2((short*)&dummy);
		m_start_vtx = level->m_vertexes[dummy];
		m_start_vtx->m_lineDefs.push_back(this);
		wadFile->ReadInt2((short*)&dummy);
		m_end_vtx = level->m_vertexes[dummy];

		// Flags
		wadFile->ReadInt2((short*)&dummy);
		m_block_p_m = ((dummy & 1) == 1);
		m_2_sided = ((dummy & 2) == 2);
		m_upper_unpegged = ((dummy & 4) == 4);
		m_lower_unpegged = ((dummy & 8) == 8);
		m_secret = ((dummy & 16) == 16);
		m_block_sound = ((dummy & 32) == 32);
		m_never_automap = ((dummy & 64) == 64);
		m_always_automap = ((dummy & 128) == 128);

		// Sector Type
		wadFile->ReadInt2((short*)&m_type);

		// Sector Tag
		wadFile->ReadInt2((short*)&dummy);
		m_tagSector = level->GetSectorByTag(dummy);

		// Right Sidedefs
		wadFile->ReadInt2((short*)&dummy);
		if (dummy != 0xFFFF)
		{
			m_rightSideDef = level->m_sideDefs[dummy];
			m_rightSideDef->m_lineDef = this;
		}
		else
		{
			m_rightSideDef = NULL;
		}
		// Left Sidedefs
		wadFile->ReadInt2((short*)&dummy);
		if (dummy != 0xFFFF)
		{
			m_leftSideDef = level->m_sideDefs[dummy];
			m_leftSideDef->m_lineDef = this;
		}
		else
		{
			m_leftSideDef = NULL;
		}
	}
};
