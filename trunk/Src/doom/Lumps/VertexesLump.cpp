
#include "VertexesLump.h"
#include "../Vertex.h"
#include "../WadFile.h"

namespace doom
{
	VertexesLump::VertexesLump(Lump * lump)
		:Lump(lump)
	{
	}
	const Vertex& VertexesLump::Get(int index) const
	{
		return m_vertexes[index];
	}

	int VertexesLump::Load()
	{
		if (m_vertexes.size() != 0)
			return 0; //Already loaded

		m_wadfile->MoveTo(m_position);

		// Each Vertex has 4 bytes of data
		int count = m_size/4;
		m_vertexes.resize(count);

		for (int i=0 ; i<count ; i++)
		{
			m_wadfile->ReadInt2(&m_vertexes[i].m_x);
			m_wadfile->ReadInt2(&m_vertexes[i].m_y);
		}
		return 0;
	}
	void VertexesLump::UnLoad()
	{
		Lump::UnLoad();

		m_vertexes.clear();
	}
};