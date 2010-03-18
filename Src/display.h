
#ifndef SK_DISPLAY
#define SK_DISPLAY

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

typedef struct { unsigned int x,y,w,h; }Rect;

extern unsigned int g_scr_w;
extern unsigned int g_scr_h;
extern float g_strech_w;
extern float g_strech_h;

void DrawLine(SDL_Surface *screen,int x0, int y0, int x1, int y1,int color);
void PutPixel(SDL_Surface *screen, int x, int y, int color);
void Put4Pixels(SDL_Surface *screen, int x, int y, int color);

void Draw_320x200(SDL_Surface *screen,
		          unsigned int * bitmap, Rect dimensions,
				  Rect bitmap_what, Rect screen_where,
				  unsigned char shade);

void RefreshDebugDisplay(SDL_Surface *screen);

#endif // SK_DISPLAY