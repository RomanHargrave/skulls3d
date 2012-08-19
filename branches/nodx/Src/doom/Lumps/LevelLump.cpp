
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
#include "../Wad.h"
#include "../Node.h"
#include "..\LumpDictionary.h"
#include "..\Patches.h"
#include "PlayPalLump.h"

namespace skulls
{
	Level::Level(File & file,
	             LumpDictionary & dictionary,
	             Patches & patches,
	             std::map<std::string,std::shared_ptr<Texture>> & textures,
	             Lump & lump,
	             Palettes & palettes)
	{
		int oldPos = file.GetPos();

		Lump * things = dictionary.Get("THINGS");
		int count = things->m_size / 10;
		m_things.reserve(count);
		for (int j=0 ; j<count ; j++)
		{
			file.MoveTo(things->m_position + 10*j);
			m_things.push_back(Thing(file, patches));
		}

		Lump * vertexes = dictionary.Get("VERTEXES");
		count = vertexes->m_size / 4;
		m_vertexes.reserve(count);
		file.MoveTo(vertexes->m_position);
		for (int j=0 ; j<count ; j++)
		{
			short x,y;
			x = file.ReadInt2();
			y = file.ReadInt2();
			m_vertexes.push_back(std::make_shared<Vertex>(x, y));
			printf("Vertex %d at %d, %d\n", j, x, y);
		}
		
		Lump * sectors = dictionary.Get("SECTORS");
		count = sectors->m_size / 26;
		m_sectors.reserve(count);
		for (int j=0 ; j<count ; j++)
		{
			file.MoveTo(sectors->m_position + 26*j);
			m_sectors.push_back(std::make_shared<Sector>(file, dictionary, palettes));
		}
		
		Lump * sidedefs = dictionary.Get("SIDEDEFS");
		count = sidedefs->m_size / 30;
		m_sideDefs.reserve(count);
		for (int j=0 ; j<count ; j++)
		{
			file.MoveTo(sidedefs->m_position + 30*j);
			auto sidedef = std::make_shared<SideDef>(file, *this, textures);
			sidedef->Resolve();
			m_sideDefs.push_back(sidedef);
		}

		Lump * linedefs = dictionary.Get("LINEDEFS");
		count = linedefs->m_size / 14;
		m_lineDefs.reserve(count);
		for (int j=0 ; j<count ; j++)
		{
			file.MoveTo(linedefs->m_position + 14*j);
			auto linedef = std::make_shared<LineDef>(file, *this);
			linedef->Resolve();
			m_lineDefs.push_back(linedef);
			printf("linedef %d is vertex (%d,%d) and (%d,%d)\n", j,
			       m_lineDefs[j]->m_start_vtx->m_x, m_lineDefs[j]->m_start_vtx->m_z,
			       m_lineDefs[j]->m_end_vtx->m_x,   m_lineDefs[j]->m_end_vtx->m_z);
		}

		Lump * segs = dictionary.Get("SEGS");
		count = segs->m_size / 12;
		m_segs.reserve(count);
		file.MoveTo(segs->m_position);
		for (int j=0 ; j<count ; j++)
		{
			unsigned short v1, v2;
			v1 = file.ReadInt2();
			v2 = file.ReadInt2();
			printf("Seg %d is v %d and %d, angle %X, linedef %d, dir %d, offset %d\n",
				j, v1, v2, file.ReadInt2(), file.ReadInt2(), file.ReadInt2(), file.ReadInt2());
			//int aligned = (m_vertexes[dummy2]->m_x-m_vertexes[dummy1]->m_x) * (m_lineDefs[dummy4]->m_end_vtx->m_x-m_lineDefs[dummy4]->m_start_vtx->m_x)
            //            + (m_vertexes[dummy2]->m_z-m_vertexes[dummy1]->m_z) * (m_lineDefs[dummy4]->m_end_vtx->m_z-m_lineDefs[dummy4]->m_start_vtx->m_z);
			//if ((dummy5 == 0 && aligned) || (dummy5 == 1 && !aligned)) // right
				m_segs.push_back(std::make_shared<Seg>(m_vertexes[v1], m_vertexes[v2]));
			//else // left
			//	m_segs[j] = new Seg(m_vertexes[dummy2], m_vertexes[dummy1]);
		}

		Lump * ssectors = dictionary.Get("SSECTORS");
		count = ssectors->m_size / 4;
		printf("%d SSectors\n", count);
		m_ssectors.reserve(count);
		//m_addssectors.reserve(count);
		file.MoveTo(ssectors->m_position);
		for (int j=0 ; j<count ; j++)
		{
			unsigned short first, size;
			size  = file.ReadInt2();
			first = file.ReadInt2();
			m_ssectors.push_back(std::make_shared<SSector>(j));
			//m_addssectors[j] = new SSector();
			for (unsigned short k=first ; k<first+size ; k++)
				m_ssectors[j]->m_segs.push_back(m_segs[k]);
		}
		
		Lump * nodes = dictionary.Get("NODES");
		count = nodes->m_size/28;
		m_bspTree = new Node(file, *this, nodes->m_position, count-1, NULL);
		m_bspTree->BuildMissingSegs();
		//count = m_bspTree->Count();
		/*
		count = l->m_size / 28;
		m_Wad->MoveTo(l->m_position);
		for (int j=0 ; j<count ; j++)
		{
			short x1, z1, x2, z2;
			unsigned short rightChild, leftChild;
			m_Wad->ReadInt2(&x1);
			m_Wad->ReadInt2(&z1);
			m_Wad->ReadInt2(&x2);
			m_Wad->ReadInt2(&z2);
			x2 += x1;
			z2 += z1;
			m_Wad->Skip(16);
			m_Wad->ReadInt2((short*)&rightChild);
			m_Wad->ReadInt2((short*)&leftChild);
			if (rightChild & 0x8000)
			{
				skulls::Seg * seg = new Seg(new Vertex(this, x1, z1), new Vertex(this, x2, z2), false);
				m_addssectors[rightChild&0x7FFF]->m_segs.push_back(seg);
			}
			if (leftChild & 0x8000)
			{
				skulls::Seg * seg = new Seg(new Vertex(this, x2, z2), new Vertex(this, x1, z1), false);
				m_addssectors[leftChild&0x7FFF]->m_segs.push_back(seg);
			}
			if ( (!(rightChild & 0x8000)) && (!(leftChild & 0x8000)) )
			{
				m_dividers.push_back(new Seg(new Vertex(this, x1, z1), new Vertex(this, x2, z2), false));
			}
		}
		*/

		/*
		i++;
		SetSegsLump(SegsLump::Get(m_Wad->m_lumps[i]));
		// SSECTORS
		i++;
		SetSSectorsLump(SSectorsLump::Get(m_Wad->m_lumps[i]));
		// NODES
		i++;
		SetNodesLump(NodesLump::Get(m_Wad->m_lumps[i]));
		// SECTORS
		i++;
		SetSectorsLump(SectorsLump::Get(m_Wad->m_lumps[i]));
		// REJECT
		i++;
		SetRejectLump(RejectLump::Get(m_Wad->m_lumps[i]));
		// BLOCKMAP
		i++;
		SetBlockMapLump(BlockMapLump::Get(m_Wad->m_lumps[i]));
		*/
	}

	std::shared_ptr<Sector> Level::GetSectorByTag(unsigned short tag)
	{
		for (auto it=m_sectors.begin(),end=m_sectors.end() ; it!=end ; ++it)
		{
			if ((*it)->m_tag == tag)
				return *it;
		}
		return nullptr;
	}
};
