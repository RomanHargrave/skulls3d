
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

		/**
		 * 0: Success
		 * 1: Unknown error
		 */
		virtual int Load();
		virtual void UnLoad();

		int ReadLumpDictionary();

		Lump* GetLump(int index);
	};
};

#endif SK_WADFILE