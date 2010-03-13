
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

// [New vs Original] screen sizes
unsigned int g_scr_w = 800;
unsigned int g_scr_h = 600;
float g_strech_w = g_scr_w / (float)320;
float g_strech_h = g_scr_h / (float)200;

// map display
int g_x = 1000;
int g_y = -3000;
float g_zoom = 0.2f;
	
// in main.cpp
extern doom::WadFile *g_doomwad;


void MapToScreenCoords(int map_x, int map_y, int *screen_x, int *screen_y)
{
	float l_x = (g_x - map_x) * g_zoom;
	*screen_x = (int) (g_scr_w/2.0f - l_x);

	float l_y = (g_y - map_y) * g_zoom;
	*screen_y = (int) (g_scr_h/2.0f - l_y);
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

void Draw_320x200(SDL_Surface *screen,
		  unsigned int * bitmap, Rect dimensions,
		  Rect bitmap_what, Rect screen_where,
		  unsigned char shade)
{
	// Lock surface if needed
	if (SDL_MUSTLOCK(screen)) 
		if (SDL_LockSurface(screen) < 0) 
			return;

	for (unsigned int j=0 ; j<bitmap_what.h ; j++)
		for (unsigned int i=0 ; i<bitmap_what.w ; i++)
		{
			unsigned int color = bitmap[(bitmap_what.y+j)*dimensions.w + (bitmap_what.x+i)];
			if (color == 0xFF000000)
				continue;
			// Apply shade
			color = ((((color&0xFF0000)*(shade+1))>>8)&0xFF0000)
				  | ((((color&0x00FF00)*(shade+1))>>8)&0x00FF00)
				  | ((((color&0x0000FF)*(shade+1))>>8)&0x0000FF);

			int screen_x_1 = (int) ((screen_where.x + i  ) * g_strech_w);
			int screen_x_2 = (int) ((screen_where.x + i+1) * g_strech_w);
			int screen_y_1 = (int) ((screen_where.y + j  ) * g_strech_h);
			int screen_y_2 = (int) ((screen_where.y + j+1) * g_strech_h);
			for (int l=screen_y_1 ; l<screen_y_2 ; l++)
				for (int k=screen_x_1 ; k<screen_x_2 ; k++)
					PutPixel(screen, k, l, color);
		}

	// Unlock if needed
	if (SDL_MUSTLOCK(screen)) 
		SDL_UnlockSurface(screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(screen, 0, 0, g_scr_w, g_scr_h); 
}