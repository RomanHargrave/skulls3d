
#ifndef SK_SIDEDEF
#define SK_SIDEDEF

#include "lumps/LevelLump.h"
#include "..\File.h"
#include "Texture.h"
#include "Sector.h"
#include "LineDef.h"

namespace skulls
{
	class SideDef : public std::enable_shared_from_this<SideDef>
	{
	public:
		SideDef::SideDef(File & file, Level & level, std::map<std::string,std::shared_ptr<Texture>> & textures);
		void Resolve();

	public:
		short m_xoffset;
		short m_yoffset;
		std::shared_ptr<Texture> m_upperTexture;
		std::shared_ptr<Texture> m_lowerTexture;
		std::shared_ptr<Texture> m_middleTexture;
		std::shared_ptr<Sector > m_sector;
		std::shared_ptr<LineDef> m_lineDef;
	};
};

#endif // SK_SIDEDEF