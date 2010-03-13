
#include <stdlib.h>
#include "LineDef.h"
#include "WadFile.h"

namespace doom
{
	LineDef::LineDef(WadFile * wadFile, VertexesLump* vertexLump, int offset)
	{
		m_vertexes = (VertexesLump*) vertexLump;

		wadFile->MoveTo(offset);

		short pos;
		wadFile->ReadInt2(&pos);
		m_start_vtx = (Vertex*) m_vertexes->Get(pos);
		wadFile->ReadInt2(&pos);
		m_end_vtx = (Vertex*)  m_vertexes->Get(pos);

		short flags;
		wadFile->ReadInt2(&flags);

		m_block_p_m = ((flags & 1) == 1);
		m_2_sided = ((flags & 2) == 2);
		m_upper_unpegged = ((flags & 4) == 4);
		m_lower_unpegged = ((flags & 8) == 8);
		m_secret = ((flags & 16) == 16);
		m_block_sound = ((flags & 32) == 32);
		m_never_automap = ((flags & 64) == 64);
		m_always_automap = ((flags & 128) == 128);

	}
	
	int LineDef::Load()
	{
		return 0;
	}
	void LineDef::UnLoad()
	{
	}
};
