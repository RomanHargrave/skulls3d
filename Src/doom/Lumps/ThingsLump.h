
#ifndef SK_THINGSLUMP
#define SK_THINGSLUMP

#include <vector>
#include "Lump.h"
#include "../Thing.h"

namespace doom
{
	class ThingsLump : public Lump
	{
	public:
		std::vector<Thing*> m_things;

		ThingsLump(Lump * lump);

		Thing * operator[](int index) const;

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_THINGSLUMP