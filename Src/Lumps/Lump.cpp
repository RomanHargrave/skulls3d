
#include <stdlib.h>
#include "Lump.h"
#include "../WadFile.h"

namespace Doom
{
	Lump::Lump(WadFile * wadfile, char * name, int pos, int size)
	{
		this->m_wadfile = wadfile;
		this->m_name = _strdup(name);
		this->m_position = pos;
		this->m_size = size;
		this->m_data = NULL;
	}

	Lump::~Lump()
	{
		UnLoad();
		delete m_name;
	}

	void Lump::Load()
	{
		m_data = new char[m_size];
		// TODO: read from file
	}

	void Lump::UnLoad()
	{
		delete m_data;
		m_data = NULL;
	}
}