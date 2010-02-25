
#ifndef SK_THINGLUMP
#define SK_THINGLUMP

#include "WadFile.h"

namespace doom
{
	class Thing
	{
	protected:
		short m_x;
		short m_y;
		short m_angle;
		bool m_is_on_skill_1_2;
		bool m_is_on_skill_3;
		bool m_is_on_skill_4_5;
		bool m_deaf;
		bool m_not_in_single_player;
	public:
		Thing(WadFile * wadFile, int offset);
	};
};

#endif // SK_THINGLUMP