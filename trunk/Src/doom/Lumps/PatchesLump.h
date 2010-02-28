
#ifndef SK_PATHESLUMP
#define SK_PATHESLUMP

#include <vector>
#include "Lump.h"
#include "PatchLump.h"
#include "PlayPalLump.h"

namespace doom
{
	class PatchesLump : public Lump
	{
	public:
		std::vector<PatchLump*> m_patches;

		PatchesLump(Lump * other);
		virtual ~PatchesLump();

		PatchLump * operator[](int index) const;

		virtual int Load(PlayPalLump * palettes);
		virtual void UnLoad();
	};
};

#endif //  SK_PATHESLUMP