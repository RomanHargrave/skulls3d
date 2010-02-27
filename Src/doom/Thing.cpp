
#include <stdlib.h>
#include "Thing.h"
#include "WadFile.h"

namespace doom
{
	Thing::Thing(WadFile * wadFile, int offset)
	{
		wadFile->MoveTo(offset);
		wadFile->ReadInt2(&m_x);
		wadFile->ReadInt2(&m_y);
		wadFile->ReadInt2(&m_angle);
		short flags;
		wadFile->ReadInt2(&flags);
		if (flags & 1) m_is_on_skill_1_2 = true;
		else           m_is_on_skill_1_2 = false;
		if (flags & 2) m_is_on_skill_3 = true;
		else           m_is_on_skill_3 = false;
		if (flags & 4) m_is_on_skill_4_5 = true;
		else           m_is_on_skill_4_5 = false;
		if (flags & 8) m_deaf = true;
		else           m_deaf = false;
		if (flags & 16) m_not_in_single_player = true;
		else            m_not_in_single_player = false;
	}
	
	int Thing::Load()
	{
		return 0;
	}
	void Thing::UnLoad()
	{
	}
};
