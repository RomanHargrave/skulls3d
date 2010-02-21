#include <stdlib.h>

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "WadFile.h"

int main(int argc, char *argv[])
{
	doom::WadFile doomwad("data\\doom.wad");
	if (doomwad.Open() != 0)
		return -1;
	if (doomwad.Load() != 0)
		return -1;

	return 0;
}