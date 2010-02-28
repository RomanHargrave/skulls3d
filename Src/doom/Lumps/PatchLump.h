
#ifndef SK_PATCHLUMP
#define SK_PATCHLUMP

#include "Lump.h"
#include "PlayPalLump.h"

namespace doom
{
	class PatchLump : public Lump
	{
	public:
		unsigned short m_w, m_h;
		unsigned int * m_texture;

		PatchLump(Lump * other);
		virtual ~PatchLump();

		virtual int Load(PlayPalLump * palettes);
		virtual void UnLoad();
	};
};

#endif // SK_PATCHLUMP