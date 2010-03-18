
#ifndef SK_GAMEPLAY
#define SK_GAMEPLAY

#include "doom/WadFile.h"
#include "doom/lumps/LevelLump.h"
#include "display.h"
#include "Camera.h"

void PlayLevel(doom::LevelLump * level);
int HandleGameplayInput();

extern Camera *g_camera;

#endif // SK_GAMEPLAY