
#include <stdlib.h>
#include "Lump.h"
#include "../ReferencedObject.h"
#include "../WadFile.h"

namespace doom
{
	Lump::Lump(void *referencer, WadFile * wadfile, int dictionary_position, char * name, int pos, int size)
		:ReferencedObject(referencer)
	{
		this->m_wadfile = wadfile;
		this->m_dictionary_position = dictionary_position;
		this->m_name = _strdup(name);
		this->m_position = pos;
		this->m_size = size;
	}

	Lump::Lump(void *referencer, Lump * other)
		:ReferencedObject(referencer)
	{
		this->m_wadfile = other->m_wadfile;
		this->m_dictionary_position = other->m_dictionary_position;
		this->m_name = _strdup(other->m_name);
		this->m_position = other->m_position;
		this->m_size = other->m_size;
	}

	Lump::~Lump()
	{
		UnLoad();
	}

	bool Lump::Load()
	{
		return true;
	}
	void Lump::UnLoad()
	{
		if (m_name != NULL)
		{
			delete m_name;
			m_name = NULL;
		}
	}
}