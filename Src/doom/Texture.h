
#ifndef SK_TEXTURELUMP
#define SK_TEXTURELUMP

#include "..\File.h"
#include "Patches.h"

namespace skulls
{
	class Wad;
	class PatchLump;

	class Texture
	{
	public:
		Texture(File & file, int position, Patches & patches);
		~Texture();
	
		inline const std::string & GetName() const { return name; }
		inline unsigned int * GetBitmap() const { return m_bitmap; }
		inline const unsigned short GetWidth() const { return m_w; }
		inline const unsigned short GetHeight() const { return m_h; }

	private:
		void MergePatchLumpIntoTexture(std::shared_ptr<Patch> & patch, short orig_x, short orig_y);
	
	private:
		std::string name;
		unsigned int * m_bitmap;
		unsigned short m_w, m_h;
	};
};

#endif // SK_TEXTURELUMP