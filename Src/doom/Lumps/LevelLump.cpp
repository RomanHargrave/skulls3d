
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "LevelLump.h"
#include "../Vertex.h"
#include "../SideDef.h"
#include "../LineDef.h"
#include "../Thing.h"
#include "../Sector.h"
#include "../WadFile.h"

namespace doom
{
	LevelLump::LevelLump(Lump * lump)
		:Lump(lump)
	{
		m_things.resize(0);
		m_sectors.resize(0);
	}

	int LevelLump::Load()
	{
		if (m_things.size() != 0)
			return 0; // Already loaded

		int i = m_dictionary_position;

		// THINGS
		Lump *l = m_wadfile->Get(i+1);
		int count = l->m_size / 10;
		m_things.resize(count);
		for (int j=0 ; j<count ; j++)
		{
			m_wadfile->MoveTo(l->m_position + 10*j);
			m_things[j] = new Thing(m_wadfile);
		}

		// VERTEXES
		*l = m_wadfile->Get(i+4);
		count = l->m_size / 4;
		m_vertexes.resize(count);
		m_wadfile->MoveTo(l->m_position);
		for (int j=0 ; j<count ; j++)
		{
			short x,y;
			m_wadfile->ReadInt2(&x);
			m_wadfile->ReadInt2(&y);
			m_vertexes[j] = new Vertex(x, y);
		}
		
		// SECTORS
		*l = m_wadfile->Get(i+8);
		count = l->m_size / 26;
		m_sectors.resize(count);
		for (int j=0 ; j<count ; j++)
		{
			m_wadfile->MoveTo(l->m_position + 26*j);
			m_sectors[j] = new Sector(m_wadfile);
		}
		
		// SIDEDEFS
		l = m_wadfile->Get(i+3);
		count = l->m_size / 30;
		m_sideDefs.resize(count);
		for (int j=0 ; j<count ; j++)
		{
			m_wadfile->MoveTo(l->m_position + 30*j);
			m_sideDefs[j] = new SideDef(m_wadfile, this);
		}

		// LINEDEFS
		l = m_wadfile->Get(i+2);
		count = l->m_size / 14;
		m_lineDefs.resize(count);
		for (int j=0 ; j<count ; j++)
		{
			m_wadfile->MoveTo(l->m_position + 14*j);
			m_lineDefs[j] = new LineDef(m_wadfile, this);
		}

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
	}

	Sector* LevelLump::GetSectorByTag(unsigned short tag)
	{
		for (std::vector<Sector*>::iterator it=m_sectors.begin() ; it!=m_sectors.end() ; ++it)
		{
			if (((Sector*)*it)->m_tag == tag)
				return (Sector*)*it;
		}
		return NULL;
	}
};
