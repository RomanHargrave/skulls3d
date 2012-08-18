
#include <stdlib.h>
#include "../File.h"
#include "WadFile.h"
#include "Texture.h"
#include "Lumps/Lump.h"
#include "lumps/PatchLump.h"
#include "Lumps/LevelLump.h"
#include "Lumps/PlayPalLump.h"

namespace doom
{
	WadFile::WadFile(const char * filename)
		: File(filename)
	{
		m_levels.resize(0);
		m_lumps.resize(0);
		m_textures.resize(0);
		m_palettes = NULL;
		m_patches.resize(0);
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
			if (_strnicmp(name, m_lumps[i]->m_name, 8) == 0)
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

	bool WadFile::Load()
	{
		if (m_lumps.size() != 0)
			return true; //Already loaded

		char buffer[10];

		// Reading header
		if (4 != ReadString(buffer, 4))
			return false;
		if (_strnicmp(buffer, "IWAD", 4) == 0)
			m_internal = true;
		else
			m_internal = false;

		// Reading lump count
		int lump_count;
		if (4 != ReadInt4(&lump_count))
			return false;
		m_lumps.resize(lump_count);

		// Read directory position in file
		if (4 != ReadInt4(&m_directory_potision))
			return false;

		// Load the dictionary
		if (LoadLumpDictionary() != 0)
			return false;

		// Load the palettes
		m_palettes = GetLump((PlayPalLump*)Get("PLAYPAL"));
		m_palettes->Load();

		// Load the PatchLumpes dictionary (sprites & texture pieces)
		Lump *l = Get("PNAMES");
		MoveTo(l->m_position);
		unsigned int count;
		ReadInt4((int*)&count);
		m_patches.resize(count);
		for (unsigned int j=0 ; j<count ; j++)
		{
			char PatchLumpName[9];
			memset(PatchLumpName, 0, 9);
			ReadString(PatchLumpName, 8);
			int pos = GetPos();
			l = Get(PatchLumpName);
			MoveTo(l->m_position);
			m_patches[j] = GetLump((PatchLump*)Get(PatchLumpName));
			MoveTo(pos);
		}


		// Load the texture dictionary
		m_textures.resize(0);
		if (LoadTextures("TEXTURE1") != 0)
			return false;
		if (LoadTextures("TEXTURE2") != 0)
			return false;

		// Fill the levels dictionary
		if (LoadLevels() != 0)
			return false;

		return true;
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
		m_patches.clear();
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
			//printf("%08d + %08d %s\n", lump_pos, lump_size, lump_name);
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
				m_levels.push_back(i);
			}
		}
		return 0;
	}

	int WadFile::LoadTextures(char * lumpName)
	{
		Lump * l = Get(lumpName);
		if (l == NULL)
			return -1;

		MoveTo(l->m_position);

		int oldSize = m_textures.size();
		int numTex;
		ReadInt4(&numTex);
		m_textures.resize(numTex+oldSize);

		// Read texture maps
		for (int i=0 ; i<numTex ; i++)
		{
			int offset;
			ReadInt4(&offset);
			// offset is relative to the beginning of the lump
			// From the position of the lump in the file we have
			// to skip the number of textures (4 bytes) and the
			// offset table (4 bytes per texture)
			offset += l->m_position/* + 4 + 4*numTex*/;

			Texture * tex = new Texture(this, offset);
			m_textures[i+oldSize] = tex;
			//tex->Load();
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
	PatchLump * WadFile::GetPatch(char *name)
	{
		for (unsigned int i=0 ; i<m_patches.size() ; i++)
		{
			PatchLump * PatchLump = m_patches[i];
			if (strcmp(PatchLump->m_name, name) == 0)
				return PatchLump;
		}
		return NULL;
	}
};