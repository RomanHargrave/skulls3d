
#include <stdlib.h>
#include "Thing.h"
#include "WadFile.h"

	typedef struct
	{
		unsigned short id;
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
		{ 1, 16, "PLAY", NULL, false, false, false, false}, // 1
		{ 2, 16, "PLAY", NULL, false, false, false, false},
		{ 3, 16, "PLAY", NULL, false, false, false, false},
		{ 4, 16, "PLAY", NULL, false, false, false, false},
		{ 5, 20, "BKEY", "AB", false,  true, false, false},
		{ 6, 20, "YKEY", "AB", false,  true, false, false},
		{ 7,128, "SPID", NULL, false, false,  true, false},
		{ 8, 20, "BPAK", "A",  false,  true, false, false},
		{ 9, 20, "SPOS", NULL, false, false,  true, false},
		{10, 16, "PLAY", "W",  false, false, false, false},
		{11, 20, NULL,   NULL, false, false, false, false},
		{12, 16, "PLAY", "W",  false, false, false, false},
		{13, 20, "RKEY", "AB", false,  true, false, false},
		{14, 20, NULL, 	 NULL, false, false, false, false},
		{15, 16, "PLAY", "N",  false, false, false, false},
		{16, 40, "CYBR", "ABCD",false, false,  true, false}, //
		{17, 20, "CELP", "A",  false,  true, false, false},
		{18, 20, "POSS", "L",  false, false, false, false},
		{19, 20, "SPOS", "L",  false, false, false, false},
		{20, 20, "TROO", "M",  false, false, false, false}, // 20
		{21, 30, "SARG", "N",  false, false, false, false},
		{22, 31, "HEAD", "L",  false, false, false, false},
		{23, 16, "SKUL", "K",  false, false, false, false},
		{24, 16, "POL5", "A",   false, false, false, false},
		{25, 16, "POL1", "A",   false, false,  true, false},
		{26, 16, "POL6", "AB",  false, false,  true, false},
		{27, 16, "POL4", "A",   false, false,  true, false},
		{28, 16, "POL2", "A",   false, false,  true, false},
		{29, 16, "POL3", "AB",  false, false,  true, false},
		{30, 16, "COL1", "A",   false, false,  true, false},
		{31, 16, "COL2", "A",   false, false,  true, false},
		{32, 16, "COL3", "A",   false, false,  true, false},
		{33, 16, "COL4", "A",   false, false,  true, false},
		{34, 16, "CAND", "A",   false, false, false, false},
		{35, 16, "CBRA", "A",   false, false,  true, false},
		{36, 16, "COL5", "AB",  false, false,  true, false},
		{37, 16, "COL6", "A",   false, false,  true, false},
		{38, 20, "RSKU", "AB",  false,  true, false, false},
		{39, 20, "YSKU", "AB",  false,  true, false, false},
		{40, 20, "BSKU", "AB",  false,  true, false, false},
		{41, 16, "CEYE", "ABCB",false, false,  true, false},
		{42, 16, "FSKU", "ABC", false, false,  true, false},
		{43, 16, "TRE1", "A", 	false, false,  true, false},
		{44, 16, "TBLU", "ABCD",false, false,  true, false},
		{45, 16, "TGRN", "ABCD",false, false,  true, false},
		{46, 16, "TRED", "ABCD",false, false,  true, false},
		{47, 16, "SMIT", "A", 	false, false,  true, false},
		{48, 16, "ELEC", "A", 	false, false,  true, false},
		{49, 16, "GOR1", "ABCB",false, false,  true,  true},
		{50, 16, "GOR2", "A", 	false, false,  true,  true},
		{51, 16, "GOR3", "A", 	false, false,  true,  true},
		{52, 16, "GOR4", "A", 	false, false,  true,  true},
		{53, 16, "GOR5", "A", 	false, false,  true,  true},
		{54, 32, "TRE2", "A", 	false, false,  true, false},
		{55, 16, "SMBT", "ABCD",false, false,  true, false},
		{56, 16, "SMGT", "ABCD",false, false,  true, false},
		{57, 16, "SMRT", "ABCD",false, false,  true, false},
		{58, 30, "SARG", NULL,	false, false,  true, false},
		{59, 16, "GOR2", "A", 	false, false, false,  true},
		{60, 16, "GOR4", "A", 	false, false, false,  true},
		{61, 16, "GOR3", "A", 	false, false, false,  true},
		{62, 16, "GOR5", "A", 	false, false, false,  true},
		{63, 16, "GOR1", "ABCB",false, false, false,  true},
		{64, 20, "VILE", NULL,	false, false,  true, false},
		{65, 20, "CPOS", NULL,	false, false,  true, false},
		{66, 20, "SKEL", NULL,	false, false,  true, false},
		{67, 48, "FATT", NULL,	false, false,  true, false},
		{68, 64, "BSPI", NULL,	false, false,  true, false},
		{69, 24, "BOS2", NULL,	false, false,  true, false},
		{70, 10, "FCAN", "ABC", false, false,  true, false},
		{71, 31, "PAIN", NULL,	false, false,  true,  true},
		{72, 16, "KEEN", "A", 	false, false,  true,  true},
		{73, 16, "HDB1", "A", 	false, false,  true,  true},
		{74, 16, "HDB2", "A", 	false, false,  true,  true},
		{75, 16, "HDB3", "A", 	false, false,  true,  true},
		{76, 16, "HDB4", "A", 	false, false,  true,  true},
		{77, 16, "HDB5", "A", 	false, false,  true,  true},
		{78, 16, "HDB6", "A", 	false, false,  true,  true},
		{79, 16, "POB1", "A", 	false, false, false, false},
		{80, 16, "POB2", "A", 	false, false, false, false},
		{81, 16, "BRS1", "A", 	false, false, false, false},
		{82, 20, "SGN2", "A", 	false,  true, false, false},
		{83, 20, "MEGA", "ABCD", true,  true, false, false},
		{84, 20, "SSWV", NULL,	false, false,  true, false},
		{85, 16, "TLMP", "ABCD",false, false,  true, false},
		{86, 16, "TLP2", "ABCD",false, false,  true, false},
		{87, 0,  NULL,   NULL,  false, false, false, false},
		{88, 16, "BBRN", NULL,  false, false,  true, false},
		{89, 20, NULL,   NULL,  false, false, false, false},
		{2001, 20, "SHOT", "A", 	false,  true, false, false},
		{2002, 20, "MGUN", "A", 	false,  true, false, false},
		{2003, 20, "LAUN", "A", 	false,  true, false, false},
		{2004, 20, "PLAS", "A", 	false,  true, false, false},
		{2005, 20, "CSAW", "A", 	false,  true, false, false},
		{2006, 20, "BFUG", "A", 	false,  true, false, false},
		{2007, 20, "CLIP", "A", 	false,  true, false, false},
		{2008, 20, "SHEL", "A", 	false,  true, false, false},
		{2010, 20, "ROCK", "A", 	false,  true, false, false},
		{2011, 20, "STIM", "A", 	false,  true, false, false},
		{2012, 20, "MEDI", "A", 	false,  true, false, false}, // 100
		{2013, 20, "SOUL", "ABCDCB",true,  true, false, false},
		{2014, 20, "BON1", "ABCDCB",true,  true, false, false},
		{2015, 20, "BON2", "ABCDCB",true,  true, false, false},
		{2018, 20, "ARM1", "AB", 	false,  true, false, false},
		{2019, 20, "ARM2", "AB", 	false,  true, false, false},
		{2022, 20, "PINV", "ABCD",  true,  true, false, false},
		{2023, 20, "PSTR", "A", 	 true,  true, false, false},
		{2024, 20, "PINS", "ABCD",  true,  true, false, false},
		{2025, 20, "SUIT", "A", 	false,  true, false, false},
		{2026, 20, "PMAP", "ABCDCB",true,  true, false, false},
		{2028, 16, "COLU", "A", 	false, false,  true, false},
		{2035, 10, "BAR1", "AB", 	false, false,  true, false},
		{2045, 20, "PVIS", "AB", 	 true,  true, false, false},
		{2046, 20, "BROK", "A", 	false,  true, false, false},
		{2047, 20, "CELL", "A", 	false,  true, false, false},
		{2048, 20, "AMMO", "A", 	false,  true, false, false},
		{2049, 20, "SBOX", "A", 	false,  true, false, false},
		{3001, 20, "TROO", "A",	false, false,  true, false},
		{3002, 30, "SARG", "A",	false, false,  true, false},
		{3003, 24, "BOSS", "A",	false, false,  true, false}, // 120
		{3004, 20, "POSS", "A",	false, false,  true, false},
		{3005, 31, "HEAD", "A",	false, false,  true,  true},
		{3006, 16, "SKUL", "A",	false, false,  true,  true}
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

		int type = 0;
		int i;
		for (i=0 ; i < sizeof(thingdefs)/sizeof(ThingDef) ; i++)
		{
			if (thingdefs[i].id == m_type)
			{
				type = i;
				break;
			}
		}
		if (i == sizeof(thingdefs)/sizeof(ThingDef))
		{
			printf("Thing type %d not found\n", m_type);
		}

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
