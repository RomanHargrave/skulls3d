
#include <stdlib.h>
#include <stdio.h>
#include "LevelLump.h"
#include "ThingsLump.h"
#include "..\WadFile.h"

namespace doom
{
	LevelLump::LevelLump(Lump * lump)
		:Lump(lump)
	{
		m_things = NULL;
	}

	int LevelLump::Load()
	{
		int i = m_dictionary_position;
		
		// THINGS
		i++;
		//SetThingsLump(ThingsLump::Get(m_wadfile->GetLump[i]));
		m_things = m_wadfile->GetLump((ThingsLump*)m_wadfile->GetLump(i));
		/*
		// LINEDEFS
		i++;
		SetLineDefsLump(LineDefsLump::Get(m_wadfile->m_lumps[i]));
		// SIDEDEFS
		i++;
		SetSideDefsLump(SideDefsLump::Get(m_wadfile->m_lumps[i]));
		// VERTEXES
		i++;
		SetVertexesLump(VertexesLump::Get(m_wadfile->m_lumps[i]));
		// SEGS
		i++;
		SetSegsLump(SegsLump::Get(m_wadfile->m_lumps[i]));
		// SSECTORS
		i++;
		SetSSectorsLump(SSectorsLump::Get(m_wadfile->m_lumps[i]));
		// NODES
		i++;
		SetNodesLump(NodesLump::Get(m_wadfile->m_lumps[i]));
		// SECTORS
		i++;
		SetSectorsLump(SectorsLump::Get(m_wadfile->m_lumps[i]));
		// REJECT
		i++;
		SetRejectLump(RejectLump::Get(m_wadfile->m_lumps[i]));
		// BLOCKMAP
		i++;
		SetBlockMapLump(BlockMapLump::Get(m_wadfile->m_lumps[i]));
		*/
		return 0;
	}
	void LevelLump::UnLoad()
	{
	}

	char* LevelLump::ToString()
	{
		static char s[128] = {'\0'};
		sprintf_s(s, "Level %s", m_name);
		return s;
	}
};
