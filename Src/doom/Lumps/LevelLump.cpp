
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "LevelLump.h"
#include "../Vertex.h"
#include "../SideDef.h"
#include "../LineDef.h"
#include "../Thing.h"
#include "../Seg.h"
#include "../Sector.h"
#include "../SSector.h"
#include "../WadFile.h"
#include "../Node.h"

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
			printf("Vertex %d at %d, %d\n", j, x, y);
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
			printf("linedef %d is vertex (%d,%d) and (%d,%d)\n", j,
				m_lineDefs[j]->m_start_vtx->m_x, m_lineDefs[j]->m_start_vtx->m_z,
				m_lineDefs[j]->m_end_vtx->m_x,   m_lineDefs[j]->m_end_vtx->m_z);
		}

		// SEGS
		l = m_wadfile->Get(i+5);
		count = l->m_size / 12;
		m_segs.resize(count);
		m_wadfile->MoveTo(l->m_position);
		for (int j=0 ; j<count ; j++)
		{
			unsigned short dummy1, dummy2, dummy3, dummy4, dummy5, dummy6;
			m_wadfile->ReadInt2((short*)&dummy1);
			m_wadfile->ReadInt2((short*)&dummy2);
			m_wadfile->ReadInt2((short*)&dummy3);
			m_wadfile->ReadInt2((short*)&dummy4);
			m_wadfile->ReadInt2((short*)&dummy5);
			m_wadfile->ReadInt2((short*)&dummy6);
			printf("Seg %d is v %d and %d, angle %X, linedef %d, dir %d, offset %d\n",
				j, dummy1, dummy2, dummy3, dummy4, dummy5, dummy6);
			//int aligned = (m_vertexes[dummy2]->m_x-m_vertexes[dummy1]->m_x) * (m_lineDefs[dummy4]->m_end_vtx->m_x-m_lineDefs[dummy4]->m_start_vtx->m_x)
            //            + (m_vertexes[dummy2]->m_z-m_vertexes[dummy1]->m_z) * (m_lineDefs[dummy4]->m_end_vtx->m_z-m_lineDefs[dummy4]->m_start_vtx->m_z);
			//if ((dummy5 == 0 && aligned) || (dummy5 == 1 && !aligned)) // right
				m_segs[j] = new Seg(m_vertexes[dummy1], m_vertexes[dummy2]);
			//else // left
			//	m_segs[j] = new Seg(m_vertexes[dummy2], m_vertexes[dummy1]);
		}

		// SSECTORS
		l = m_wadfile->Get(i+6);
		count = l->m_size / 4;
		printf("%d SSectors\n", count);
		m_ssectors.resize(count);
		//m_addssectors.resize(count);
		m_wadfile->MoveTo(l->m_position);
		for (int j=0 ; j<count ; j++)
		{
			unsigned short first, size;
			m_wadfile->ReadInt2((short*)&size);
			m_wadfile->ReadInt2((short*)&first);
			m_ssectors[j] = new SSector(j);
			//m_addssectors[j] = new SSector();
			for (unsigned short k=first ; k<first+size ; k++)
				m_ssectors[j]->m_segs.push_back(m_segs[k]);
		}
		
		// NODES
		l = m_wadfile->Get(i+7);
		count = l->m_size/28;
		m_bspTree = new Node(this, l->m_position, count-1, NULL);
		count = m_bspTree->Count();
		/*
		count = l->m_size / 28;
		m_wadfile->MoveTo(l->m_position);
		for (int j=0 ; j<count ; j++)
		{
			short x1, z1, x2, z2;
			unsigned short rightChild, leftChild;
			m_wadfile->ReadInt2(&x1);
			m_wadfile->ReadInt2(&z1);
			m_wadfile->ReadInt2(&x2);
			m_wadfile->ReadInt2(&z2);
			x2 += x1;
			z2 += z1;
			m_wadfile->Skip(16);
			m_wadfile->ReadInt2((short*)&rightChild);
			m_wadfile->ReadInt2((short*)&leftChild);
			if (rightChild & 0x8000)
			{
				doom::Seg * seg = new Seg(new Vertex(x1, z1), new Vertex(x2, z2), false);
				m_addssectors[rightChild&0x7FFF]->m_segs.push_back(seg);
			}
			if (leftChild & 0x8000)
			{
				doom::Seg * seg = new Seg(new Vertex(x2, z2), new Vertex(x1, z1), false);
				m_addssectors[leftChild&0x7FFF]->m_segs.push_back(seg);
			}
			if ( (!(rightChild & 0x8000)) && (!(leftChild & 0x8000)) )
			{
				m_dividers.push_back(new Seg(new Vertex(x1, z1), new Vertex(x2, z2), false));
			}
		}
		*/

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
