
#ifndef SK_TEXTURELUMP
#define SK_TEXTURELUMP

namespace doom
{
	class Texture
	{
	public:
		int * m_bitmap;

		Texture();

		virtual int Load();
		virtual void UnLoad();
	};
};

#endif // SK_TEXTURELUMP