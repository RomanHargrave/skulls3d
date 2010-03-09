
#include "Lump.h"
#include "SoundLump.h"
#include "../WadFile.h"

namespace doom
{
	SoundLump::SoundLump(Lump *other)
		:Lump(other)
	{
		m_soundData = NULL;
		m_len = 0;
	}

	int SoundLump::Load()
	{
		if (m_soundData != NULL)
			return 0; // Already loaded

		m_wadfile->MoveTo(m_position);
		m_wadfile->Skip(4);
		m_wadfile->ReadInt2((short*)&m_len);
		m_wadfile->Skip(2);
		m_soundData = new unsigned char[m_len];
		m_wadfile->ReadString((char*)m_soundData, m_len);

		return 0;
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
