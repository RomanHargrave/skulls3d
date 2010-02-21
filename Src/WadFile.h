
#ifndef SK_WADFILE
#define SK_WADFILE

#include <vector>
#include "File.h"

namespace Doom
{
	class Lump;

	class WadFile : public File
	{
	private:
		std::vector<Lump> m_lumps;

	public:
		WadFile(const char * filename);

		virtual int Load();
		virtual void UnLoad();

		Lump& GetLump(int index);
	};
};

#endif SK_WADFILE