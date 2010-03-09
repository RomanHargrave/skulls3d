
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

static doom::PatchLump * gs_newGame = NULL;
static doom::PatchLump * gs_options = NULL;
static doom::PatchLump * gs_readThis = NULL;
static doom::PatchLump * gs_quitGame = NULL;

int HandleMenuInput();
void ShowTopMenu();

void ShowMainMenu()
{
	Rect bckg_dimensions;

	gs_show_menu = false;

	gs_titleBackg = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("TITLEPIC"));
	if (gs_titleBackg == NULL) goto mainMenuEnd;
	if (gs_titleBackg->Load() != 0) goto mainMenuEnd;

	gs_logo = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_DOOM"));
	if (gs_logo == NULL) goto mainMenuEnd;
	if (gs_logo->Load() != 0) goto mainMenuEnd;

	gs_newGame = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_NGAME"));
	if (gs_newGame == NULL) goto mainMenuEnd;
	if (gs_newGame->Load() != 0) goto mainMenuEnd;

	gs_options = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_OPTION"));
	if (gs_options == NULL) goto mainMenuEnd;
	if (gs_options->Load() != 0) goto mainMenuEnd;

	gs_readThis = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_RDTHIS"));
	if (gs_readThis == NULL) goto mainMenuEnd;
	if (gs_readThis->Load() != 0) goto mainMenuEnd;

	gs_quitGame = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_QUITG"));
	if (gs_quitGame == NULL) goto mainMenuEnd;
	if (gs_quitGame->Load() != 0) goto mainMenuEnd;

	bckg_dimensions.x = 0;
	bckg_dimensions.y = 0;
	bckg_dimensions.w = gs_titleBackg->m_w;
	bckg_dimensions.h = gs_titleBackg->m_h;
	Draw_320x200(g_screen, gs_titleBackg->m_bitmap, bckg_dimensions, bckg_dimensions, bckg_dimensions, 255);

	while (1)
	{
		if (HandleMenuInput() < 0)
			goto mainMenuEnd;
	}

mainMenuEnd:
	if (gs_titleBackg != NULL) gs_titleBackg->UnLoad();
	if (gs_logo != NULL) gs_logo->UnLoad();
	if (gs_newGame != NULL) gs_newGame->UnLoad();
	if (gs_options != NULL) gs_options->UnLoad();
	if (gs_readThis != NULL) gs_readThis->UnLoad();
	if (gs_quitGame != NULL) gs_quitGame->UnLoad();
}

int HandleMenuInput()
{
	SDL_Event event;
	Rect bckg_dimensions = {0, 0, gs_titleBackg->m_w, gs_titleBackg->m_h};
	Rect logo_dimensions = {0, 0, gs_logo->m_w, gs_logo->m_h};
	Rect logo_screen_where = {320/2 - gs_logo->m_w/2, 3, 0, 0};


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
						Draw_320x200(g_screen, gs_titleBackg->m_bitmap, bckg_dimensions, bckg_dimensions, bckg_dimensions, 128);
						Draw_320x200(g_screen, gs_logo->m_bitmap, logo_dimensions, logo_dimensions, logo_screen_where, (unsigned char)255);
						ShowTopMenu();
						gs_show_menu = true;
					}
					else
					{
						Draw_320x200(g_screen, gs_titleBackg->m_bitmap, bckg_dimensions, bckg_dimensions, bckg_dimensions, 255);
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

void ShowTopMenu()
{
	Rect screen_where = {320/2 - gs_logo->m_w/2, 70, 0, 0};

	Rect newGame_dimensions = {0, 0, gs_newGame->m_w, gs_newGame->m_h};
	Draw_320x200(g_screen, gs_newGame->m_bitmap, newGame_dimensions, newGame_dimensions, screen_where, (unsigned char)255);
	
	screen_where.y = 90;
	Rect options_dimensions = {0, 0, gs_options->m_w, gs_options->m_h};
	Draw_320x200(g_screen, gs_options->m_bitmap, options_dimensions, options_dimensions, screen_where, (unsigned char)255);
	
	screen_where.y = 110;
	Rect readThis_dimensions = {0, 0, gs_readThis->m_w, gs_readThis->m_h};
	Draw_320x200(g_screen, gs_readThis->m_bitmap, readThis_dimensions, readThis_dimensions, screen_where, (unsigned char)255);
	
	screen_where.y = 130;
	Rect quitGame_dimensions = {0, 0, gs_quitGame->m_w, gs_quitGame->m_h};
	Draw_320x200(g_screen, gs_quitGame->m_bitmap, quitGame_dimensions, quitGame_dimensions, screen_where, (unsigned char)255);
}