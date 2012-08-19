
#include <stdlib.h>
#include "Lump.h"
#include "../Wad.h"

/*
namespace skulls
{
	Lump::Lump(Wad * Wad, int dictionary_position, char * name, int pos, int size)
	{
		this->m_Wad = Wad;
		this->m_dictionary_position = dictionary_position;
		this->m_name = _strdup(name);
		this->m_position = pos;
		this->m_size = size;
	}

	Lump::Lump(Lump * other)
	{
		this->m_Wad = other->m_Wad;
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
*/