
#ifndef SK_Sound
#define SK_Sound

#include <vector>
#include "Lump.h"
#include "..\..\File.h"

namespace skulls
{
	class Sound
	{
	public:
		Sound(File & file, Lump & lump);
		
	public:
		std::vector<unsigned char> m_soundData;
	};
};

#endif // SK_Sound