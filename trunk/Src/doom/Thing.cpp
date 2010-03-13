
#include <stdlib.h>
#include "Thing.h"
#include "WadFile.h"

	typedef struct
	{
		unsigned char radius;
		char * sprite;
		char * sequence;
		bool artifact;
		bool pickup;
		bool obstacle;
		bool hanging;
		// TODO...

	}ThingDef;
	ThingDef thingdefs[] = {
		{ 1, "PADD", NULL, false, false, false, false},
		{16, "PLAY", NULL, false, false, false, false},
		{16, "PLAY", NULL, false, false, false, false},
		{16, "PLAY", NULL, false, false, false, false},
		{16, "PLAY", NULL, false, false, false, false},
		{20, "BKEY", "AB", false,  true, false, false},
		{20, "YKEY", "AB", false,  true, false, false},
		{128,"SPID", NULL, false, false,  true, false},
		{20, "BPAK", "A",  false,  true, false, false},
		{20, "SPOS", NULL, false, false,  true, false},
		{16, "PLAY", "W",  false, false, false, false},
		{20, NULL,   NULL, false, false, false, false},
		{16, "PLAY", "W",  false, false, false, false},
		{20, "RKEY", "AB", false,  true, false, false},
		{20, NULL, 	 NULL, false, false, false, false},
		{16, "PLAY", "N",  false, false, false, false},
		{40, "CYBR", "ABCD",false, false,  true, false},
		{20, "CELP", "A",  false,  true, false, false},
		{20, "POSS", "L",  false, false, false, false},
		{20, "SPOS", "L",  false, false, false, false},
		{20, "TROO", "M",  false, false, false, false},
		{30, "SARG", "N",  false, false, false, false},
		{31, "HEAD", "L",  false, false, false, false},
		{16, "SKUL", "K",  false, false, false, false},
		{16, "POL5", "A",   false, false, false, false},
		{16, "POL1", "A",   false, false,  true, false},
		{16, "POL6", "AB",  false, false,  true, false},
		{16, "POL4", "A",   false, false,  true, false},
		{16, "POL2", "A",   false, false,  true, false},
		{16, "POL3", "AB",  false, false,  true, false},
		{16, "COL1", "A",   false, false,  true, false},
		{16, "COL2", "A",   false, false,  true, false},
		{16, "COL3", "A",   false, false,  true, false},
		{16, "COL4", "A",   false, false,  true, false},
		{16, "CAND", "A",   false, false, false, false},
		{16, "CBRA", "A",   false, false,  true, false},
		{16, "COL5", "AB",  false, false,  true, false},
		{16, "COL6", "A",   false, false,  true, false},
		{20, "RSKU", "AB",  false,  true, false, false},
		{20, "YSKU", "AB",  false,  true, false, false},
		{20, "BSKU", "AB",  false,  true, false, false},
		{16, "CEYE", "ABCB",false, false,  true, false},
		{16, "FSKU", "ABC", false, false,  true, false},
		{16, "TRE1", "A", 	false, false,  true, false},
		{16, "TBLU", "ABCD",false, false,  true, false},
		{16, "TGRN", "ABCD",false, false,  true, false},
		{16, "TRED", "ABCD",false, false,  true, false},
		{16, "SMIT", "A", 	false, false,  true, false},
		{16, "ELEC", "A", 	false, false,  true, false},
		{16, "GOR1", "ABCB",false, false,  true,  true},
		{16, "GOR2", "A", 	false, false,  true,  true},
		{16, "GOR3", "A", 	false, false,  true,  true},
		{16, "GOR4", "A", 	false, false,  true,  true},
		{16, "GOR5", "A", 	false, false,  true,  true},
		{32, "TRE2", "A", 	false, false,  true, false},
		{16, "SMBT", "ABCD",false, false,  true, false},
		{16, "SMGT", "ABCD",false, false,  true, false},
		{16, "SMRT", "ABCD",false, false,  true, false},
		{30, "SARG", NULL,	false, false,  true, false},
		{16, "GOR2", "A", 	false, false, false,  true},
		{16, "GOR4", "A", 	false, false, false,  true},
		{16, "GOR3", "A", 	false, false, false,  true},
		{16, "GOR5", "A", 	false, false, false,  true},
		{16, "GOR1", "ABCB",false, false, false,  true},
		{20, "VILE", NULL,	false, false,  true, false},
		{20, "CPOS", NULL,	false, false,  true, false},
		{20, "SKEL", NULL,	false, false,  true, false},
		{48, "FATT", NULL,	false, false,  true, false},
		{64, "BSPI", NULL,	false, false,  true, false},
		{24, "BOS2", NULL,	false, false,  true, false},
		{10, "FCAN", "ABC", false, false,  true, false},
		{31, "PAIN", NULL,	false, false,  true,  true},
		{16, "KEEN", "A", 	false, false,  true,  true},
		{16, "HDB1", "A", 	false, false,  true,  true},
		{16, "HDB2", "A", 	false, false,  true,  true},
		{16, "HDB3", "A", 	false, false,  true,  true},
		{16, "HDB4", "A", 	false, false,  true,  true},
		{16, "HDB5", "A", 	false, false,  true,  true},
		{16, "HDB6", "A", 	false, false,  true,  true},
		{16, "POB1", "A", 	false, false, false, false},
		{16, "POB2", "A", 	false, false, false, false},
		{16, "BRS1", "A", 	false, false, false, false},
		{20, "SGN2", "A", 	false,  true, false, false},
		{20, "MEGA", "ABCD", true,  true, false, false},
		{20, "SSWV", NULL,	false, false,  true, false},
		{16, "TLMP", "ABCD",false, false,  true, false},
		{16, "TLP2", "ABCD",false, false,  true, false},
		{0,  NULL,   NULL,  false, false, false, false},
		{16, "BBRN", NULL,  false, false,  true, false},
		{20, NULL,   NULL,  false, false, false, false},
		{20, "SHOT", "A", 	false,  true, false, false},
		{20, "MGUN", "A", 	false,  true, false, false},
		{20, "LAUN", "A", 	false,  true, false, false},
		{20, "PLAS", "A", 	false,  true, false, false},
		{20, "CSAW", "A", 	false,  true, false, false},
		{20, "BFUG", "A", 	false,  true, false, false},
		{20, "CLIP", "A", 	false,  true, false, false},
		{20, "SHEL", "A", 	false,  true, false, false},
		{20, "ROCK", "A", 	false,  true, false, false},
		{20, "STIM", "A", 	false,  true, false, false},
		{20, "MEDI", "A", 	false,  true, false, false},
		{20, "SOUL", "ABCDCB",true,  true, false, false},
		{20, "BON1", "ABCDCB",true,  true, false, false},
		{20, "BON2", "ABCDCB",true,  true, false, false},
		{20, "ARM1", "AB", 	false,  true, false, false},
		{20, "ARM2", "AB", 	false,  true, false, false},
		{20, "PINV", "ABCD",  true,  true, false, false},
		{20, "PSTR", "A", 	 true,  true, false, false},
		{20, "PINS", "ABCD",  true,  true, false, false},
		{20, "SUIT", "A", 	false,  true, false, false},
		{20, "PMAP", "ABCDCB",true,  true, false, false},
		{16, "COLU", "A", 	false, false,  true, false},
		{10, "BAR1", "AB", 	false, false,  true, false},
		{20, "PVIS", "AB", 	 true,  true, false, false},
		{20, "BROK", "A", 	false,  true, false, false},
		{20, "CELL", "A", 	false,  true, false, false},
		{20, "AMMO", "A", 	false,  true, false, false},
		{20, "SBOX", "A", 	false,  true, false, false},
		{20, "TROO", "A",	false, false,  true, false},
		{30, "SARG", "A",	false, false,  true, false},
		{24, "BOSS", "A",	false, false,  true, false},
		{20, "POSS", "A",	false, false,  true, false},
		{31, "HEAD", "A",	false, false,  true,  true},
		{16, "SKUL", "A",	false, false,  true,  true}
	};

