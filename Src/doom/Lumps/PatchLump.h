
#ifndef SK_PATCHLUMP
#define SK_PATCHLUMP

#include "Lump.h"

namespace doom
{
	class WadFile;
	class Lump;

	class PatchLump : public Lump
	{
	public:
		unsigned short m_w, m_h;
		unsigned int * m_bitmap;

		PatchLump(doom::Lump *other);
		virtual ~PatchLump();
		
		virtual bool Load();
		virtual void UnLoad();
	};
};

#endif // SK_PATCHLUMP