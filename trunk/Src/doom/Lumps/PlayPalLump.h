
#ifndef SK_PLAYPALLUMP
#define SK_PLAYPALLUMP

#include "Lump.h"

namespace doom
{
	class PlayPalLump : public Lump
	{
	public:
		unsigned int ** m_palettes;

		PlayPalLump(Lump * lump);

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_PLAYPALLUMP