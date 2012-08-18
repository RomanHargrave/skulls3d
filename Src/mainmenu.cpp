
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include <windows.h>
#include "mainmenu.h"
#include "input.h"
#include "display.h"
#include "audio.h"
#include "gameplay.h"
#include "doom/WadFile.h"
#include "doom/lumps/PatchLump.h"
#include "doom/lumps/SoundLump.h"

// in main.cpp
extern doom::WadFile *g_doomwad;
extern SDL_Surface * g_screen;

static doom::PatchLump * gs_titleBackg = NULL;
static doom::PatchLump * gs_logo = NULL;

static doom::PatchLump * gs_skull1 = NULL;
static doom::PatchLump * gs_skull2 = NULL;

static doom::PatchLump * gs_newGame = NULL;
static doom::PatchLump * gs_options = NULL;
static doom::PatchLump * gs_readThis = NULL;
static doom::PatchLump * gs_quitGame = NULL;

static doom::SoundLump *gs_menuEnter = NULL;
static doom::SoundLump *gs_menuExit = NULL;
static doom::SoundLump *gs_menuMove = NULL;

enum MenuTypes
{
	NoMenu = 0,
	TopMenu,
	MenuCount,
};

static unsigned int gs_whichMenu = NoMenu;
static unsigned int gs_topMenuPos = 0;
static bool gs_skullLigthenUp = false;

int HandleMenuInput();
void DrawSplashScreen();
void DrawTopMenu();
void EraseMenuCursor(int pos);
void DrawMenuCursor(int pos);

void ShowMainMenu()
{
	gs_whichMenu = NoMenu;
	gs_skullLigthenUp = ((GetTickCount()>8) & 0x01) == 1;

	gs_titleBackg = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("TITLEPIC"));
	if (gs_titleBackg == NULL) goto mainMenuEnd;
	if (gs_titleBackg->Load() != true) goto mainMenuEnd;

	gs_logo = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_DOOM"));
	if (gs_logo == NULL) goto mainMenuEnd;
	if (gs_logo->Load() != true) goto mainMenuEnd;

	gs_skull1 = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_SKULL1"));
	if (gs_skull1 == NULL) goto mainMenuEnd;
	if (gs_skull1->Load() != true) goto mainMenuEnd;

	gs_skull2 = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_SKULL2"));
	if (gs_skull2 == NULL) goto mainMenuEnd;
	if (gs_skull2->Load() != true) goto mainMenuEnd;

	gs_newGame = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_NGAME"));
	if (gs_newGame == NULL) goto mainMenuEnd;
	if (gs_newGame->Load() != true) goto mainMenuEnd;

	gs_options = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_OPTION"));
	if (gs_options == NULL) goto mainMenuEnd;
	if (gs_options->Load() != true) goto mainMenuEnd;

	gs_readThis = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_RDTHIS"));
	if (gs_readThis == NULL) goto mainMenuEnd;
	if (gs_readThis->Load() != true) goto mainMenuEnd;

	gs_quitGame = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("M_QUITG"));
	if (gs_quitGame == NULL) goto mainMenuEnd;
	if (gs_quitGame->Load() != true) goto mainMenuEnd;

	gs_menuEnter = g_doomwad->GetLump((doom::SoundLump*)g_doomwad->Get("DSSWTCHN"));
	if (gs_menuEnter == NULL) goto mainMenuEnd;
	if (gs_menuEnter->Load() != true) goto mainMenuEnd;

	gs_menuExit = g_doomwad->GetLump((doom::SoundLump*)g_doomwad->Get("DSSWTCHX"));
	if (gs_menuExit == NULL) goto mainMenuEnd;
	if (gs_menuExit->Load() != true) goto mainMenuEnd;

	gs_menuMove = g_doomwad->GetLump((doom::SoundLump*)g_doomwad->Get("DSSTNMOV"));
	if (gs_menuMove == NULL) goto mainMenuEnd;
	if (gs_menuMove->Load() != true) goto mainMenuEnd;

	DrawSplashScreen();

	while (1)
	{
		if (HandleMenuInput() < 0)
			break;
	}

mainMenuEnd:
	if (gs_menuEnter != NULL) gs_menuEnter->UnLoad();
	if (gs_menuExit != NULL) gs_menuExit->UnLoad();
	if (gs_menuMove != NULL) gs_menuMove->UnLoad();
}

