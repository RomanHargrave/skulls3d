
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>
#include <swegl/swegl.hpp>

#include "audio.h"
#include "display.h"
#include "input.h"
#include "mainmenu.h"
#include "gameplay.h"
#include "minimap.h"
#include "doom/Wad.h"

SDL_Surface * InitSDL();
void CloseSDL();

// Keyboard action are taken into account every KEYBOARD_RATE_MS milliseconds
#define KEYBOARD_RATE_MS 5

int HandleInput();


int main(int argc, char *argv[])
{
    SDL_Surface * screen = InitSDL();
	if (screen == nullptr) return -1;

	skulls::Wad doomwad(File("doom.wad"));

	//ShowMainMenu();
	PlayLevel(*doomwad.GetLevel(0), screen);
	
	/*
	while (1)
	{
		RefreshDebugDisplay(screen);
		if (int a=HandleInput() < 0)
			return -a;
	}
	*/

	CloseSDL();
	return 0;
}


SDL_Surface * InitSDL()
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


void CloseSDL()
{
	CloseAudio();
}