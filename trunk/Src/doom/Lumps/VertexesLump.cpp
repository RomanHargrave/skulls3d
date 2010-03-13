
#include "VertexesLump.h"
#include "../Vertex.h"
#include "../WadFile.h"

namespace doom
{
	VertexesLump::VertexesLump(Lump * lump)
		:Lump(lump)
	{
	}
	Vertex* VertexesLump::Get(int index)
	{
		return m_vertexes[index];
	}

	int VertexesLump::Load()
	{
		if (m_vertexes.size() != 0)
			return 0; //Already loaded

		// Each Vertex has 4 bytes of data
		int count = m_size/4;
		m_vertexes.resize(count);

		//printf("%d Vertexes\n", count);

		for (int i=0 ; i<count ; i++)
		{
			m_vertexes[i] = new Vertex(m_wadfile, m_position+4*i);
		}
		size = count;
		return 0;
	}
	void VertexesLump::UnLoad()
	{
		Lump::UnLoad();

		m_vertexes.clear();
	}
};