
#include "Tore.h"
#include "Tex.h"
#include "Vec3f.h"
#include "Vec2f.h"
#include "Matrix4x4.h"

Tore::Tore(unsigned int precision, Tex *t)
{
	Mesh::Mesh();

	this->m_texture = t;
	this->m_vertexcount = precision*precision;
	this->m_vb = new CUSTOMVERTEX[this->m_vertexcount];

	float angle = (2*3.141592653589f) / precision;
	Matrix4x4 bigg, ssmall;
	bigg.SetIdentity();
	ssmall.SetIdentity();

	bigg.Translate(2.0f, 0.0f, 0.0f);
	ssmall.Translate(0.8f, 0.0f, 0.0f);

	for (unsigned int bg=0 ; bg<precision ; bg++)
	{
		for (unsigned int sm=0 ; sm<precision ; sm++)
		{
			Vec3f v;
			v = ssmall * v;
			v = bigg * v;
			m_vb[bg*precision + sm].x = v.x;
			m_vb[bg*precision + sm].y = v.y;
			m_vb[bg*precision + sm].z = v.z;
			ssmall.RotateZ(angle);
		}
		bigg.RotateY(angle);
	}

	for (unsigned int bg=0 ; bg<precision ; bg++)
	{
		unsigned int *ib = new unsigned int[precision*2 + 2];
		Vec2f *tb = new Vec2f[3*precision*2];
		ib[0] = bg*precision;
		ib[1] = ((bg+1)%precision)*precision;
		for (unsigned int i=1 ; i<=precision ; i++)
		{
			ib[i*2]   = bg*precision + (i%precision);
			ib[i*2+1] = ((bg+1)%precision)*precision + (i%precision);

			tb[(i-1)*6].x = (t->m_mipmaps[0].m_width*1.0f / precision) * bg;
			tb[(i-1)*6].y = (t->m_mipmaps[0].m_height*1.0f / precision) * (i-1);
			tb[(i-1)*6+1].x = (t->m_mipmaps[0].m_width*1.0f / precision) * (bg+1);
			tb[(i-1)*6+1].y = (t->m_mipmaps[0].m_height*1.0f / precision) * (i-1);
			tb[(i-1)*6+2].x = (t->m_mipmaps[0].m_width*1.0f / precision) * bg;
			tb[(i-1)*6+2].y = (t->m_mipmaps[0].m_height*1.0f / precision) * i;
			
			tb[(i-1)*6+3].x = (t->m_mipmaps[0].m_width*1.0f / precision) * (bg+1);
			tb[(i-1)*6+3].y = (t->m_mipmaps[0].m_height*1.0f / precision) * (i-1);
			tb[(i-1)*6+4].x = (t->m_mipmaps[0].m_width*1.0f / precision) * bg;
			tb[(i-1)*6+4].y = (t->m_mipmaps[0].m_height*1.0f / precision) * i;
			tb[(i-1)*6+5].x = (t->m_mipmaps[0].m_width*1.0f / precision) * (bg+1);
			tb[(i-1)*6+5].y = (t->m_mipmaps[0].m_height*1.0f / precision) * i;
			
		}
		this->AddStrip(ib, tb, precision*2 + 2);
		delete[] ib;
		delete[] tb;
	}
}
