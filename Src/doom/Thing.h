
#ifndef SK_THINGLUMP
#define SK_THINGLUMP

#include <vector>
#include "..\File.h"

namespace skulls
{
	class Wad;
	class Patch;
	class Patches;

	class Thing
	{
	public:
		short m_x;
		short m_z;
		short m_angle;
		unsigned short m_type;
		bool m_is_on_skill_1_2;
		bool m_is_on_skill_3;
		bool m_is_on_skill_4_5;
		bool m_deaf;
		bool m_not_in_single_player;

		std::vector<std::shared_ptr<Patch>> m_sprites;

		Thing(File & file, Patches & patches);
	};
}


#endif // SK_THINGLUMP