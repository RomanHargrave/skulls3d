
#include <memory.h>
#include "SideDef.h"
#include "Sector.h"
#include "Wad.h"
#include "Texture.h"
#include "lumps/LevelLump.h"
#include "..\File.h"

namespace skulls
{
	SideDef::SideDef(File & file, Level & level, std::map<std::string,std::shared_ptr<Texture>> & textures)
		:m_upperTexture(nullptr),
		m_lowerTexture(nullptr),
		m_middleTexture(nullptr)
	{
		m_xoffset = file.ReadInt2();
		m_yoffset = file.ReadInt2();

		{
			auto tex = textures.find(file.ReadString(8));
			if (tex != textures.end())
				m_upperTexture  = tex->second;
		}
		{
			auto tex = textures.find(file.ReadString(8));
			if (tex != textures.end())
				m_lowerTexture  = tex->second;
		}
		{
			auto tex = textures.find(file.ReadString(8));
			if (tex != textures.end())
				m_middleTexture = tex->second;
		}

		m_sector = level.m_sectors[file.ReadInt2()];
	}

	void SideDef::Resolve()
	{
		m_sector->m_sideDefs.push_back(shared_from_this());
	}

};
