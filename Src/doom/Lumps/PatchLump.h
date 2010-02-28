
#ifndef SK_PATCHLUMP
#define SK_PATCHLUMP

#include "Lump.h"

namespace doom
{
	class PatchLump : public Lump
	{
	public:
		unsigned short m_w, m_h;
		unsigned int * m_texture;

		PatchLump(Lump * other);
		virtual ~PatchLump();

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_PATCHLUMP