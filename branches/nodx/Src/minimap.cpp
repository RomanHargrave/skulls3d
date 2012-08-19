
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>
#include <swegl/swegl.hpp>

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
#include "doom/SSector.h"
#include "doom/Node.h"
#include "doom/lumps/PlayPalLump.h"
#include "doom/lumps/FlatLump.h"
#include "doom/lumps/PatchLump.h"


// From display.cpp
extern unsigned int g_scr_w;
extern unsigned int g_scr_h;

// map display
extern swegl::Camera *camera; // from gameplay.cpp
float g_zoom = 0.2f;
int g_gridZoom = 64;

void MapToScreenCoords(int map_x, int map_z, int *screen_x, int *screen_y);
void ScreenToMapCoords(int screen_x, int screen_y, float *map_x, float *map_z);
void PutMapPixel(SDL_Surface *screen, int map_x, int map_z, int color);
void Put4MapPixels(SDL_Surface *screen, int map_x, int map_z, int color);
void DrawMapLine(SDL_Surface *screen, int x0, int z0, int x1, int z1,int color);
void DrawSeg(SDL_Surface *screen, skulls::Vertex *v1, skulls::Vertex *v2, unsigned int color, bool plotV1=true, bool plotV2=true);

void ShowMinimap(SDL_Surface * screen, skulls::Level & level)
{
	int ssectorId = 0;
	while(1)
	{
		Sleep(1);
		SDL_Event event = RefreshKeybState();
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB)
			break;
		HandleGameplayInput(event);

		/* fps stuff
		static int fpsticks = SDL_GetTicks();
		char fps[32];
		*/

		// Lock surface if needed
		if (SDL_MUSTLOCK(screen)) 
			if (SDL_LockSurface(screen) < 0) 
				return;

		// Cleaning
		for (unsigned int j=0 ; j<g_scr_h ; j++)
			for (unsigned int i=0 ; i<g_scr_w ; i++)
				PutPixel(screen, i, j, 0);

		// Drawing grid
		float xMin, xMax, zMin, zMax;
		ScreenToMapCoords(0, 0, &xMin, &zMin);
		ScreenToMapCoords(g_scr_w, g_scr_h, &xMax, &zMax);
		xMax -= g_gridZoom + ((int)xMax % g_gridZoom);
		zMax -= g_gridZoom + ((int)zMax % g_gridZoom);
		for (int i=(int)xMax ; i<=(int)xMin ; i+=g_gridZoom)
			DrawMapLine(screen, i, (int)zMin, i, (int)zMax, 0x00303030);
		for (int i=(int)zMax ; i<=(int)zMin ; i+=g_gridZoom)
			DrawMapLine(screen, (int)xMin, i, (int)xMax, i, 0x00303030);

		// Plotting things
		//skulls::Level * level = wad.GetLevel(0);
		//if (level != NULL)
		//{
			// Plot things
			for (unsigned int i=0 ; i<level.m_things.size() ; i++)
			{
				skulls::Thing & thing = level.m_things[i];
				PutMapPixel(screen, thing.m_x, thing.m_z, 0x00FFFFFF); // white at x,y
			}
			
			if (g_keys['z'])
			{
				srand(12345678);
				for (unsigned int j=0 ; j<level.m_ssectors.size()-1 ; j++)
				{
					unsigned int color = 0;
					while (color==0)
						color = (!(rand()%3)?0:((56+rand()%200)<<16)) | (!(rand()%3)?0:((56+rand()%200)<<8)) | (!(rand()%3)?0:((56+rand()%200)));
					std::shared_ptr<skulls::SSector> ssector = level.m_ssectors[j];
					//skulls::Vertex *start_vertex = ssector->m_segs.front()->v1;

					// Draw ssectors and segs
					for (auto it=ssector->m_segs.begin(),end=ssector->m_segs.end() ; it!=end ; ++it)
					{
						//if ((it)->m_valid == false)
						//	continue;
						DrawSeg(screen, (*it)->m_v1.get(), (*it)->m_v2.get(), color);
					}
				}
			}
			else if (g_keys['b'])
			{
				// Draw SSector dividers from BSP Tree
				float cam_x = -g_camera->m_viewmatrix.m_data[3];
				float cam_z = -g_camera->m_viewmatrix.m_data[11];
				skulls::SSector *ssector = level.m_bspTree->GetSSByPosition(cam_x, cam_z);
				for (skulls::Node *n = ssector->m_parentNode ; n != NULL ; n = n->m_parent)
				{
					if (n->IsOnRight(cam_x, cam_z))
						DrawSeg(screen, &n->m_startVertex, &n->m_endVertex, 0x0000FF00, false, false);
					else
						DrawSeg(screen, &n->m_endVertex, &n->m_startVertex, 0x0000FF00, false, false);
				}
			}
			else if (g_keys['c'])
			{
				// Draw ssector segs
				float cam_x = -g_camera->m_viewmatrix.m_data[3];
				float cam_z = -g_camera->m_viewmatrix.m_data[11];

				skulls::SSector *ssector = level.m_bspTree->GetSSByPosition(cam_x, cam_z);
				if (ssector->id != ssectorId)
				{
					ssectorId = ssector->id;
					printf("SSector %d\n", ssectorId);
				}

				for (auto it=ssector->m_segs.begin(),end=ssector->m_segs.end() ; it!=end ; ++it)
				{
					//skulls::Seg *seg = *it;
					//if (seg->m_valid == false)
					//	continue;
					DrawSeg(screen, (*it)->m_v1.get(), (*it)->m_v2.get(), 0x000000FF, !(*it)->m_backOpen, !(*it)->m_frontOpen);
				}
			}
			else if (g_keys['v'])
			{
				// Draw all ssectors
				for (auto itss=level.m_ssectors.begin(),endss=level.m_ssectors.end() ; itss!=endss ; ++itss)
				{
					for (auto it=(*itss)->m_segs.begin(),end=(*itss)->m_segs.end() ; it!=end ; ++it)
					{
						//skulls::Seg *seg = *it;
						//if (seg->m_valid == false)
						//	continue;
						DrawSeg(screen, (*it)->m_v1.get(), (*it)->m_v2.get(), 0x000000FF, true, true/*!seg->m_backOpen, !seg->m_frontOpen*/);
					}
				}
			}
			else if (g_keys['x'])
			{
				unsigned int color = 0x0000FF00;

				// Draw dividers
				for (auto it=level.m_dividers.begin(),end=level.m_dividers.end() ; it!=end ; ++it)
				{
					//skulls::Seg *seg = *it;
					//if (seg->m_valid == false)
					//	continue;
					DrawSeg(screen, (*it)->m_v1.get(), (*it)->m_v2.get(), color);
				}
			}
			else
			{
				// Draw linedefs
				for (unsigned int j=0 ; j<level.m_lineDefs.size() ; j++)
				{
					int _x0 = level.m_lineDefs[j]->m_start_vtx->m_x;				
					int _z0 = level.m_lineDefs[j]->m_start_vtx->m_z;				
					int _x1 = level.m_lineDefs[j]->m_end_vtx->m_x;
					int _z1 = level.m_lineDefs[j]->m_end_vtx->m_z;
					
					DrawMapLine(screen,_x0,_z0,_x1,_z1,0x00FF0000);
					Put4MapPixels(screen, _x0, _z0, 0x0000FF00);
					Put4MapPixels(screen, _x1, _z1, 0x0000FF00);
				}
			}
		//}

		DrawLine(screen, g_scr_w/2-5, g_scr_h/2, g_scr_w/2+5, g_scr_h/2, 0x00FFFFFF);
		DrawLine(screen, g_scr_w/2, g_scr_h/2-5, g_scr_w/2, g_scr_h/2+5, 0x00FFFFFF);

		// Painting the palette
		/*
		skulls::PlayPalLump * paletteLump = wad.GetLump((skulls::PlayPalLump*)wad.Get("PLAYPAL"));
		if (paletteLump != NULL)
		{
			for (unsigned int j=0 ; j<16 ; j++)
				for (unsigned int i=0 ; i<16 ; i++)
				{
					int color = paletteLump->m_palette[j*16 + i];
					Put4Pixels(screen, (int)i*2, (int)j*2, color);
				}
		}
		*/

		// Painting an arbitrary PatchLump
		/*
		int ticks = (SDL_GetTicks()>>8) % 4;
		char name[9];
		sprintf_s(name, "CYBR%c1", 'A'+ticks);
		skulls::PatchLump * doorPatchLump = wad.GetLump((skulls::PatchLump*)wad.Get(name));
		
		//skulls::PatchLump * doorPatchLump = wad.GetLump((skulls::PatchLump*)wad.Get("PLAYW0"));
		if (doorPatchLump != NULL)
		{
			doorPatchLump->Load();
			for (unsigned int j=0 ; j<doorPatchLump->m_h ; j++)
				for (unsigned int i=0 ; i<doorPatchLump->m_w ; i++)
				{
					int color = doorPatchLump->m_bitmap[j*doorPatchLump->m_w + i];
					if (color == 0xFF000000)
						continue;
					Put4Pixels(screen, (int)i*2, 128+32+(int)j*2, color);
				}
		}
		*/

		// Painting an arbitrary Flat
		/*
		skulls::Flat * floorFlat = wad.GetLump((skulls::Flat*)wad.Get("FLOOR5_1"));
		if (floorFlat != NULL)
		{
			floorFlat->Load();
			for (unsigned int j=0 ; j<64 ; j++)
				for (unsigned int i=0 ; i<64 ; i++)
				{
					int color = floorFlat->m_bitmap[j*64 + i];
					Put4Pixels(screen, (int)i*2, 32+(int)j*2, color);
				}
		}
		*/

		// Painting an arbitrary Texture
		/*
		skulls::Texture * tex = wad.GetTexture("COMP2");
		if (tex != NULL)
		{
			tex->Load();
			for (unsigned int j=0 ; j<tex->m_h ; j++)
				for (unsigned int i=0 ; i<tex->m_w ; i++)
				{
					int color = tex->m_bitmap[j*tex->m_w + i];
					if (color == 0xFF000000)
						continue;
					Put4Pixels(screen, g_scr_w-tex->m_w+(int)i*2, (int)j*2, color);
				}
		}
		/**/
		
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
		SDL_UpdateRect(screen, 0, 0, g_scr_w, g_scr_h); 
	}
}

