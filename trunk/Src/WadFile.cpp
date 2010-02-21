
#include <stdlib.h>
#include "File.h"
#include "WadFile.h"
#include "Lumps/Lump.h"

namespace Doom
{
	WadFile::WadFile(const char * filename)
		:File(filename)
	{
	}

	int WadFile::Load()
	{
		// TODO
		return 0;
	}

	void WadFile::UnLoad()
	{
		this->m_lumps.clear();
	}
	
	Lump& WadFile::GetLump(int index)
	{
		return m_lumps[index];
	}
};