


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

		keystick += KEYBOARD_RATE_MS;
	}

	return 0;
}