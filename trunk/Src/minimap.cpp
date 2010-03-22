
#include <windows.h>
#include <stdlib.h>

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "minimap.h"
#include "input.h"
#include "display.h"
#include "gameplay.h"
#include "doom/Thing.h"
#include "doom/Vertex.h"
#include "doom/LineDef.h"
#include "doom/SideDef.h"
#include "doom/Texture.h"
#include "doom/Seg.h"
#include "doom/lumps/PlayPalLump.h"
#include "doom/lumps/FlatLump.h"
#include "doom/lumps/PatchLump.h"

#include "Camera.h"
#include "Vec3f.h"

// in main.cpp
extern doom::WadFile *g_doomwad;
extern SDL_Surface * g_screen;

// From display.cpp
extern unsigned int g_scr_w;
extern unsigned int g_scr_h;

// map display
extern Camera *camera; // from gameplay.cpp
float g_zoom = 0.2f;

void MapToScreenCoords(int map_x, int map_y, int *screen_x, int *screen_y);
void PutMapPixel(SDL_Surface *screen, int map_x, int map_y, int color);
void Put4MapPixels(SDL_Surface *screen, int map_x, int map_y, int color);
void DrawMapLine(SDL_Surface *screen,int x0, int y0, int x1, int y1,int color);
int HandleInput();

void ShowMinimap(doom::LevelLump * level)
{
	while(1)
	{
		Sleep(1);
		if (RefreshKeybState() == SDLK_TAB)
			break;
		HandleGameplayInput();

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

			// Plot things
			for (unsigned int i=0 ; i<level->m_things.size() ; i++)
			{
				doom::Thing * thing = level->m_things[i];
				PutMapPixel(g_screen, thing->m_x, thing->m_z, 0x00FFFFFF); // white at x,y
			}
			

			if (g_keys['z'])
			{
				// Draw segs
				for (unsigned int j=0 ; j<level->m_segs.size() ; j++)
				{
					doom::Vertex *v1 = level->m_segs[j]->v1;
					doom::Vertex *v2 = level->m_segs[j]->v2;
					DrawMapLine(g_screen, v1->m_x, v1->m_z, v2->m_x, v2->m_z, 0x000000FF);
					Put4MapPixels(g_screen, v1->m_x, v1->m_z, 0x00FFFF00);
					Put4MapPixels(g_screen, v2->m_x, v2->m_z, 0x00FFFF00);
				}
			}
			else
			{
				// Draw linedefs
				for (unsigned int j=0 ; j<level->m_lineDefs.size() ; j++)
				{
					int _x0 = level->m_lineDefs[j]->m_start_vtx->m_x;				
					int _z0 = level->m_lineDefs[j]->m_start_vtx->m_z;				
					int _x1 = level->m_lineDefs[j]->m_end_vtx->m_x;
					int _z1 = level->m_lineDefs[j]->m_end_vtx->m_z;
					
					DrawMapLine(g_screen,_x0,_z0,_x1,_z1,0x00FF0000);
					Put4MapPixels(g_screen, _x0, _z0, 0x0000FF00);
					Put4MapPixels(g_screen, _x1, _z1, 0x0000FF00);

				}
			}
		}

		DrawLine(g_screen, g_scr_w/2-5, g_scr_h/2, g_scr_w/2+5, g_scr_h/2, 0x00FFFFFF);
		DrawLine(g_screen, g_scr_w/2, g_scr_h/2-5, g_scr_w/2, g_scr_h/2+5, 0x00FFFFFF);

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

		// Painting an arbitrary PatchLump
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

void MapToScreenCoords(int map_x, int map_y, int *screen_x, int *screen_y)
{
	float l_x = (g_camera->m_viewmatrix.m_data[3] + map_x) * g_zoom;
	*screen_x = (int) (g_scr_w/2.0f + l_x);

	float l_y = (g_camera->m_viewmatrix.m_data[11] + map_y) * g_zoom;
	*screen_y = (int) (g_scr_h/2.0f - l_y);
}

void PutMapPixel(SDL_Surface *screen, int map_x, int map_y, int color)
{
	int screen_x, screen_y;
	MapToScreenCoords(map_x, map_y, &screen_x, &screen_y);
	PutPixel(screen, screen_x, screen_y, color);
}
void Put4MapPixels(SDL_Surface *screen, int map_x, int map_y, int color)
{
	int screen_x, screen_y;
	MapToScreenCoords(map_x, map_y, &screen_x, &screen_y);
	Put4Pixels(screen, screen_x, screen_y, color);
}

void DrawMapLine(SDL_Surface *screen,int x0, int y0, int x1, int y1, int color)
{	
	int _x0, _y0, _x1, _y1;

	MapToScreenCoords(x0,y0,&_x0,&_y0);
	MapToScreenCoords(x1,y1,&_x1,&_y1);

	DrawLine(screen,_x0,  _y0,  _x1,  _y1,  color);
}