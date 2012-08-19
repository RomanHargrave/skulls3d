
#ifndef SK_GAMEPLAY
#define SK_GAMEPLAY

#include <SDL/SDL.h>
#include <swegl/swegl.hpp>

#include "doom/Wad.h"
#include "doom/lumps/LevelLump.h"
#include "display.h"


void PlayLevel(skulls::Level & level, SDL_Surface * screen);
int HandleGameplayInput(SDL_Event event);

extern swegl::Camera *g_camera;

#endif // SK_GAMEPLAY