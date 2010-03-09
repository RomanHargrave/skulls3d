
#include "LineDefsLump.h"
#include "LevelLump.h"
#include "Lump.h"
#include "../LineDef.h"
#include "../WadFile.h"

namespace doom
{
	LineDefsLump::LineDefsLump(LevelLump * levelLump) :Lump(levelLump)
	{
		m_level = levelLump;	
	}
	LineDefsLump::~LineDefsLump()
	{
	}

	LineDef* LineDefsLump::operator[](int index) const
	{
		return m_linedefs[index];
	}

	int LineDefsLump::Load()
	{
		if (m_linedefs.size() != 0)
			return 0; //Already loaded

		// Each Thing has 14 bytes of data
		int count = m_size/14;
		m_linedefs.resize(count);

		printf("%d LineDefs\n", count);

		for (int i=0 ; i<count ; i++)
		{
			m_linedefs[i] = new LineDef(m_wadfile, (VertexesLump*) m_level->m_vertexes, (m_position+14*i) );
		}

		return 0;
	}
	void LineDefsLump::UnLoad()
	{
		Lump::UnLoad();

		m_linedefs.clear();
	}
};