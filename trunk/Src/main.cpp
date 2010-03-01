#include <stdlib.h>
#include <time.h>

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

#define SCR_WIDTH 800
#define SCR_HEIGHT 600
SDL_Surface *InitSDL();
void VideoWorks(SDL_Surface *screen);
int KeyboardWorks();

// Rendering parameters
int g_x = 1000;
int g_y = -3000;
float g_zoom = 0.2f;
doom::WadFile *g_doomwad = NULL;

// Keyboard state
char g_keys[256];


int main(int argc, char *argv[])
{
    SDL_Surface *screen = InitSDL();
	if (screen == NULL) return -1;

	g_doomwad = new doom::WadFile("doom.wad");
	if (g_doomwad == NULL)
		return -1;
	if (g_doomwad->Open() != 0)
		return -1;
	if (g_doomwad->Load() != 0)
		return -1;

	while (1)
	{
		VideoWorks(screen);
		if (int a=KeyboardWorks() < 0)
			return -a;
	}
	return 0;
}


SDL_Surface *InitSDL()
{
	SDL_Surface *screen;
	// Initialize SDL's subsystems - in this case, only video.
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	//SDL_WM_GrabInput(SDL_GRAB_ON);
	//SDL_ShowCursor(0);

	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);

	// Attempt to create a SCR_WIDTHxSCR_HEIGHT window with 32bit pixels.
	screen = SDL_SetVideoMode(SCR_WIDTH, SCR_HEIGHT, 32, SDL_SWSURFACE);
	return screen;
}



