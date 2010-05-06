
#ifndef SWE_TEXTURE
#define SWE_TEXTURE

#define MIPMAPS_COUNT 5

typedef struct
{
	unsigned int *m_bitmap;
	unsigned int m_width;
	unsigned int m_height;
	
}Mipmap;

class Tex
{	
public:
	int m_mipmapsCount;
	Mipmap *m_mipmaps;

	Tex(char *filename);
	Tex(unsigned int rgb);
	Tex(unsigned * data, int w, int h);
};

#endif // SWE_TEXTURE