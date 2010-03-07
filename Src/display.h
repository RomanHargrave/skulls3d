
#ifndef SK_DISPLAY
#define SK_DISPLAY

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

extern unsigned int g_scr_width;
extern unsigned int g_scr_height;
extern int g_x;
extern int g_y;
extern float g_zoom;

void MapToScreenCoords(int map_x, int map_y, int & screen_x, int & screen_y);
void PutMapPixel(SDL_Surface *screen, int x, int y, int color);
void PutPixel(SDL_Surface *screen, int x, int y, int color);
void Put4Pixels(SDL_Surface *screen, int x, int y, int color);

void RefreshDisplay(SDL_Surface *screen);

#endif // SK_DISPLAY