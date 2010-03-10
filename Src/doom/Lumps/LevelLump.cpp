
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
		if (m_things != NULL)
			return 0; // Already loaded

		int i = m_dictionary_position;

		// THINGS
		i++;
		//SetThingsLump(ThingsLump::Get(m_wadfile->GetLump[i]));
		m_things = m_wadfile->GetLump((ThingsLump*)m_wadfile->Get(i));
		if (m_things == NULL)
			return 1;
		if (m_things->Load() != 0)
			return 2;

		//*
		// LINEDEFS
		i++;
		short linedefsIndex = i;
		//SetLineDefsLump(LineDefsLump::Get(m_wadfile->m_lumps[i]));
		// SIDEDEFS
		i++;
		//SetSideDefsLump(SideDefsLump::Get(m_wadfile->m_lumps[i]));
		// VERTEXES
		i++;
		//SetVertexesLump(VertexesLump::Get(m_wadfile->m_lumps[i]));
		m_vertexes = m_wadfile->GetLump((VertexesLump*)m_wadfile->Get(i));
		if (m_vertexes == NULL)
			return 1;
		if (m_vertexes->Load() != 0)
			return 2;
		
		//*
		// LINEDEFS
		m_linedefs = m_wadfile->GetLump((LineDefsLump*)m_wadfile->Get(linedefsIndex));
		if (m_linedefs == NULL)
			return 1;
		if (m_linedefs->Load(this) != 0)
			return 2;		
		
		// SEGS
		/*
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
		Lump::UnLoad();

		if (m_things != NULL)
		{
			delete m_things;
			m_things = NULL;
		}
		if (m_vertexes != NULL)
		{
			delete m_vertexes;
			m_vertexes = NULL;
		}
	}
};
