
#ifndef SK_LINEDEF
#define SK_LINEDEF

namespace doom
{
	class WadFile;
	class Vertex;
	class SideDef;
	class Sector;
	class LevelLump;

	class LineDef
	{
	public:
		Vertex *m_start_vtx;
		Vertex *m_end_vtx;		
		//Flags - Start
		bool m_block_p_m;
		bool m_block_m;
		bool m_2_sided;		
		bool m_upper_unpegged;
		bool m_lower_unpegged;
		bool m_secret;
		bool m_block_sound;
		bool m_never_automap;
        bool m_always_automap;
		//Flag - End
		unsigned short m_type;
		Sector *m_tagSector;
		SideDef *m_rightSideDef;
		SideDef *m_leftSideDef;

		LineDef(WadFile * wadFile, LevelLump *level);
	};
};

#endif // SK_LINEDEF