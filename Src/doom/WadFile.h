
#ifndef SK_WADFILE
#define SK_WADFILE

#include <vector>
#include "../File.h"

namespace doom
{
	class Lump;
	class LevelLump;
	class PatchesLump;
	class PlayPalLump;
	class Texture;

	class WadFile : public File
	{
	private:
		bool m_internal; // true: IWAD (Internal wad), false: PWAD (Patch wad)
		int m_directory_potision; // offset in the file
		std::vector<Lump*> m_lumps;
		std::vector<int> m_levels;
		std::vector<Texture*> m_textures;

		int LoadLumpDictionary();
		int LoadLevels();
		int LoadTextures();

	protected:
		void SetLump(Lump * newLump);

	public:
		PlayPalLump * m_palettes;
		PatchesLump * m_patches;

		WadFile(const char * filename);
		virtual ~WadFile();

		/**
		 * Loading a wad file does not load all its data in memory, it prepares
		 * the data structure so that it is ready to load relevant data like levels.
		 * Loading a level will make the wadfile load the related data.
		 * 0: Success
		 * 1: Unknown error
		 * 2: Error reading dictionary
		 * 3: Error reading levels
		 */
		virtual int Load();
		virtual void UnLoad();

		LevelLump * GetLevel(unsigned int level_number);
		Texture * GetTexture(char * name);

		Lump* Get(int index);
		Lump* Get(char * name);

		template <class LUMPTYPE>
		LUMPTYPE* GetLump(LUMPTYPE * lump);
	};

	template<class LUMPTYPE>
	LUMPTYPE * WadFile::GetLump(LUMPTYPE * l)
	{
		if (l == NULL)
			return NULL;
		
		// l may not be of LUMPTYPE yet, check if it is
		Lump * lump = l;
		LUMPTYPE * result = dynamic_cast<LUMPTYPE*>(lump);
		if (result == NULL)
		{
			// Lump has not been transformed yet, transform it now
			result = new LUMPTYPE(lump);
			// Update the dictionary reference and dispose of the old lump
			result->m_wadfile->SetLump(result); // deletes the old one automatically
		}

		return result;
	}
};

#endif SK_WADFILE