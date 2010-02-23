
#ifndef SK_LEVELLUMP
#define SK_LEVELLUMP

#include "Lump.h"

namespace doom
{
	class LevelLump : public Lump
	{
	public:
		static LevelLump * Get(Lump * lump);

		LevelLump(Lump * lump);

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_LEVELLUMP