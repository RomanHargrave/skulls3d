
#include <stdlib.h>
#include "Vertex.h"
#include "SideDef.h"
#include "WadFile.h"
#include "ReferencedObject.h"

namespace doom
{
	Vertex::Vertex(void *referencer, int x, int z)
		:ReferencedObject(referencer)
	{
		m_x = x;
		m_z = z;
	}

	bool Vertex::Load()
	{
		return true;
	}
	void Vertex::UnLoad()
	{
	}
};
