

#include <windows.h>
#include <SDL/SDL.h>
#include "mainmenu.h"
#include "input.h"
#include "display.h"
#include "audio.h"
#include "gameplay.h"
#include "doom/Wad.h"
#include "doom/lumps/PatchLump.h"
#include "doom/lumps/SoundLump.h"
#include "doom/Lumps\PatchLump.h"


static skulls::Patch * gs_titleBackg = NULL;
static skulls::Patch * gs_logo = NULL;

static skulls::Patch * gs_skull1 = NULL;
static skulls::Patch * gs_skull2 = NULL;
					 
static skulls::Patch * gs_newGame = NULL;
static skulls::Patch * gs_options = NULL;
static skulls::Patch * gs_readThis = NULL;
static skulls::Patch * gs_quitGame = NULL;

static skulls::Sound *gs_menuEnter = NULL;
static skulls::Sound *gs_menuExit = NULL;
static skulls::Sound *gs_menuMove = NULL;

enum MenuTypes
{
	NoMenu = 0,
	TopMenu,
	MenuCount,
};

static unsigned int gs_whichMenu = NoMenu;
static unsigned int gs_topMenuPos = 0;
static bool gs_skullLigthenUp = false;

int HandleMenuInput(skulls::Wad & wad, SDL_Surface * screen);
void DrawSplashScreen(SDL_Surface * screen);
void DrawTopMenu(SDL_Surface * screen);
void EraseMenuCursor(SDL_Surface * screen, int pos);
void DrawMenuCursor(SDL_Surface * screen, int pos);

/*
void ShowMainMenu(skulls::Wad & wad)
{
	gs_whichMenu = NoMenu;
	gs_skullLigthenUp = ((GetTickCount()>8) & 0x01) == 1;

	gs_titleBackg = wad.GetLump((skulls::PatchLump*)wad.Get("TITLEPIC"));
	if (gs_titleBackg == NULL) goto mainMenuEnd;
	if (gs_titleBackg->Load() != true) goto mainMenuEnd;

	gs_logo = wad.GetLump((skulls::PatchLump*)wad.Get("M_DOOM"));
	if (gs_logo == NULL) goto mainMenuEnd;
	if (gs_logo->Load() != true) goto mainMenuEnd;

	gs_skull1 = wad.GetLump((skulls::PatchLump*)wad.Get("M_SKULL1"));
	if (gs_skull1 == NULL) goto mainMenuEnd;
	if (gs_skull1->Load() != true) goto mainMenuEnd;

	gs_skull2 = wad.GetLump((skulls::PatchLump*)wad.Get("M_SKULL2"));
	if (gs_skull2 == NULL) goto mainMenuEnd;
	if (gs_skull2->Load() != true) goto mainMenuEnd;

	gs_newGame = wad.GetLump((skulls::PatchLump*)wad.Get("M_NGAME"));
	if (gs_newGame == NULL) goto mainMenuEnd;
	if (gs_newGame->Load() != true) goto mainMenuEnd;

	gs_options = wad.GetLump((skulls::PatchLump*)wad.Get("M_OPTION"));
	if (gs_options == NULL) goto mainMenuEnd;
	if (gs_options->Load() != true) goto mainMenuEnd;

	gs_readThis = wad.GetLump((skulls::PatchLump*)wad.Get("M_RDTHIS"));
	if (gs_readThis == NULL) goto mainMenuEnd;
	if (gs_readThis->Load() != true) goto mainMenuEnd;

	gs_quitGame = wad.GetLump((skulls::PatchLump*)wad.Get("M_QUITG"));
	if (gs_quitGame == NULL) goto mainMenuEnd;
	if (gs_quitGame->Load() != true) goto mainMenuEnd;

	gs_menuEnter = wad.GetLump((skulls::Sound*)wad.Get("DSSWTCHN"));
	if (gs_menuEnter == NULL) goto mainMenuEnd;
	if (gs_menuEnter->Load() != true) goto mainMenuEnd;

	gs_menuExit = wad.GetLump((skulls::Sound*)wad.Get("DSSWTCHX"));
	if (gs_menuExit == NULL) goto mainMenuEnd;
	if (gs_menuExit->Load() != true) goto mainMenuEnd;

	gs_menuMove = wad.GetLump((skulls::Sound*)wad.Get("DSSTNMOV"));
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
*/

