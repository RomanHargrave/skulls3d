
#ifndef SK_LEVELLUMP
#define SK_LEVELLUMP

#include "Lump.h"
#include "ThingsLump.h"

namespace doom
{
	class LevelLump : public Lump
	{
	protected:
		ThingsLump * m_things;

	public:
		static LevelLump * Get(Lump * lump);

		LevelLump(Lump * lump);

		virtual int Load();
		virtual void UnLoad();

		// TODO : This is for debug only, remove later
		virtual char* ToString();
	};
};

#endif // SK_LEVELLUMP