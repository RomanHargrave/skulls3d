
#ifndef SK_LUMP
#define SK_LUMP

namespace doom
{
	class WadFile;

	class Lump
	{
	public:
		WadFile * m_wadfile;

		char * m_name;
		int m_position;
		int m_size;

		Lump(WadFile * wadfile, char * name, int pos, int size);
		~Lump();

		virtual void Load() = 0;
		virtual void UnLoad() = 0;
	};
};

#endif SK_LUMP