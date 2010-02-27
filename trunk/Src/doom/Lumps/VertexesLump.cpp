
#include "VertexesLump.h"
#include "../Vertexes.h"
#include "../WadFile.h"

namespace doom
{
	VertexesLump::VertexesLump(Lump * lump)
		:Lump(lump)
	{
	}
	Vertexes* VertexesLump::operator[](int index) const
	{
		// TODO : maybe index won't be global in m_wadfile->m_lumps
		// but local to this THINGS collection
		return m_vertexes[index];
	}

	int VertexesLump::Load()
	{
		// Each ThingLump has 10 bytes of data
		int count = m_size/4;
		m_vertexes.resize(count);

		printf("%d Vertexes\n", count);

		for (int i=0 ; i<count ; i++)
		{
			m_vertexes[i] = new Vertexes(m_wadfile, m_position+4*i);
		}

		return 0;
	}
	void VertexesLump::UnLoad()
	{
	}
};