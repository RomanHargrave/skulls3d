
#ifndef SK_LUMP
#define SK_LUMP

#include "../ReferencedObject.h"

namespace doom
{
	class WadFile;

	class Lump : public ReferencedObject
	{
	public:
		WadFile * m_wadfile;

		char * m_name;
		int m_dictionary_position;
		int m_position;
		int m_size;

		Lump(void *referencer, WadFile * wadfile, int dictionary_position, char * name, int pos, int size);
		Lump(void *referencer, Lump * lump);
		virtual ~Lump();

		virtual bool Load();
		virtual void UnLoad();
	};
};

#endif SK_LUMP