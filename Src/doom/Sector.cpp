
#include <memory.h>
#include "Sector.h"
#include "Wad.h"
#include "lumps/FlatLump.h"
#include "lumps/Lump.h"
#include "LumpDictionary.h"
#include "Lumps\PlayPalLump.h"

namespace skulls
{
	Sector::Sector(File & file, LumpDictionary & dictionary, Palettes & palettes)
	{
		m_floorHeight   = file.ReadInt2();
		m_ceilingHeight = file.ReadInt2();

		m_floorTexture   = dictionary.GetFlat(file, file.ReadString(8), palettes);
		m_ceilingTexture = dictionary.GetFlat(file, file.ReadString(8), palettes);

		m_lightLevel = file.ReadInt2();
		m_type       = file.ReadInt2();
		m_tag        = file.ReadInt2();
	}
};
