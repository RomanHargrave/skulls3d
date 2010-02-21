
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
		char * m_data;

		Lump(WadFile * wadfile, char * name, int pos, int size);
		~Lump();

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif SK_LUMP