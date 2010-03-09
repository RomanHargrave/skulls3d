
#ifndef SK_LINEDEF
#define SK_LINEDEF

#include "Vertex.h"
#include "lumps\VertexesLump.h"

namespace doom
{
	class WadFile;

	class LineDef
	{
	public:
		VertexesLump* m_vertexes;

		Vertex* m_start_vtx;
		Vertex* m_end_vtx;		
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
		//ToDo - Special Type, Sector Tag, Right Sidedef and Left Sidedef

		LineDef(WadFile * wadFile, VertexesLump* vertexLump , int offset);

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_LINEDEF