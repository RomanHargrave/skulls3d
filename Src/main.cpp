#include <stdlib.h>
#include <time.h>

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "display.h"
#include "input.h"
#include "doom/WadFile.h"
#include "doom/lumps/LevelLump.h"
#include "doom/lumps/PatchLump.h"
#include "doom/lumps/PlayPalLump.h"
#include "doom/lumps/FlatLump.h"
#include "doom/Thing.h"
#include "doom/Texture.h"

SDL_Surface *Init();

// Rendering parameters
doom::WadFile *g_doomwad = NULL;

// Keyboard action are taken into account every KEYBOARD_RATE_MS milliseconds
#define KEYBOARD_RATE_MS 5

int HandleInput();


int main(int argc, char *argv[])
{
    SDL_Surface *screen = Init();
	if (screen == NULL) return -1;

	g_doomwad = new doom::WadFile("doom.wad");
	if (g_doomwad == NULL)
		return -1;
	if (g_doomwad->Open() != 0)
		return -1;
	if (g_doomwad->Load() != 0)
		return -1;

	while (1)
	{
		RefreshDisplay(screen);
		if (int a=HandleInput() < 0)
			return -a;
	}
	return 0;
}


SDL_Surface *Init()
{
	InitKeyboardInput();

	SDL_Surface *screen;
	// Initialize SDL's subsystems - in this case, only video.
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	//SDL_WM_GrabInput(SDL_GRAB_ON);
	//SDL_ShowCursor(0);

	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);

	// Attempt to create a SCR_WIDTHxSCR_HEIGHT window with 32bit pixels.
	screen = SDL_SetVideoMode(g_scr_width, g_scr_height, 32, SDL_SWSURFACE);
	return screen;
}



int HandleInput()
{
	static int keystick = SDL_GetTicks();
	
	while ( (SDL_GetTicks())-keystick > KEYBOARD_RATE_MS ) // Limit the keyrate
	{
		if (g_keys['d'])
			g_x -= (int) (1/g_zoom);
		if (g_keys['a'])
			g_x += (int) (1/g_zoom);
		if (g_keys['w'])
			g_y += (int) (1/g_zoom);
		if (g_keys['s'])
			g_y -= (int) (1/g_zoom);
		if (g_keys['e'])
		{
			g_zoom *= 1.01f;
		}
		if (g_keys['q'])
		{
			g_zoom /= 1.01f;
		}

		keystick += KEYBOARD_RATE_MS;
	}

	return 0;
}