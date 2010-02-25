
#ifndef SK_WADFILE
#define SK_WADFILE

#include <vector>
#include "../File.h"

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
		void SetLump(Lump * newLump);

		template <class LUMPTYPE>
		LUMPTYPE* GetLump(LUMPTYPE * lump);
	};

	template<class LUMPTYPE>
	LUMPTYPE * WadFile::GetLump(LUMPTYPE * l)
	{
		Lump * lump = l;
		LUMPTYPE * result = dynamic_cast<LUMPTYPE*>(lump);
		if (result == NULL)
		{
			// Lump has not been transformed yet, transform it now
			result = new LUMPTYPE(lump);
			// Update the dictionary reference and dispose of the old lump
			result->m_wadfile->SetLump(result); // deletes the old one automatically
		}

		// Debug:
		char * debugStr = result->ToString();
		if (debugStr[0] != 0)
			printf("Found this lump : %s\n", debugStr);

		return result;
	}
};

#endif SK_WADFILE