namespace doom
{

	Thing::Thing(WadFile * wadFile, int offset)
	{
		m_wadfile = wadFile;
		m_sprites.resize(0);
		wadFile->MoveTo(offset);
		wadFile->ReadInt2(&m_x);
		wadFile->ReadInt2(&m_y);
		wadFile->ReadInt2(&m_angle);
		wadFile->ReadInt2((short*)&m_type);
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
	Thing::~Thing()
	{
		UnLoad();
	}
	
	int Thing::Load()
	{
		if (m_sprites.size() != 0)
			return 0; // Already loaded

		int type = m_type;
		if (type >= 2001 && type < 3000)
			type = type - 2001 + 90;
		else if (type >= 3001)
			type = type - 3001 + 2050 - 2001 + 90;

		PatchLump * sprite;

		if (thingdefs[type].sprite != NULL)
		{
			if (strcmp(thingdefs[type].sprite, "CYBR") == 0)
			{
				int a=0;
			}
			if (thingdefs[type].sequence != NULL)
			{
				char spriteName[9];
				sprintf_s(spriteName, "%s%c0", thingdefs[type].sprite, thingdefs[type].sequence[0]);
				sprite = m_wadfile->GetLump((PatchLump*)m_wadfile->Get(spriteName));
				if (sprite == NULL)
				{
					sprintf_s(spriteName, "%s%c1", thingdefs[type].sprite, thingdefs[type].sequence[0]);
					sprite = m_wadfile->GetLump((PatchLump*)m_wadfile->Get(spriteName));
				}
			}
			else
			{
				sprite = m_wadfile->GetLump((PatchLump*)m_wadfile->Get(thingdefs[type].sprite));
			}
		}
		else
		{
			sprite = m_wadfile->GetLump((PatchLump*)m_wadfile->Get("PLAYA1"));
		}
		if (sprite == 0)
			return -1;
		if (sprite->Load() != 0)
			return -2;

		m_sprites.push_back(sprite);
		return 0;
	}
	void Thing::UnLoad()
	{
		if (m_sprites.size() != 0)
			m_sprites.clear();
	}

	
};
