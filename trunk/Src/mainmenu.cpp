
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "mainmenu.h"
#include "input.h"
#include "display.h"
#include "doom/WadFile.h"
#include "doom/lumps/PatchLump.h"

// in main.cpp
extern doom::WadFile *g_doomwad;
extern SDL_Surface * g_screen;

static int gs_selected_item = 0;
static bool gs_show_menu = false;
static doom::PatchLump * gs_titleBackg = NULL;
static doom::PatchLump * gs_logo = NULL;

int HandleMenuInput();

void ShowMainMenu()
{
	gs_titleBackg = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("TITLEPIC"));
	if (gs_titleBackg == NULL)
		return;
	if (gs_titleBackg->Load() != 0)
		return;

	gs_logo = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_DOOM"));
	if (gs_logo == NULL)
		return;
	if (gs_logo->Load() != 0)
		return;

	DrawBackGround(g_screen, gs_titleBackg->m_bitmap, gs_titleBackg->m_w, gs_titleBackg->m_h, (unsigned char)255);
	
	Rect logo_dimensions = {0, 0, gs_logo->m_w, gs_logo->m_h};
	Rect screen_where = {320/2 - gs_logo->m_w/2, 5, 0, 0};
	Draw(g_screen, gs_logo->m_bitmap, logo_dimensions, logo_dimensions, screen_where, (unsigned char)255);

	while (1)
	{
		if (HandleMenuInput() < 0)
			return;
	}
}

int HandleMenuInput()
{
	SDL_Event event;

	// Poll for events, and handle the ones we care about.
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type) 
		{
			case SDL_MOUSEMOTION:
				break;

			case SDL_KEYDOWN:
				// If escape is pressed, return (and thus, quit)
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					if (gs_show_menu == false)
					{
						DrawBackGround(g_screen, gs_titleBackg->m_bitmap, gs_titleBackg->m_w, gs_titleBackg->m_h, (unsigned char)128);
						gs_show_menu = true;
					}
					else
					{
						DrawBackGround(g_screen, gs_titleBackg->m_bitmap, gs_titleBackg->m_w, gs_titleBackg->m_h, (unsigned char)255);
						gs_show_menu = false;
					}
				}
				else if (event.key.keysym.sym == SDLK_UP)
				{
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
				}
				else if (event.key.keysym.sym == SDLK_RETURN)
				{
				}
				break;
				
			case SDL_KEYUP:
				// If escape is pressed, return (and thus, quit)
				if (event.key.keysym.sym == SDLK_UP)
				{
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
				}
				break;
			case SDL_QUIT:
				return -3;
		}
	}
	return 0;
}