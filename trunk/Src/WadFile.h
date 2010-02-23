
#ifndef SK_WADFILE
#define SK_WADFILE

#include <vector>
#include "File.h"

namespace doom
{
	class Lump;

	class WadFile : public File
	{
	private:
		bool m_internal; // true: IWAD (Internal wad), false: PWAD (Patch wad)
		int m_directory_potision; // offset in the file
		std::vector<Lump*> m_lumps;

	public:
		WadFile(const char * filename);
		virtual ~WadFile();

		/**
		 * 0: Success
		 * 1: Unknown error
		 * 2: Error reading dictionary
		 * 3: Error reading levels
		 */
		virtual int Load();
		virtual void UnLoad();

		int ReadLumpDictionary();
		int ReadLevels();

		Lump* GetLump(int index);
		void SetLump(Lump * lump);
	};
};

#endif SK_WADFILE