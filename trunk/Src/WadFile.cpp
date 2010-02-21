
#include <stdlib.h>
#include "File.h"
#include "WadFile.h"
#include "Lumps/Lump.h"

namespace doom
{
	WadFile::WadFile(const char * filename)
		:File(filename)
	{
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

		return ReadLumpDictionary();
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
			Lump * lump = new Lump(this, lump_name, lump_pos, lump_size);
			m_lumps[i] = lump;
			printf("%08d + %08d %s\n", lump_pos, lump_size, lump_name);
		}
		return 0;
	}
	
	Lump* WadFile::GetLump(int index)
	{
		return m_lumps[index];
	}
};