
#ifndef SK_TEXTURELUMP
#define SK_TEXTURELUMP

namespace doom
{
	class WadFile;
	class PatchLump;

	class Texture
	{
	private:
		void MergePatchLumpIntoTexture(PatchLump * PatchLump, short orig_x, short orig_y);

	public:
		char * m_name;
		int m_position;
		unsigned int * m_bitmap;
		unsigned short m_w, m_h;
		WadFile * m_wadfile;

		Texture(WadFile * m_wadfile, int position);

		/**
		 * The file must come with its read pointer at the right position.
		 */
		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_TEXTURELUMP