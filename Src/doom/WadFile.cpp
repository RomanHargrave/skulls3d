
#include <stdlib.h>
#include "../File.h"
#include "WadFile.h"
#include "Lumps/Lump.h"
#include "Lumps/LevelLump.h"

namespace doom
{
	WadFile::WadFile(const char * filename)
		:File(filename)
	{
	}
	WadFile::~WadFile()
	{
		UnLoad();
	}

	Lump* WadFile::GetLump(int index)
	{
		return m_lumps[index];
	}

	void WadFile::SetLump(Lump * newLump)
	{
		Lump * oldLump = m_lumps[newLump->m_dictionary_position];
		if (oldLump != NULL)
			delete oldLump;
		m_lumps[newLump->m_dictionary_position] = newLump;
	}

	int WadFile::Load()
	{
		char buffer[10];

		// Reading header
		if (4 != ReadString(buffer, 4))
			return 1;
		if (_strnicmp(buffer, "IWAD", 4) == 0)
			m_internal = true;
		else
			m_internal = false;

		// Reading lump count
		int lump_count;
		if (4 != ReadInt4(&lump_count))
			return 1;
		m_lumps.resize(lump_count);

		if (4 != ReadInt4(&m_directory_potision))
			return 1;

		if (ReadLumpDictionary() != 0)
			return 2;

		if (ReadLevels() != 0)
			return 3;

		return 0;
	}

	void WadFile::UnLoad()
	{
		this->m_lumps.clear();
	}

	int WadFile::ReadLumpDictionary()
	{
		MoveTo(m_directory_potision);

		int lump_count = (int)m_lumps.size();
		char lump_name[9];
		int lump_pos, lump_size;
		for (int i=0 ; i<lump_count ; i++)
		{
			if (4 != ReadInt4(&lump_pos))
				return 1;
			if (4 != ReadInt4(&lump_size))
				return 1;
			memset(lump_name, 0, 9);
			if (8 != ReadString(lump_name, 8))
				return 1;
			Lump * lump = new Lump(this, i, lump_name, lump_pos, lump_size);
			m_lumps[i] = lump;
			printf("%08d + %08d %s\n", lump_pos, lump_size, lump_name);
		}
		return 0;
	}
	
	int WadFile::ReadLevels()
	{
		int lump_count = (int)m_lumps.size();
		LevelLump * current_level = NULL;
		for (int i=0 ; i<lump_count ; i++)
		{
			if (   m_lumps[i]->m_name[0] == 'E' && m_lumps[i]->m_name[2] == 'M'
				&& m_lumps[i]->m_name[1] >= '0' && m_lumps[i]->m_name[3] <= '9'
				&& strlen(m_lumps[i]->m_name) == 4)
			{
				// Get may return NULL
				current_level = GetLump((LevelLump*)m_lumps[i]);
				if (current_level == NULL)
					continue;
				current_level->Load();
			}
		}
		return 0;
	}
};