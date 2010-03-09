
#ifndef SK_LINEDEFSLUMP
#define SK_LINEDEFSLUMP

#include <vector>
#include "Lump.h"
#include "../LineDef.h"

namespace doom
{
	class LineDefsLump : public Lump
	{
	public:
		LevelLump* m_level;

		std::vector<LineDef*> m_linedefs;
		
		LineDefsLump(LevelLump * levelLump);
		~LineDefsLump();

		LineDef * operator[](int index) const;

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_THINGSLUMP