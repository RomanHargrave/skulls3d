#include <stdlib.h>
#include <time.h>

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "audio.h"
#include "display.h"
#include "input.h"
#include "mainmenu.h"
#include "minimap.h"
#include "doom/WadFile.h"

SDL_Surface *Init();

// Rendering parameters
doom::WadFile *g_doomwad = NULL;
SDL_Surface *g_screen;

// Keyboard action are taken into account every KEYBOARD_RATE_MS milliseconds
#define KEYBOARD_RATE_MS 5

int HandleInput();


int main(int argc, char *argv[])
{
    g_screen = Init();
	if (g_screen == NULL) return -1;

	g_doomwad = new doom::WadFile("doom.wad");
	if (g_doomwad == NULL)
		return -1;
	if (g_doomwad->Open() != 0)
		return -1;
	if (g_doomwad->Load() != 0)
		return -1;

	ShowMainMenu();
	
	/*
	while (1)
	{
		RefreshDebugDisplay(screen);
		if (int a=HandleInput() < 0)
			return -a;
	}
	*/

	CloseAudio();
	return 0;
}


SDL_Surface *Init()
{
	InitKeyboardInput();
	InitAudio();

	SDL_Surface *screen;
	// Initialize SDL's subsystems - in this case, only video.
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(0);

	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);

	// Attempt to create a SCR_WIDTHxSCR_HEIGHT window with 32bit pixels.
	screen = SDL_SetVideoMode(g_scr_w, g_scr_h, 32, SDL_SWSURFACE);
	return screen;
}


