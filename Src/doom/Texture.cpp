
#include <stdlib.h>
#include "Texture.h"
#include "WadFile.h"

namespace doom
{
	Texture::Texture()
	{
		m_bitmap = NULL;
	}

	int Texture::Load()
	{

		return 0;
	}
	void Texture::UnLoad()
	{
		if (m_bitmap != NULL)
		{
			delete m_bitmap;
			m_bitmap = NULL;
		}
	}
};