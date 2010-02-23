
#include <stdlib.h>
#include <stdio.h>
#include "LevelLump.h"
#include "ThingsLump.h"
#include "..\WadFile.h"

namespace doom
{
	LevelLump * LevelLump::Get(Lump * lump)
	{
		LevelLump * result = dynamic_cast<LevelLump*>(lump);
		if (result == NULL)
		{
			// Lump has not been transformed yet, transform it now
			result = new LevelLump(lump);
			// Update the dictionary reference and dispose of the old lump
			result->m_wadfile->SetLump(result);
		}	
		printf("New level found : %s\n", result->m_name);
		return result;
	}

	LevelLump::LevelLump(Lump * lump)
		:Lump(lump)
	{
	}

	int LevelLump::Load()
	{
		int i = m_dictionary_position;
		
		// THINGS
		i++;
		//SetThingsLump(ThingsLump::Get(m_wadfile->GetLump[i]));
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
};
