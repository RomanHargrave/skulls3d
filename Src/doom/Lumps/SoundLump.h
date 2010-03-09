
#ifndef SK_SOUNDLUMP
#define SK_SOUNDLUMP

#include "Lump.h"

namespace doom
{
	class SoundLump : public Lump
	{
	public:
		unsigned char *m_soundData;
		unsigned short m_len;

		SoundLump(Lump *other);

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_SOUNDLUMP