
#ifndef SK_THINGLUMP
#define SK_THINGLUMP

namespace doom
{
	class WadFile;

	class Thing
	{
	public:
		short m_x;
		short m_y;
		short m_angle;
		bool m_is_on_skill_1_2;
		bool m_is_on_skill_3;
		bool m_is_on_skill_4_5;
		bool m_deaf;
		bool m_not_in_single_player;

		Thing(WadFile * wadFile, int offset);

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_THINGLUMP