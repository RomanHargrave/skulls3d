
#include <stdlib.h>
#include "Lump.h"
#include "../WadFile.h"

namespace doom
{
	Lump::Lump(WadFile * wadfile, char * name, int pos, int size)
	{
		this->m_wadfile = wadfile;
		this->m_name = _strdup(name);
		this->m_position = pos;
		this->m_size = size;
	}

	Lump::~Lump()
	{
		delete m_name;
	}
}