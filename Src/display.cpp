
#include "display.h"

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "doom/WadFile.h"
#include "doom/lumps/LevelLump.h"
#include "doom/lumps/PatchLump.h"
#include "doom/lumps/PlayPalLump.h"
#include "doom/lumps/FlatLump.h"
#include "doom/Thing.h"
#include "doom/Texture.h"

unsigned int g_scr_width = 800;
unsigned int g_scr_height = 600;
int g_x = 1000;
int g_y = -3000;
float g_zoom = 0.2f;

// in main.cpp
extern doom::WadFile *g_doomwad;


void MapToScreenCoords(int map_x, int map_y, int *screen_x, int *screen_y)
{
	float l_x = (g_x - map_x) * g_zoom;
	*screen_x = (int) (g_scr_width/2.0f - l_x);

	float l_y = (g_y - map_y) * g_zoom;
	*screen_y = (int) (g_scr_height/2.0f - l_y);
}

void PutMapPixel(SDL_Surface *screen, int map_x, int map_y, int color)
{
	int screen_x, screen_y;
	MapToScreenCoords(map_x, map_y, &screen_x, &screen_y);
	PutPixel(screen, screen_x, screen_y, color);
}

void PutPixel(SDL_Surface *screen, int x, int y, int color)
{
	if (y < 0 || y >= screen->h)
		return;
	if (x < 0 || x >= screen->w)
		return;
	
	((unsigned int*)screen->pixels)[y*(screen->pitch/4) + x] = color;
}

void Put4Pixels(SDL_Surface *screen, int x, int y, int color)
{
	PutPixel(screen, x  , y  , color);
	PutPixel(screen, x+1, y  , color);
	PutPixel(screen, x  , y+1, color);
	PutPixel(screen, x+1, y+1, color);
}

void RefreshDisplay(SDL_Surface *screen)
{
	/* fps stuff
	static int fpsticks = SDL_GetTicks();
	char fps[32];
	*/

	// Lock surface if needed
	if (SDL_MUSTLOCK(screen)) 
		if (SDL_LockSurface(screen) < 0) 
			return;

	// Cleaning
	for (unsigned int j=0 ; j<g_scr_width ; j++)
		for (unsigned int i=0 ; i<g_scr_height ; i++)
			((unsigned int*)screen->pixels)[(int) (j*screen->pitch/4 + i)] = 0;

	// Plotting things
	doom::LevelLump * level = g_doomwad->GetLevel(0);
	if (level != NULL)
	{
		level->Load();
		for (unsigned int i=0 ; i<level->m_things->m_things.size() ; i++)
		{
			doom::Thing * thing = level->m_things->m_things[i];
			PutMapPixel(screen, thing->m_x, thing->m_y, 0x00FFFFFF); // white at x,y
		}
	}

	// Painting the palette
	doom::PlayPalLump * paletteLump = g_doomwad->GetLump((doom::PlayPalLump*)g_doomwad->Get("PLAYPAL"));
	if (paletteLump != NULL)
	{
		for (unsigned int j=0 ; j<16 ; j++)
			for (unsigned int i=0 ; i<16 ; i++)
			{
				int color = paletteLump->m_palette[j*16 + i];
				Put4Pixels(screen, (int)i*2, (int)j*2, color);
			}
	}

	// Painting an arbitrary Patch
	int ticks = (SDL_GetTicks()>>8) % 4;
	char name[9];
	sprintf_s(name, "CYBR%c1", 'A'+ticks);
	doom::PatchLump * doorPatchLump = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get(name));
	
	//doom::PatchLump * doorPatchLump = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get("PLAYW0"));
	if (doorPatchLump != NULL)
	{
		doorPatchLump->Load();
		for (unsigned int j=0 ; j<doorPatchLump->m_h ; j++)
			for (unsigned int i=0 ; i<doorPatchLump->m_w ; i++)
			{
				int color = doorPatchLump->m_bitmap[j*doorPatchLump->m_w + i];
				if (color == 0xFF000000)
					continue;
				Put4Pixels(screen, (int)i*2, 128+32+(int)j*2, color);
			}
	}

	// Painting an arbitrary Flat
	doom::FlatLump * floorFlat = g_doomwad->GetLump((doom::FlatLump*)g_doomwad->Get("FLOOR5_1"));
	if (floorFlat != NULL)
	{
		floorFlat->Load();
		for (unsigned int j=0 ; j<64 ; j++)
			for (unsigned int i=0 ; i<64 ; i++)
			{
				int color = floorFlat->m_bitmap[j*64 + i];
				Put4Pixels(screen, (int)i*2, 32+(int)j*2, color);
			}
	}

	// Painting an arbitrary Texture
	doom::Texture * tex = g_doomwad->GetTexture("COMP2");
	if (tex != NULL)
	{
		tex->Load();
		for (unsigned int j=0 ; j<tex->m_h ; j++)
			for (unsigned int i=0 ; i<tex->m_w ; i++)
			{
				int color = tex->m_bitmap[j*tex->m_w + i];
				if (color == 0xFF000000)
					continue;
				Put4Pixels(screen, g_scr_width-tex->m_w+(int)i*2, (int)j*2, color);
			}
	}
	
	// Print fps
	/*
	sprintf_s(fps, "%.2f fps", 1000.0f/(SDL_GetTicks()-fpsticks));
	font.Print(fps, 10, 10, screen);
	fpsticks = SDL_GetTicks();
	#if defined(_DEBUG) || defined(DEBUG)
		sprintf(fps, "%d tris, %d pixels", g_trianglesdrawn, g_pixelsdrawn);
		font.Print(fps, 10, 26, screen);
		g_trianglesdrawn = g_pixelsdrawn = 0;
	#endif
	*/

	// Unlock if needed
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, g_scr_width, g_scr_height); 
}