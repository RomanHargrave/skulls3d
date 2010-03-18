
#include "display.h"
#include <math.h>
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "doom/WadFile.h"
#include "doom/Thing.h"
#include "doom/Texture.h"
#include "doom/lumps/LevelLump.h"
#include "doom/lumps/PlayPalLump.h"
#include "doom/lumps/FlatLump.h"
#include "doom/lumps/PatchLump.h"

// [New vs Original] screen sizes
unsigned int g_scr_w = 800;
unsigned int g_scr_h = 600;
float g_strech_w = g_scr_w / (float)320;
float g_strech_h = g_scr_h / (float)200;
	
// in main.cpp
extern doom::WadFile *g_doomwad;



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


void DrawLine(SDL_Surface *screen,int _x0, int _y0, int _x1, int _y1, int color)
{	
	if (   (_x0 < 0 && _x1 < 0) 
		|| (_y0 < 0 && _y1 < 0)
		|| (_x0 >= (int)g_scr_w && _x1 > (int)g_scr_w)
		|| (_y0 >= (int)g_scr_h && _y1 > (int)g_scr_h) )
	{
		// Clipped
		return;
	}
	
	bool steep = false;

	if ( abs((_y1-_y0)) > abs(_x1-_x0) )
	{
		int change = _x0;
		_x0 = _y0;
		_y0 = change;

		change = _x1;
		_x1 = _y1;
		_y1 = change;

		steep = true;
	}

	if (_x1 < _x0){
		int change = _x0;
		_x0 = _x1;
		_x1 = change;

		change = _y0;
		_y0 = _y1;
		_y1 = change;
	}
	int dX = _x1 - _x0;
	int dY = abs(_y1 - _y0);
	int error = dX / 2;
	int ystep;
	int y = _y0;

	if (_y0 < _y1)
		ystep = 1;
	else 
		ystep = -1;

	// Skip pixels before the beginning of the screen
	if (_x0 < 0)
	{
		error -= (-_x0) * dY;
		if (error < 0)
		{
			int nb = 1+(-error/dX);
			y += nb * ystep;
			error += nb * dX;
		}
		_x0 = 0;
	}

	// Skip pixels after the end of the screen
	if (steep && _x1 >= (int)g_scr_h)
		_x1 = (int)g_scr_h-1;
	else if ((!steep) && _x1 >= (int)g_scr_w)
		_x1 = (int)g_scr_w-1;

	for (int x=_x0 ; x<=_x1 ; x++)
	{
		if (steep)
			PutPixel(screen,y,x,color);					
		else
			PutPixel(screen,x,y,color);	
		error -= dY;
		if (error < 0)
		{
			y += ystep;
			error += dX;
		}
	}								
}