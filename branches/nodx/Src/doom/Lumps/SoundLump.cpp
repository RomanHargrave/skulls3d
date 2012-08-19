
#include "SoundLump.h"
#include "Lump.h"
#include "..\..\File.h"

namespace skulls
{
	
	Sound::Sound(File & file, Lump & lump)
	{
		int oldPos = file.GetPos();

		file.MoveTo(lump.m_position);
		file.Skip(4);
		int len = file.ReadInt2();
		file.Skip(2);
		m_soundData.reserve(len);
		file.ReadString((char*)&m_soundData[0], len);

		file.MoveTo(oldPos);
	}

};
