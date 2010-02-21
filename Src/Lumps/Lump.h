
#ifndef SK_LUMP
#define SK_LUMP

namespace Doom
{
	class WadFile;

	class Lump
	{
	public:
		WadFile * m_wadfile;

		char * m_name;
		int m_position;
		int m_size;

		char * m_data; // NULL until lump is loaded

		Lump(WadFile * wadfile, char * name, int pos, int size);
		~Lump();

		virtual void Load(); // Reads m_size bytes of m_wadfile at m_position and stores it m_data
		virtual void UnLoad(); // Destroys m_data but keeps the other data for reLoading
	};
};

#endif SK_LUMP