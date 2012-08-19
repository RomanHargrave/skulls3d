
#include "input.h"

#include <SDL/SDL.h>

char g_keys[256];
float g_mouse_x;
float g_mouse_y;

void InitKeyboardInput()
{
	for (int i=0 ; i<256 ; i++)
		g_keys[i] = 0;
	g_mouse_x = 0;
	g_mouse_y = 0;
}

SDL_Event RefreshKeybState()
{
	SDL_Event sdlEvent;

	// Poll for events, and handle the ones we care about.
	while (SDL_PollEvent(&sdlEvent)) 
	{
		switch (sdlEvent.type) 
		{
			case SDL_MOUSEMOTION:
				g_mouse_x += sdlEvent.motion.xrel;
				g_mouse_y += sdlEvent.motion.yrel;
				break;

			case SDL_KEYDOWN:
				// If escape is pressed, return (and thus, quit)
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
					return sdlEvent;
				if (sdlEvent.key.keysym.sym == SDLK_TAB)
					return sdlEvent;
				else if (sdlEvent.key.keysym.sym == SDLK_a)
					g_keys['a'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_b)
					g_keys['b'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_c)
					g_keys['c'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_d)
					g_keys['d'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_w)
					g_keys['w'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_s)
					g_keys['s'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_e)
					g_keys['e'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_q)		
					g_keys['q'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_i)
					g_keys['i'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_k)
					g_keys['k'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_l)
					g_keys['l'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_j)
					g_keys['j'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_o)
					g_keys['o'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_u)
					g_keys['u'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_t)
					g_keys['t'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_g)
					g_keys['g'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_r)
					g_keys['r'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_f)
					g_keys['f'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_v)
					g_keys['v'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_x)
					g_keys['x'] = 1;
				else if (sdlEvent.key.keysym.sym == SDLK_z)
					g_keys['z'] = 1;
				break;
				
			case SDL_KEYUP:
				// If escape is pressed, return (and thus, quit)
				if (sdlEvent.key.keysym.sym == SDLK_d)
					g_keys['d'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_a)
					g_keys['a'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_b)
					g_keys['b'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_c)
					g_keys['c'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_w)
					g_keys['w'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_s)
					g_keys['s'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_e)
					g_keys['e'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_q)		
					g_keys['q'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_i)
					g_keys['i'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_k)
					g_keys['k'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_l)
					g_keys['l'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_j)
					g_keys['j'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_o)
					g_keys['o'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_u)
					g_keys['u'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_t)
					g_keys['t'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_g)
					g_keys['g'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_r)
					g_keys['r'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_f)
					g_keys['f'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_v)
					g_keys['v'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_x)
					g_keys['x'] = 0;
				else if (sdlEvent.key.keysym.sym == SDLK_z)
					g_keys['z'] = 0;
				break;
			case SDL_QUIT:
				return sdlEvent;
		}
	}
	return sdlEvent;
}