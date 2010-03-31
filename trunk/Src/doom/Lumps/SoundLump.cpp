
#include "Lump.h"
#include "SoundLump.h"
#include "../WadFile.h"

namespace doom
{
	SoundLump::SoundLump(void *referencer, Lump *other)
		:Lump(referencer, other)
	{
		m_soundData = NULL;
		m_len = 0;
	}

	bool SoundLump::Load()
	{
		if (m_soundData != NULL)
			return true; // Already loaded

		m_wadfile->MoveTo(m_position);
		m_wadfile->Skip(4);
		m_wadfile->ReadInt2((short*)&m_len);
		m_wadfile->Skip(2);
		m_soundData = new unsigned char[m_len];
		m_wadfile->ReadString((char*)m_soundData, m_len);

		return true;
	}
	void SoundLump::UnLoad()
	{
		if (m_soundData != NULL)
		{
			delete m_soundData;
			m_soundData = NULL;
			m_len = 0;
		}
	}
};
