
#ifndef SK_THINGLUMP
#define SK_THINGLUMP

#include <vector>

namespace doom
{
	class WadFile;
	class PatchLump;

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

		std::vector<PatchLump*> m_sprites;

		Thing(WadFile * wadFile);
	};
};


#endif // SK_THINGLUMP