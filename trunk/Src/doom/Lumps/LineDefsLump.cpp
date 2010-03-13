
#include "LineDefsLump.h"
#include "LevelLump.h"
#include "Lump.h"
#include "../LineDef.h"
#include "../WadFile.h"

namespace doom
{
	LineDefsLump::LineDefsLump(Lump * Lump) :Lump(Lump)
	{		
	}
	LineDefsLump::~LineDefsLump()
	{
	}

	LineDef* LineDefsLump::Get(int index)
	{
		return m_linedefs[index];
	}

	int LineDefsLump::Load(LevelLump* levelLump)
	{
		m_level = levelLump;	

		if (m_linedefs.size() != 0)
			return 0; //Already loaded

		// Each Thing has 14 bytes of data
		int count = m_size/14;
		m_linedefs.resize(count);

		//printf("%d LineDefs\n", count);

		for (int i=0 ; i<count ; i++)
		{
			m_linedefs[i] = new LineDef(m_wadfile, (VertexesLump*) m_level->m_vertexes, (m_position+14*i) );
		}
		size = count;
		return 0;
	}
	void LineDefsLump::UnLoad()
	{
		Lump::UnLoad();

		m_linedefs.clear();
	}
};