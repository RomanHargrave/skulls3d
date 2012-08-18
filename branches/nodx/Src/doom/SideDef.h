
#ifndef SK_SIDEDEF
#define SK_SIDEDEF

namespace doom
{
	class WadFile;
	class Sector;
	class Texture;
	class LevelLump;
	class LineDef;

	class SideDef
	{
	public:
		short m_xoffset;
		short m_yoffset;
		Texture *m_upperTexture;
		Texture *m_lowerTexture;
		Texture *m_middleTexture;
		Sector *m_sector;
		LineDef *m_lineDef;

		SideDef(WadFile *wadFile, LevelLump *level);
	};
};

#endif // SK_SIDEDEF