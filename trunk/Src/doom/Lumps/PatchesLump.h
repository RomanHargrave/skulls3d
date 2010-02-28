
#ifndef SK_PATHESLUMP
#define SK_PATHESLUMP

#include <vector>
#include "Lump.h"
#include "PatchLump.h"

namespace doom
{
	class PatchesLump : public Lump
	{
	public:
		std::vector<PatchLump*> m_patches;

		PatchesLump(Lump * other);
		virtual ~PatchesLump();

		PatchLump * operator[](int index) const;

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif //  SK_PATHESLUMP