
#include <stdlib.h>
#include "../File.h"
#include "WadFile.h"
#include "Lumps/Lump.h"
#include "Lumps/LevelLump.h"
#include "Lumps/PatchesLump.h"
#include "Lumps/PlayPalLump.h"
#include "Texture.h"

namespace doom
{
	WadFile::WadFile(const char * filename)
		:File(filename)
	{
		m_levels.resize(0);
		m_lumps.resize(0);
		m_textures.resize(0);
		m_palettes = NULL;
		m_patches = NULL;
	}
	WadFile::~WadFile()
	{
		UnLoad();
	}

	Lump* WadFile::Get(int index)
	{
		return m_lumps[index];
	}
	Lump* WadFile::Get(char * name)
	{
		unsigned int size = (unsigned int) m_lumps.size();
		for (unsigned int i=0 ; i<size ; i++)
		{
			if (strncmp(name, m_lumps[i]->m_name, 8) == 0)
				return m_lumps[i];
		}
		return NULL;
	}

	void WadFile::SetLump(Lump * newLump)
	{
		Lump * oldLump = m_lumps[newLump->m_dictionary_position];
		if (oldLump != NULL && oldLump != newLump)
			delete oldLump;
		m_lumps[newLump->m_dictionary_position] = newLump;
	}

	int WadFile::Load()
	{
		if (m_lumps.size() != 0)
			return 0; //Already loaded

		char buffer[10];

		// Reading header
		if (4 != ReadString(buffer, 4))
			return 1;
		if (_strnicmp(buffer, "IWAD", 4) == 0)
			m_internal = true;
		else
			m_internal = false;

		// Reading lump count
		int lump_count;
		if (4 != ReadInt4(&lump_count))
			return 1;
		m_lumps.resize(lump_count);

		// Read directory position in file
		if (4 != ReadInt4(&m_directory_potision))
			return 1;

		// Load the dictionary
		if (LoadLumpDictionary() != 0)
			return 2;

		// Load the palettes
		m_palettes = GetLump((PlayPalLump*)Get("PLAYPAL"));
		m_palettes->Load();

		// Load the patches dictionary (sprites & texture pieces)
		m_patches =  GetLump((PatchesLump*)Get("PNAMES"));
		m_patches->Load();

		// Load the texture
		if (LoadTextures() != 0)
			return 4;

		// Search the lumps for levels and load them
		if (LoadLevels() != 0)
			return 3;

		return 0;
	}

	void WadFile::UnLoad()
	{
		m_lumps.clear();
		m_levels.clear();
		m_textures.clear();
		if (m_palettes != NULL)
		{
			delete m_palettes;
			m_palettes = NULL;
		}
		if (m_patches != NULL)
		{
			delete m_patches;
			m_patches = NULL;
		}
	}

	int WadFile::LoadLumpDictionary()
	{
		MoveTo(m_directory_potision);

		int lump_count = (int)m_lumps.size();
		char lump_name[9];
		int lump_pos, lump_size;
		for (int i=0 ; i<lump_count ; i++)
		{
			if (4 != ReadInt4(&lump_pos))
				return 1;
			if (4 != ReadInt4(&lump_size))
				return 1;
			memset(lump_name, 0, 9);
			if (8 != ReadString(lump_name, 8))
				return 1;
			Lump * lump = new Lump(this, i, lump_name, lump_pos, lump_size);
			m_lumps[i] = lump;
			printf("%08d + %08d %s\n", lump_pos, lump_size, lump_name);
		}
		return 0;
	}
	
	int WadFile::LoadLevels()
	{
		int lump_count = (int)m_lumps.size();
		LevelLump * current_level = NULL;
		for (int i=0 ; i<lump_count ; i++)
		{
			if (   m_lumps[i]->m_name[0] == 'E' && m_lumps[i]->m_name[2] == 'M'
				&& m_lumps[i]->m_name[1] >= '0' && m_lumps[i]->m_name[3] <= '9'
				&& strlen(m_lumps[i]->m_name) == 4)
			{
				// Get may return NULL
				current_level = GetLump((LevelLump*)m_lumps[i]);
				if (current_level == NULL)
					continue;
				if (0 == current_level->Load())
				{
					// Load successful
					m_levels.push_back(i);
				}
			}
		}
		return 0;
	}

	int WadFile::LoadTextures()
	{
		Lump * l = Get("TEXTURE1");
		if (l == NULL)
			return -1;

		MoveTo(l->m_position);

		int numTex;
		ReadInt4(&numTex);
		m_textures.resize(numTex);

		// Skip offset table
		Skip(numTex * 4);

		// Read texture maps
		for (int i=0 ; i<numTex ; i++)
		{
			Texture * tex = new Texture(this);
			m_textures[i] = tex;
			tex->Load();
		}

		return 0;
	}
	
	LevelLump * WadFile::GetLevel(unsigned int level_number)
	{
		if (level_number > m_levels.size())
			return NULL;
		else
		{
			LevelLump * result = GetLump((LevelLump*)Get(m_levels[level_number]));
			return result;
		}
	}
	
	Texture * WadFile::GetTexture(char * name)
	{
		for (unsigned int i=0 ; i<m_textures.size() ; i++)
		{
			Texture * tex = m_textures[i];
			if (strcmp(tex->m_name, name) == 0)
				return tex;
		}
		return NULL;
	}
};