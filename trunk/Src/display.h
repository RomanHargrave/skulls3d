
#ifndef SK_DISPLAY
#define SK_DISPLAY

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

typedef struct { int x,y,w,h; }Rect;

extern unsigned int g_scr_w;
extern unsigned int g_scr_h;
extern float g_strech_w;
extern float g_strech_h;

extern int g_x;
extern int g_y;
extern float g_zoom;

void MapToScreenCoords(int map_x, int map_y, int & screen_x, int & screen_y);
void PutMapPixel(SDL_Surface *screen, int x, int y, int color);
void PutPixel(SDL_Surface *screen, int x, int y, int color);
void Put4Pixels(SDL_Surface *screen, int x, int y, int color);

void Draw(SDL_Surface *screen,
		  unsigned int * bitmap, Rect dimensions,
		  Rect bitmap_what, Rect screen_where,
		  unsigned char shade);

void DrawBackGround(SDL_Surface *screen,
					unsigned int * bitmap, unsigned int bitmap_w, unsigned int bitmap_h,
					unsigned char shade);

void RefreshDebugDisplay(SDL_Surface *screen);

#endif // SK_DISPLAY