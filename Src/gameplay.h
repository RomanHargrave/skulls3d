
#ifndef SK_GAMEPLAY
#define SK_GAMEPLAY

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "doom/WadFile.h"
#include "doom/lumps/LevelLump.h"
#include "display.h"
#include "Camera.h"

void PlayLevel(doom::LevelLump * level);
int HandleGameplayInput(SDL_Event event);

extern Camera *g_camera;

#endif // SK_GAMEPLAY