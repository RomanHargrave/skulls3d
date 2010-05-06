
#ifndef SWE_R001DOTS
#define SWE_R001DOTS

#include "R000Virtual.h"

class R001Dots : public R000Virtual
{
public:
	R001Dots(Scene *scene, Camera *camera, ViewPort *viewport);

	void Render();
};

#endif // SWE_R001DOTS
