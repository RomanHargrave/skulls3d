
#ifndef SK_LINEDEFSLUMP
#define SK_LINEDEFSLUMP

#include <vector>
#include "Lump.h"
#include "../LineDef.h"

namespace doom
{
	class LineDefsLump : public Lump
	{
	protected:
		std::vector<LineDef*> m_linedefs;
	public:
		LevelLump* m_level;		
		int size;
		LineDefsLump(Lump * Lump);
		~LineDefsLump();

		LineDef * Get(int index);

		virtual int Load(LevelLump * levelLump);
		virtual void UnLoad();
	};
};

#endif // SK_THINGSLUMP