int HandleMenuInput(skulls::Wad & wad, SDL_Surface * screen)
{
	SDL_Event event;

	// Poll for events, and handle the ones we care about.
	while(true)
	{
		if (gs_skullLigthenUp != (((GetTickCount()>>8) & 0x01) == 1))
		{
			if (gs_whichMenu == TopMenu)
				DrawMenuCursor(screen, gs_topMenuPos);
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
							SKPlaySound(&gs_menuEnter->m_soundData[0], gs_menuEnter->m_soundData.size());
							DrawTopMenu(screen);
						} else {
							SKPlaySound(&gs_menuExit->m_soundData[0], gs_menuExit->m_soundData.size());
							DrawSplashScreen(screen);
						}
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						if (gs_whichMenu == NoMenu) {
							SKPlaySound(&gs_menuEnter->m_soundData[0], gs_menuEnter->m_soundData.size());
							DrawTopMenu(screen);
						} else if (gs_whichMenu == TopMenu) {
							SKPlaySound(&gs_menuMove->m_soundData[0], gs_menuMove->m_soundData.size());
							EraseMenuCursor(screen, gs_topMenuPos);
							gs_topMenuPos = (gs_topMenuPos-1) % 4;
							DrawMenuCursor(screen, gs_topMenuPos);
						}
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						if (gs_whichMenu == NoMenu) {
							SKPlaySound(&gs_menuEnter->m_soundData[0], gs_menuEnter->m_soundData.size());
							DrawTopMenu(screen);
						} else if (gs_whichMenu == TopMenu) {
							SKPlaySound(&gs_menuMove->m_soundData[0], gs_menuMove->m_soundData.size());
							EraseMenuCursor(screen, gs_topMenuPos);
							gs_topMenuPos = (gs_topMenuPos+1) % 4;
							DrawMenuCursor(screen, gs_topMenuPos);
						}
					}
					else if (event.key.keysym.sym == SDLK_RETURN)
					{
						if (gs_whichMenu == TopMenu)
						{
							if (gs_topMenuPos == 0)
							{
								PlayLevel(*wad.GetLevel(0), screen);
								SKPlaySound(&gs_menuEnter->m_soundData[0], gs_menuEnter->m_soundData.size());
								DrawTopMenu(screen);
								DrawMenuCursor(screen, gs_topMenuPos);
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

void DrawSplashScreen(SDL_Surface * screen)
{
	gs_whichMenu = NoMenu;
	Rect bckg_dimensions = {0, 0, gs_titleBackg->m_w, gs_titleBackg->m_h};
	Draw_320x200(screen, &gs_titleBackg->m_bitmap[0], bckg_dimensions, bckg_dimensions, bckg_dimensions, 255);
}

void DrawTopMenu(SDL_Surface * screen)
{	
	gs_whichMenu = TopMenu;

	Rect bckg_dimensions = {0, 0, gs_titleBackg->m_w, gs_titleBackg->m_h};
	Rect logo_dimensions = {0, 0, gs_logo->m_w, gs_logo->m_h};
	Rect logo_screen_where = {320/2 - gs_logo->m_w/2, 3, 0, 0};

	Draw_320x200(screen, &gs_titleBackg->m_bitmap[0], bckg_dimensions, bckg_dimensions, bckg_dimensions, 128);
	Draw_320x200(screen, &gs_logo->m_bitmap[0], logo_dimensions, logo_dimensions, logo_screen_where, (unsigned char)255);

	Rect screen_where = {320/2 - gs_logo->m_w/2, 70, 0, 0};
	Rect newGame_dimensions = {0, 0, gs_newGame->m_w, gs_newGame->m_h};
	Draw_320x200(screen, &gs_newGame->m_bitmap[0], newGame_dimensions, newGame_dimensions, screen_where, (unsigned char)255);
	
	screen_where.y = 90;
	Rect options_dimensions = {0, 0, gs_options->m_w, gs_options->m_h};
	Draw_320x200(screen, &gs_options->m_bitmap[0], options_dimensions, options_dimensions, screen_where, (unsigned char)255);
	
	screen_where.y = 110;
	Rect readThis_dimensions = {0, 0, gs_readThis->m_w, gs_readThis->m_h};
	Draw_320x200(screen, &gs_readThis->m_bitmap[0], readThis_dimensions, readThis_dimensions, screen_where, (unsigned char)255);
	
	screen_where.y = 130;
	Rect quitGame_dimensions = {0, 0, gs_quitGame->m_w, gs_quitGame->m_h};
	Draw_320x200(screen, &gs_quitGame->m_bitmap[0], quitGame_dimensions, quitGame_dimensions, screen_where, (unsigned char)255);

	DrawMenuCursor(screen, gs_topMenuPos);
}

void EraseMenuCursor(SDL_Surface * screen, int pos)
{
	Rect bckg_dimensions = {0, 0, gs_titleBackg->m_w, gs_titleBackg->m_h};
	Rect skull1_dimensions = {320/2 - gs_logo->m_w/2-30, 70+20*pos, gs_skull1->m_w, gs_skull1->m_h};
	Draw_320x200(screen, &gs_titleBackg->m_bitmap[0], bckg_dimensions, skull1_dimensions, skull1_dimensions, (unsigned char)128);
}
void DrawMenuCursor(SDL_Surface * screen, int pos)
{
	gs_skullLigthenUp = ((GetTickCount()>>8) & 0x01) == 1;
	Rect screen_where = {320/2 - gs_logo->m_w/2-30, 70+20*pos, 0, 0};
	Rect skull1_dimensions = {0, 0, gs_skull1->m_w, gs_skull1->m_h};
	unsigned int * bitmap = gs_skullLigthenUp?&gs_skull2->m_bitmap[0]:&gs_skull1->m_bitmap[0];
	Draw_320x200(screen, bitmap, skull1_dimensions, skull1_dimensions, screen_where, (unsigned char)255);
}