int HandleMenuInput()
{
	SDL_Event event;

	// Poll for events, and handle the ones we care about.
	while(true)
	{
		if (gs_skullLigthenUp != (((GetTickCount()>>8) & 0x01) == 1))
		{
			if (gs_whichMenu == TopMenu)
				DrawMenuCursor(gs_topMenuPos);
		}
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
				case SDL_KEYDOWN:
					// If escape is pressed, return (and thus, quit)
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						if (gs_whichMenu == NoMenu) {
							SKPlaySound(gs_menuEnter->m_soundData, gs_menuEnter->m_len);
							DrawTopMenu();
						} else {
							SKPlaySound(gs_menuExit->m_soundData, gs_menuExit->m_len);
							DrawSplashScreen();
						}
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						if (gs_whichMenu == NoMenu) {
							SKPlaySound(gs_menuEnter->m_soundData, gs_menuEnter->m_len);
							DrawTopMenu();
						} else if (gs_whichMenu == TopMenu) {
							SKPlaySound(gs_menuMove->m_soundData, gs_menuMove->m_len);
							EraseMenuCursor(gs_topMenuPos);
							gs_topMenuPos = (gs_topMenuPos-1) % 4;
							DrawMenuCursor(gs_topMenuPos);
						}
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						if (gs_whichMenu == NoMenu) {
							SKPlaySound(gs_menuEnter->m_soundData, gs_menuEnter->m_len);
							DrawTopMenu();
						} else if (gs_whichMenu == TopMenu) {
							SKPlaySound(gs_menuMove->m_soundData, gs_menuMove->m_len);
							EraseMenuCursor(gs_topMenuPos);
							gs_topMenuPos = (gs_topMenuPos+1) % 4;
							DrawMenuCursor(gs_topMenuPos);
						}
					}
					else if (event.key.keysym.sym == SDLK_RETURN)
					{
						if (gs_whichMenu == TopMenu)
						{
							if (gs_topMenuPos == 0)
							{
								PlayLevel(g_doomwad->GetLevel(0));
								SKPlaySound(gs_menuEnter->m_soundData, gs_menuEnter->m_len);
								DrawTopMenu();
								DrawMenuCursor(gs_topMenuPos);
							}
							else if (gs_topMenuPos == 3)
							{
								return -1;
							}
						}
					}
					break;
				case SDL_QUIT:
					return -3;
			}
		}
		Sleep(1);
	}
	return 0;
}

void DrawSplashScreen()
{
	gs_whichMenu = NoMenu;
	Rect bckg_dimensions = {0, 0, gs_titleBackg->m_w, gs_titleBackg->m_h};
	Draw_320x200(g_screen, gs_titleBackg->m_bitmap, bckg_dimensions, bckg_dimensions, bckg_dimensions, 255);
}

void DrawTopMenu()
{	
	gs_whichMenu = TopMenu;

	Rect bckg_dimensions = {0, 0, gs_titleBackg->m_w, gs_titleBackg->m_h};
	Rect logo_dimensions = {0, 0, gs_logo->m_w, gs_logo->m_h};
	Rect logo_screen_where = {320/2 - gs_logo->m_w/2, 3, 0, 0};

	Draw_320x200(g_screen, gs_titleBackg->m_bitmap, bckg_dimensions, bckg_dimensions, bckg_dimensions, 128);
	Draw_320x200(g_screen, gs_logo->m_bitmap, logo_dimensions, logo_dimensions, logo_screen_where, (unsigned char)255);

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

	DrawMenuCursor(gs_topMenuPos);
}

void EraseMenuCursor(int pos)
{
	Rect bckg_dimensions = {0, 0, gs_titleBackg->m_w, gs_titleBackg->m_h};
	Rect skull1_dimensions = {320/2 - gs_logo->m_w/2-30, 70+20*pos, gs_skull1->m_w, gs_skull1->m_h};
	Draw_320x200(g_screen, gs_titleBackg->m_bitmap, bckg_dimensions, skull1_dimensions, skull1_dimensions, (unsigned char)128);
}
void DrawMenuCursor(int pos)
{
	gs_skullLigthenUp = ((GetTickCount()>>8) & 0x01) == 1;
	Rect screen_where = {320/2 - gs_logo->m_w/2-30, 70+20*pos, 0, 0};
	Rect skull1_dimensions = {0, 0, gs_skull1->m_w, gs_skull1->m_h};
	unsigned int * bitmap = gs_skullLigthenUp?gs_skull2->m_bitmap:gs_skull1->m_bitmap;
	Draw_320x200(g_screen, bitmap, skull1_dimensions, skull1_dimensions, screen_where, (unsigned char)255);
}
