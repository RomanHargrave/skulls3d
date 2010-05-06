
#ifndef SWE_CUBE
#define SWE_CUBE

#include "Mesh.h"
#include "Tex.h"

class Cube : public Mesh
{
public:
	Cube(float size, Tex *t);
};

#endif // SWE_CUBE
