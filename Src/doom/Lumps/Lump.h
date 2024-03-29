
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
		int m_dictionary_position;
		int m_position;
		int m_size;

		Lump(WadFile * wadfile, int dictionary_position, char * name, int pos, int size);
		Lump(Lump * lump);
		virtual ~Lump();

		virtual bool Load();
		virtual void UnLoad();
	};
};

#endif SK_LUMP