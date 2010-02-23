
#include <stdlib.h>
#include "Lump.h"
#include "../WadFile.h"

namespace doom
{
	Lump::Lump(WadFile * wadfile, int dictionary_position, char * name, int pos, int size)
	{
		this->m_wadfile = wadfile;
		this->m_dictionary_position = dictionary_position;
		this->m_name = _strdup(name);
		this->m_position = pos;
		this->m_size = size;
		this->m_data = NULL;
	}

	Lump::Lump(Lump * other)
	{
		this->m_wadfile = other->m_wadfile;
		this->m_dictionary_position = other->m_dictionary_position;
		this->m_name = _strdup(other->m_name);
		this->m_position = other->m_position;
		this->m_size = other->m_size;
		this->m_data = NULL;
	}

	Lump::~Lump()
	{
		UnLoad();
		delete m_name;
	}

	int Lump::Load()
	{
		m_data = new char[m_size];
		m_wadfile->MoveTo(m_position);
		if (0 == m_wadfile->ReadString(m_data, m_size))
			return 1; // Error
		return 0;
	}
	void Lump::UnLoad()
	{
		if (m_data != NULL)
			delete m_data;
	}
}