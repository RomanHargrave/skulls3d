
#include <stdlib.h>
#include "Vertex.h"
#include "WadFile.h"

namespace doom
{
	Vertex::Vertex(WadFile * wadFile, int offset)
	{
		wadFile->MoveTo(offset);
		wadFile->ReadInt2(&m_x);
		wadFile->ReadInt2(&m_y);		
	}
};
