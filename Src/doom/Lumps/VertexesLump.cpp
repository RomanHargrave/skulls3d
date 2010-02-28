
#include "VertexesLump.h"
#include "../Vertex.h"
#include "../WadFile.h"

namespace doom
{
	VertexesLump::VertexesLump(Lump * lump)
		:Lump(lump)
	{
	}
	Vertex* VertexesLump::operator[](int index) const
	{
		return m_vertexes[index];
	}

	int VertexesLump::Load()
	{
		// Each Vertex has 4 bytes of data
		int count = m_size/4;
		m_vertexes.resize(count);

		printf("%d Vertexes\n", count);

		for (int i=0 ; i<count ; i++)
		{
			m_vertexes[i] = new Vertex(m_wadfile, m_position+4*i);
		}

		return 0;
	}
	void VertexesLump::UnLoad()
	{
	}
};