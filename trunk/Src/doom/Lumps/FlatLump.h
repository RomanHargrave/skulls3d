
#ifndef SK_FLAT
#define SK_FLAT

#include "Lump.h"

namespace doom
{
	class FlatLump : public Lump
	{
	public:
		unsigned int * m_bitmap;

		FlatLump(void *referencer, Lump * other);
		
		virtual bool Load();
		virtual void UnLoad();
	};
};

#endif // SK_FLAT