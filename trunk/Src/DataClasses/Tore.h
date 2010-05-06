
#ifndef SWE_TORE
#define SWE_TORE

#include "Mesh.h"
//#include "Tex.h"

class Tex;

class Tore : public Mesh
{
public:
	Tore(unsigned int precision, Tex *t);
};

#endif // SWE_TORE
