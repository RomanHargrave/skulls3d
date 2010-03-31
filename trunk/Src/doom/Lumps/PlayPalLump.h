
#ifndef SK_PLAYPALLUMP
#define SK_PLAYPALLUMP

#include "Lump.h"

namespace doom
{
	class PlayPalLump : public Lump
	{
	public:
		unsigned int * m_palette;

		PlayPalLump(void *referencer, Lump * lump);

		virtual bool Load();
		virtual void UnLoad();
	};
};

#endif // SK_PLAYPALLUMP