

#include <math.h>
#include "minimap.h"
#include "input.h"
#include "display.h"
#include "doom/lumps/PlayPalLump.h"
#include "doom/lumps/PatchLump.h"
#include "doom/lumps/FlatLump.h"
#include "doom/Texture.h"

#include <stdlib.h>
#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include <windows.h>
// in main.cpp
extern doom::WadFile *g_doomwad;
extern SDL_Surface * g_screen;

// From display.cpp
extern unsigned int g_scr_w;
extern unsigned int g_scr_h;

int HandleInput();

void ShowMinimap(doom::LevelLump * level)
{
	while(1)
	{
		Sleep(1);
		if (RefreshKeybState() < 0)
			return;
		HandleInput();

		/* fps stuff
		static int fpsticks = SDL_GetTicks();
		char fps[32];
		*/

		// Lock surface if needed
		if (SDL_MUSTLOCK(g_screen)) 
			if (SDL_LockSurface(g_screen) < 0) 
				return;

		// Cleaning
		for (unsigned int j=0 ; j<g_scr_h ; j++)
			for (unsigned int i=0 ; i<g_scr_w ; i++)
				PutPixel(g_screen, i, j, 0);

		// Plotting things
		doom::LevelLump * level = g_doomwad->GetLevel(0);
		if (level != NULL)
		{
			level->Load();
			for (unsigned int i=0 ; i<level->m_things->m_things.size() ; i++)
			{
				doom::Thing * thing = level->m_things->m_things[i];
				PutMapPixel(g_screen, thing->m_x, thing->m_y, 0x00FFFFFF); // white at x,y
			}
			//Plotting vertexes
			
			for (int j=0; j< level->m_linedefs->size;j++)
			{
				int _x1 = level->m_linedefs->Get(j)->m_start_vtx->m_x;				
				int _y1 = level->m_linedefs->Get(j)->m_start_vtx->m_y;
				if ((j+1) == level->m_linedefs->size)
					break;
				int _x2 = level->m_linedefs->Get(j+1)->m_end_vtx->m_x;
				int _y2 = level->m_linedefs->Get(j+1)->m_end_vtx->m_y;
				
				bool steep = false;

				if ( ( fabs( float (_y2 - _y1) ) ) > ( fabs( float (_x2 - _x1) ) ) )
				{
					int change = _x1;
					_x1 = _y1;
					_y1 = change;

					change = _x2;
					_x2 = _y2;
					_y2 = change;

					steep = true;
				}

				if (_x2 < _x1){
					int change = _x1;
					_x1 = _x2;
					_x2 = change;

					change = _y1;
					_y1 = _y2;
					_y2 = change;
				}
				int dX = _x2 - _x1;
				int dY = fabs(float(_y2 - _y1));
				int error = dX / 2;
				int ystep;
				int y = _y1;

				if (_y1 < _y2)
					ystep = 1;
				else 
					ystep = -1;

				for (int x =_x1 ; x<=_x2 ; x++)
				{
					if (steep)
						PutMapPixel(g_screen,y,x,200);					
					else
						PutMapPixel(g_screen,x,y,200);					
					error -= dY;
					if (error < 0)
					{
						y += ystep;
						error += dX;
					}
				}								
			}
		}		

		// Painting the palette
		/*
		doom::PlayPalLump * paletteLump = g_doomwad->GetLump((doom::PlayPalLump*)g_doomwad->Get("PLAYPAL"));
		if (paletteLump != NULL)
		{
			for (unsigned int j=0 ; j<16 ; j++)
				for (unsigned int i=0 ; i<16 ; i++)
				{
					int color = paletteLump->m_palette[j*16 + i];
					Put4Pixels(g_screen, (int)i*2, (int)j*2, color);
				}
		}
		*/

		// Painting an arbitrary Patch
		/*
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
					Put4Pixels(g_screen, (int)i*2, 128+32+(int)j*2, color);
				}
		}
		*/

		// Painting an arbitrary Flat
		/*
		doom::FlatLump * floorFlat = g_doomwad->GetLump((doom::FlatLump*)g_doomwad->Get("FLOOR5_1"));
		if (floorFlat != NULL)
		{
			floorFlat->Load();
			for (unsigned int j=0 ; j<64 ; j++)
				for (unsigned int i=0 ; i<64 ; i++)
				{
					int color = floorFlat->m_bitmap[j*64 + i];
					Put4Pixels(g_screen, (int)i*2, 32+(int)j*2, color);
				}
		}
		*/

		// Painting an arbitrary Texture
		/*
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
					Put4Pixels(g_screen, g_scr_w-tex->m_w+(int)i*2, (int)j*2, color);
				}
		}
		*/
		
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
		if (SDL_MUSTLOCK(g_screen)) 
			SDL_UnlockSurface(g_screen);

		// Tell SDL to update the whole screen
		SDL_UpdateRect(g_screen, 0, 0, g_scr_w, g_scr_h); 
	}
}



int HandleInput()
{
	static int keystick = SDL_GetTicks();
	
	while ( (SDL_GetTicks())-keystick > KEYBOARD_RATE_MS ) // Limit the keyrate
	{
		int shift = (int)(1/g_zoom);
		if (shift == 0)
			shift = 1;

		if (g_keys['d'])
			g_x -= shift;
		if (g_keys['a'])
			g_x += shift;
		if (g_keys['w'])
			g_y += shift;
		if (g_keys['s'])
			g_y -= shift;
		if (g_keys['e'])
		{
			g_zoom *= 1.01f;
		}
		if (g_keys['q'])
		{
			g_zoom /= 1.01f;
		}

		keystick += KEYBOARD_RATE_MS;
	}

	return 0;
}