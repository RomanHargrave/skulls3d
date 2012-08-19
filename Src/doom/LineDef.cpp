
#include <stdlib.h>
#include "LineDef.h"
#include "SideDef.h"
#include "Wad.h"
#include "Vertex.h"
#include "lumps/LevelLump.h"
#include "..\File.h"

namespace skulls
{
	LineDef::LineDef(File & file, Level & level)
	{
		m_start_vtx = level.m_vertexes[file.ReadInt2()];
		m_end_vtx = level.m_vertexes[file.ReadInt2()];

		// Flags
		short flags = file.ReadInt2();
		m_block_p_m = ((flags & 1) == 1);
		m_2_sided = ((flags & 2) == 2);
		m_upper_unpegged = ((flags & 4) == 4);
		m_lower_unpegged = ((flags & 8) == 8);
		m_secret = ((flags & 16) == 16);
		m_block_sound = ((flags & 32) == 32);
		m_never_automap = ((flags & 64) == 64);
		m_always_automap = ((flags & 128) == 128);

		// Sector Type
		m_type = file.ReadInt2();

		// Sector Tag
		m_tagSector = level.GetSectorByTag(file.ReadInt2());

		// Right Sidedefs
		short rightSidedef = file.ReadInt2();
		if (rightSidedef != 0xFFFF)
		{
			m_rightSideDef = level.m_sideDefs[rightSidedef];
		}
		else
		{
			m_rightSideDef = nullptr;
		}
		// Left Sidedefs
		short leftSidedef = file.ReadInt2();
		if (leftSidedef != -1)
		{
			m_leftSideDef = level.m_sideDefs[leftSidedef];
		}
		else
		{
			m_leftSideDef = nullptr;
		}
	}

	void LineDef::Resolve()
	{
		m_start_vtx->m_lineDefs.push_back(shared_from_this());
		if (m_rightSideDef)
			m_rightSideDef->m_lineDef = shared_from_this();
		if (m_leftSideDef)
			m_leftSideDef->m_lineDef = shared_from_this();
	}
};
