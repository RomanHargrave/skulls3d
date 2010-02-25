
#ifndef SK_THINGSLUMP
#define SK_THINGSLUMP

#include <vector>
#include "Lump.h"
#include "../Thing.h"

namespace doom
{
	class ThingsLump : public Lump
	{
	protected:
		std::vector<Thing*> m_things;

	public:
		ThingsLump(Lump * lump);

		Thing * operator[](int index) const;

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_THINGSLUMP