void MapToScreenCoords(int map_x, int map_z, int *screen_x, int *screen_y)
{
	float l_x = (g_camera->m_viewmatrix.m_data[3] + map_x) * g_zoom;
	*screen_x = (int) (g_scr_w/2.0f + l_x);

	float l_y = (g_camera->m_viewmatrix.m_data[11] + map_z) * g_zoom;
	*screen_y = (int) (g_scr_h/2.0f - l_y);
}

void ScreenToMapCoords(int screen_x, int screen_y, float *map_x, float *map_z)
{
	float _x = (g_scr_w/2.0f) - screen_x;
	*map_x = (_x / g_zoom) - g_camera->m_viewmatrix.m_data[3];
	
	float _y = (g_scr_h/2.0f) - screen_y;
	*map_z = (_y / g_zoom) - g_camera->m_viewmatrix.m_data[11];
}

void PutMapPixel(SDL_Surface *screen, int map_x, int map_z, int color)
{
	int screen_x, screen_y;
	MapToScreenCoords(map_x, map_z, &screen_x, &screen_y);
	PutPixel(screen, screen_x, screen_y, color);
}
void Put4MapPixels(SDL_Surface *screen, int map_x, int map_z, int color)
{
	int screen_x, screen_y;
	MapToScreenCoords(map_x, map_z, &screen_x, &screen_y);
	Put4Pixels(screen, screen_x, screen_y, color);
}

