
#ifndef SK_LINEDEF
#define SK_LINEDEF

#include "..\File.h"
#include "Vertex.h"
#include "Sector.h"
#include "SideDef.h"

namespace skulls
{
	class Wad;
	class SideDef;
	class Sector;
	class Level;

	class LineDef : public std::enable_shared_from_this<LineDef>
	{
	public:
		LineDef(File & file, Level & level);
		void Resolve();
		
	public:
		std::shared_ptr<Vertex> m_start_vtx;
		std::shared_ptr<Vertex> m_end_vtx;		
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
		std::shared_ptr<Sector > m_tagSector;
		std::shared_ptr<SideDef> m_rightSideDef;
		std::shared_ptr<SideDef> m_leftSideDef;
	};
};

#endif // SK_LINEDEF