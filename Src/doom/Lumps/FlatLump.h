
#ifndef SK_FLAT
#define SK_FLAT

#include "Lump.h"

namespace doom
{
	class FlatLump : public Lump
	{
	public:
		unsigned int * m_texture;

		FlatLump(Lump * other);
		
		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_FLAT