void DrawMapLine(SDL_Surface *screen,int x0, int z0, int x1, int z1, int color)
{	
	int _x0, _y0, _x1, _y1;

	MapToScreenCoords(x0,z0,&_x0,&_y0);
	MapToScreenCoords(x1,z1,&_x1,&_y1);

	DrawLine(screen,_x0,  _y0,  _x1,  _y1,  color);
}

void DrawSeg(SDL_Surface * screen, skulls::Vertex *v1, skulls::Vertex *v2, unsigned int color, bool plotV1, bool plotV2)
{
	unsigned int dot_color = 0x00FFFFFF - color;
	DrawMapLine(screen, v1->m_x, v1->m_z, v2->m_x, v2->m_z, color);
	if (plotV1)
		Put4MapPixels(screen, v1->m_x, v1->m_z, dot_color);
	if (plotV2)
		Put4MapPixels(screen, v2->m_x, v2->m_z, dot_color);

	// Drawing right side mark
	unsigned int xMid = (v1->m_x + v2->m_x)/2;
	unsigned int zMid = (v1->m_z + v2->m_z)/2;
	unsigned int xDep = (v2->m_z - v1->m_z)/10;
	unsigned int zDep = -(v2->m_x - v1->m_x)/10;

	DrawMapLine(screen, xMid, zMid, xMid+xDep, zMid+zDep, color);
}