void VideoWorks(SDL_Surface *screen)
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
	for (int j=0 ; j<SCR_HEIGHT ; j++)
		for (int i=0 ; i<SCR_WIDTH ; i++)
			((unsigned int*)screen->pixels)[(int) (j*screen->pitch/4 + i)] = 0;

	// Plotting things
	doom::LevelLump * level = g_doomwad->LoadLevel(0);
	for (unsigned int i=0 ; i<level->m_things->m_things.size() ; i++)
	{
		doom::Thing * thing = level->m_things->m_things[i];

		float x = (g_x - thing->m_x) * g_zoom;
		x = SCR_WIDTH/2.0f - x;

		float y = (g_y - thing->m_y) * g_zoom;
		y = SCR_HEIGHT/2.0f - y;

		if (y < 0 || y >= screen->h)
			continue;
		if (x < 0 || x >= screen->w)
			continue;

		((unsigned int*)screen->pixels)[((int)y)*(screen->pitch/4) + (int)x] = 0xFFFFFFFF;
	}

	// Painting the palette
	doom::PlayPalLump * paletteLump = g_doomwad->GetLump((doom::PlayPalLump*)g_doomwad->Get("PLAYPAL"));
	if (paletteLump != NULL)
	{
		for (unsigned int j=0 ; j<16 ; j++)
			for (unsigned int i=0 ; i<16 ; i++)
			{
				int color = paletteLump->m_palette[j*16 + i];
				((unsigned int*)screen->pixels)[((int)j*2  )*(screen->pitch/4) + (int)i*2  ] = (int) color;
				((unsigned int*)screen->pixels)[((int)j*2+1)*(screen->pitch/4) + (int)i*2  ] = (int) color;
				((unsigned int*)screen->pixels)[((int)j*2  )*(screen->pitch/4) + (int)i*2+1] = (int) color;
				((unsigned int*)screen->pixels)[((int)j*2+1)*(screen->pitch/4) + (int)i*2+1] = (int) color;
			}
	}

	// Painting an arbitrary Patch
	int ticks = (SDL_GetTicks()>>8) % 4;
	char name[9];
	sprintf(name, "CYBR%c1", 'A'+ticks);
	doom::PatchLump * doorPatchLump = g_doomwad->GetLump((doom::PatchLump*)g_doomwad->Get(name));
	if (doorPatchLump != NULL)
	{
		doorPatchLump->Load();
		for (unsigned int j=0 ; j<doorPatchLump->m_h ; j++)
			for (unsigned int i=0 ; i<doorPatchLump->m_w ; i++)
			{
				int color = doorPatchLump->m_texture[j*doorPatchLump->m_w + i];
				if (color == 0xFF000000)
					continue;
				((unsigned int*)screen->pixels)[(128+32+(int)j*2  )*(screen->pitch/4) + (int)i*2  ] = (int) color;
				((unsigned int*)screen->pixels)[(128+32+(int)j*2+1)*(screen->pitch/4) + (int)i*2  ] = (int) color;
				((unsigned int*)screen->pixels)[(128+32+(int)j*2  )*(screen->pitch/4) + (int)i*2+1] = (int) color;
				((unsigned int*)screen->pixels)[(128+32+(int)j*2+1)*(screen->pitch/4) + (int)i*2+1] = (int) color;
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
				int color = floorFlat->m_texture[j*64 + i];
				((unsigned int*)screen->pixels)[(32+(int)j*2  )*(screen->pitch/4) + (int)i*2  ] = (int) color;
				((unsigned int*)screen->pixels)[(32+(int)j*2+1)*(screen->pitch/4) + (int)i*2  ] = (int) color;
				((unsigned int*)screen->pixels)[(32+(int)j*2  )*(screen->pitch/4) + (int)i*2+1] = (int) color;
				((unsigned int*)screen->pixels)[(32+(int)j*2+1)*(screen->pitch/4) + (int)i*2+1] = (int) color;
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
	SDL_UpdateRect(screen, 0, 0, SCR_WIDTH, SCR_HEIGHT); 
}

int KeyboardWorks()
{
	static int keystick = SDL_GetTicks();
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
					return -10;
				else if (event.key.keysym.sym == SDLK_d)
					g_keys['d'] = 1;
				else if (event.key.keysym.sym == SDLK_a)
					g_keys['a'] = 1;
				else if (event.key.keysym.sym == SDLK_w)
					g_keys['w'] = 1;
				else if (event.key.keysym.sym == SDLK_s)
					g_keys['s'] = 1;
				else if (event.key.keysym.sym == SDLK_e)
					g_keys['e'] = 1;
				else if (event.key.keysym.sym == SDLK_q)		
					g_keys['q'] = 1;
				else if (event.key.keysym.sym == SDLK_i)
					g_keys['i'] = 1;
				else if (event.key.keysym.sym == SDLK_k)
					g_keys['k'] = 1;
				else if (event.key.keysym.sym == SDLK_l)
					g_keys['l'] = 1;
				else if (event.key.keysym.sym == SDLK_j)
					g_keys['j'] = 1;
				else if (event.key.keysym.sym == SDLK_o)
					g_keys['o'] = 1;
				else if (event.key.keysym.sym == SDLK_u)
					g_keys['u'] = 1;
				else if (event.key.keysym.sym == SDLK_PLUS)
					g_keys['+'] = 1;
				else if (event.key.keysym.sym == SDLK_MINUS)
					g_keys['-'] = 1;
				break;
				
			case SDL_KEYUP:
				// If escape is pressed, return (and thus, quit)
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return -2;
				else if (event.key.keysym.sym == SDLK_d)
					g_keys['d'] = 0;
				else if (event.key.keysym.sym == SDLK_a)
					g_keys['a'] = 0;
				else if (event.key.keysym.sym == SDLK_w)
					g_keys['w'] = 0;
				else if (event.key.keysym.sym == SDLK_s)
					g_keys['s'] = 0;
				else if (event.key.keysym.sym == SDLK_e)
					g_keys['e'] = 0;
				else if (event.key.keysym.sym == SDLK_q)		
					g_keys['q'] = 0;
				else if (event.key.keysym.sym == SDLK_i)
					g_keys['i'] = 0;
				else if (event.key.keysym.sym == SDLK_k)
					g_keys['k'] = 0;
				else if (event.key.keysym.sym == SDLK_l)
					g_keys['l'] = 0;
				else if (event.key.keysym.sym == SDLK_j)
					g_keys['j'] = 0;
				else if (event.key.keysym.sym == SDLK_o)
					g_keys['o'] = 0;
				else if (event.key.keysym.sym == SDLK_u)
					g_keys['u'] = 0;
				else if (event.key.keysym.sym == SDLK_PLUS)
					g_keys['+'] = 0;
				else if (event.key.keysym.sym == SDLK_MINUS)
					g_keys['_'] = 0;
					break;
			case SDL_QUIT:
				return -3;
		}
	}

	if ( (SDL_GetTicks())-keystick > 5 ) // Limit the keyrate
	{
		if (g_keys['d'])
			g_x -= (int) (1/g_zoom);
		if (g_keys['a'])
			g_x += (int) (1/g_zoom);
		if (g_keys['w'])
			g_y += (int) (1/g_zoom);
		if (g_keys['s'])
			g_y -= (int) (1/g_zoom);
		if (g_keys['e'])
		{
			g_zoom *= 1.01f;
		}
		if (g_keys['q'])
		{
			g_zoom /= 1.01f;
		}

		keystick = SDL_GetTicks();
	}

	return 0;
}