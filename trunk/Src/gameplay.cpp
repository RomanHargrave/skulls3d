
#include <windows.h>
#include <stdlib.h>

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "gameplay.h"
#include "minimap.h"
#include "input.h"

#include "Matrix4x4.h"
#include "ViewPort.h"
#include "Scene.h"
#include "Renderer.h"
#include "Texture.h"
#include "Vec3f.h"
#include "RectangleTriangle.h"

#include "doom/Thing.h"
#include "doom/SideDef.h"
#include "doom/LineDef.h"
#include "doom/Vertex.h"
#include "doom/Sector.h"
#include "doom/Texture.h"
#include "doom/lumps/PatchLump.h"

#include "Cube.h"

// From display.cpp
extern unsigned int g_scr_w;
extern unsigned int g_scr_h;

// From main.cpp
extern SDL_Surface *g_screen;
LARGE_INTEGER g_renderingTime;
unsigned int g_frameCount = 0;

// From minimap.cpp
extern float g_zoom;

Scene* BuildScene(doom::LevelLump * level);
void VideoWorks();
int HandleGameplayInput();

static Scene *scene;
Camera *g_camera;
static ViewPort *viewport1;
static Renderer *renderer1 = NULL;
	
static float g_cameraxrotation;
static float g_camerayrotation;


void PlayLevel(doom::LevelLump * level)
{
	if (level == NULL)
		return;
	if (level->Load() != 0)
		return;
	
	scene = BuildScene(level);

	g_camera = new Camera(g_scr_w/(1.0f*g_scr_h));
	viewport1 = new ViewPort(0, 0, g_scr_w, g_scr_h, g_screen);
	renderer1 = new Renderer(scene, g_camera, viewport1);
	g_camera->Translate(level->m_things[0]->m_x, 0.0f, level->m_things[0]->m_z);

	ShowMinimap(level);

	/*
	while (1)
	{
		VideoWorks();
		int a = RefreshKeybState();
		if (a < 0)
			return;
		else if (a == SDLK_TAB)
			ShowMinimap(level);
		HandleGameplayInput();
	}
	*/
	return;
}

Scene* BuildScene(doom::LevelLump * level)
{
	Scene *s = new Scene();
	unsigned int count;

	/*
	count = (unsigned int) level->m_things.size();
	for (unsigned int i=0 ; i<count ; i++)
	{
		doom::Thing * thingLump = level->m_things[i];

		Texture *texture = new Texture(thingLump->m_sprites[0]->m_bitmap,
									   thingLump->m_sprites[0]->m_w,
									   thingLump->m_sprites[0]->m_h);

		Cube *c = new Cube(texture->m_mipmaps[0].m_height*1.0f, texture);
		c->m_worldmatrix.Translate(thingLump->m_x, 0.0f, thingLump->m_z);
		s->AddMesh(c);
	}
	*/

	Texture *texture = new Texture(0x00FFFFFF);
	count = (unsigned int) level->m_sideDefs.size();
	for (unsigned int i=0 ; i<count ; i++)
	{
		doom::SideDef *sideDef = level->m_sideDefs[i];
		doom::Vertex *v0, *v1;
		doom::Sector *sector, *otherSector;
		if (sideDef == sideDef->m_lineDef->m_rightSideDef)
		{
			v0 = sideDef->m_lineDef->m_start_vtx;
			v1 = sideDef->m_lineDef->m_end_vtx;
			sector = sideDef->m_sector;
			if (sideDef->m_lineDef->m_leftSideDef != NULL)
				otherSector = sideDef->m_lineDef->m_leftSideDef->m_sector;
			else
				otherSector = NULL;
		}
		else
		{
			v1 = sideDef->m_lineDef->m_start_vtx;
			v0 = sideDef->m_lineDef->m_end_vtx;
			sector = sideDef->m_sector;
			if (sideDef->m_lineDef->m_rightSideDef != NULL)
				otherSector = sideDef->m_lineDef->m_rightSideDef->m_sector;
			else
				otherSector = NULL;
		}
		if (otherSector == NULL)
		{
			float floorHeight = sideDef->m_sector->m_floorHeight;
			float ceilingHeight = sideDef->m_sector->m_ceilingHeight;

			Mesh *mesh = new Mesh();
			Vec3f *vecs = new Vec3f[4];
			vecs[0].x = v0->m_x; vecs[0].y = floorHeight;   vecs[0].z = v0->m_z;
			vecs[1].x = v1->m_x; vecs[1].y = floorHeight;   vecs[1].z = v1->m_z;
			vecs[2].x = v0->m_x; vecs[2].y = ceilingHeight; vecs[2].z = v0->m_z;
			vecs[3].x = v1->m_x; vecs[3].y = ceilingHeight; vecs[3].z = v1->m_z;
			mesh->SetVertexBuffer(vecs, 4);
			Texture *tex = new Texture(sideDef->m_middleTexture->m_bitmap, sideDef->m_middleTexture->m_w, sideDef->m_middleTexture->m_h);
			mesh->SetTexture(tex);
			Vec2f t0, t1, t2, t3;
			t0.x=0;                             t0.y=sideDef->m_middleTexture->m_h;
			t1.x=sideDef->m_middleTexture->m_w; t1.y=sideDef->m_middleTexture->m_h;
			t2.x=0;                             t2.y=0;
			t3.x=sideDef->m_middleTexture->m_w; t3.y=0;
			Vec2f *texBuf = new Vec2f[6];
			texBuf[0]=t0; texBuf[1]=t1; texBuf[2]=t3;
			texBuf[3]=t0; texBuf[4]=t3; texBuf[5]=t2; 
			unsigned int *indexBuf = new unsigned int[4];
			indexBuf[0]=0; indexBuf[1]=1; indexBuf[2]=3; indexBuf[3]=2;
			mesh->AddFan(indexBuf, texBuf, 4);
			s->AddMesh(mesh);
		}
		else
		{
			if (sector->m_floorHeight < otherSector->m_floorHeight)
			{
				float floorHeight = sector->m_floorHeight;
				float ceilingHeight = otherSector->m_floorHeight;

				Mesh *mesh = new Mesh();
				Vec3f *vecs = new Vec3f[4];
				vecs[0].x = v0->m_x; vecs[0].y = floorHeight;   vecs[0].z = v0->m_z;
				vecs[1].x = v1->m_x; vecs[1].y = floorHeight;   vecs[1].z = v1->m_z;
				vecs[2].x = v0->m_x; vecs[2].y = ceilingHeight; vecs[2].z = v0->m_z;
				vecs[3].x = v1->m_x; vecs[3].y = ceilingHeight; vecs[3].z = v1->m_z;
				mesh->SetVertexBuffer(vecs, 4);
				mesh->SetTexture(texture);
				Vec2f *texBuf = new Vec2f[4];
				texBuf[0].x=0; texBuf[0].y=0;
				texBuf[1].x=1; texBuf[1].y=0;
				texBuf[2].x=0; texBuf[2].y=1;
				texBuf[3].x=1; texBuf[3].y=1;
				unsigned int *indexBuf = new unsigned int[4];
				indexBuf[0]=0; indexBuf[1]=1; indexBuf[2]=3; indexBuf[3]=2;
				mesh->AddFan(indexBuf, texBuf, 4);
				s->AddMesh(mesh);
			}
			if (sector->m_ceilingHeight > otherSector->m_ceilingHeight)
			{
				float floorHeight = otherSector->m_ceilingHeight;
				float ceilingHeight = sector->m_ceilingHeight;

				Mesh *mesh = new Mesh();
				Vec3f *vecs = new Vec3f[4];
				vecs[0].x = v0->m_x; vecs[0].y = floorHeight;   vecs[0].z = v0->m_z;
				vecs[1].x = v1->m_x; vecs[1].y = floorHeight;   vecs[1].z = v1->m_z;
				vecs[2].x = v0->m_x; vecs[2].y = ceilingHeight; vecs[2].z = v0->m_z;
				vecs[3].x = v1->m_x; vecs[3].y = ceilingHeight; vecs[3].z = v1->m_z;
				mesh->SetVertexBuffer(vecs, 4);
				mesh->SetTexture(texture);
				Vec2f *texBuf = new Vec2f[4];
				texBuf[0].x=0; texBuf[0].y=0;
				texBuf[1].x=1; texBuf[1].y=0;
				texBuf[2].x=0; texBuf[2].y=1;
				texBuf[3].x=1; texBuf[3].y=1;
				unsigned int *indexBuf = new unsigned int[4];
				indexBuf[0]=0; indexBuf[1]=1; indexBuf[2]=3; indexBuf[3]=2;
				mesh->AddFan(indexBuf, texBuf, 4);
				s->AddMesh(mesh);
			}
		}
	}

	/*
	count = level->m_sectors.size();
	for (unsigned int i=0 ; i<count ; i++)
	{
		doom::Sector *sector = level->m_sectors[i];
		
		// Build the list of vertices of the sector
		std::list<doom::Vertex*> vertexes;
		for (std::list<doom::SideDef*>::iterator it=sector->m_sideDefs.begin() ; it!=sector->m_sideDefs.end() ; ++it)
		{
			vertexes.push_back(((doom::SideDef*)(*it))->m_lineDef->m_start_vtx);
			vertexes.push_back(((doom::SideDef*)(*it))->m_lineDef->m_end_vtx);
		}
		vertexes.unique(); // Remove vertices that were added twice

		// Sectors may be composed of more than one unconnected closed polygons
		// We need to tesselate each polygon one by one separately
		std::list<std::list<doom::Vertex*>> polygons;
		while (!vertexes.empty())
		{
			std::list<doom::Vertex*> polygon;
			doom::Vertex *firstVertex = vertexes.front();
			vertexes.pop_front();

			doom::Vertex *nextVertex = NULL;
			do {
				// Find which is the next vertex
			}while (nextVertex != firstVertex);
			
			polygons.push_back(polygon);
		}
	}*/

	return s;
}

void VideoWorks()
{
	LARGE_INTEGER microseconds1;

	// Lock surface if needed
	if (SDL_MUSTLOCK(g_screen)) 
		if (SDL_LockSurface(g_screen) < 0) 
			return;

	if ((g_frameCount % 100) == 0) {
		QueryPerformanceCounter(&microseconds1);
	}

	g_camera->RotateY(g_camerayrotation);
	g_camera->RotateX(g_cameraxrotation);
	renderer1->Render();
	g_camera->RotateX(-g_cameraxrotation);
	g_camera->RotateY(-g_camerayrotation);
	
	if ((g_frameCount % 100) == 0) {
		LARGE_INTEGER microseconds2;
		QueryPerformanceCounter(&microseconds2);
		g_renderingTime.QuadPart = microseconds2.QuadPart - microseconds1.QuadPart;
		
		LARGE_INTEGER freq;			
		QueryPerformanceFrequency(&freq);
		double seconds = g_renderingTime.QuadPart / (1.0f*freq.QuadPart);
		char tmp[256];
		sprintf_s(tmp, "Time per frame is %e s\n", seconds);
		TCHAR tmp2[512];
		wsprintf(tmp2, L"%S", tmp);
		OutputDebugString(tmp2);
	}
	g_frameCount++;

	// Unlock if needed
	if (SDL_MUSTLOCK(g_screen)) 
		SDL_UnlockSurface(g_screen);

	// Tell SDL to update the whole screen
	SDL_UpdateRect(g_screen, 0, 0, g_scr_w, g_scr_h); 
}

int HandleGameplayInput()
{
	static int g_keystick = SDL_GetTicks();
	
	while ( (SDL_GetTicks())-g_keystick > KEYBOARD_RATE_MS ) // Limit the keyrate
	{
		if (g_mouse_x != 0 || g_mouse_y != 0)
		{
			//g_camera->RotateX(-g_cameraxrotation);
			//g_camera->RotateY(g_mouse_x / 2000.0f);
			g_cameraxrotation += g_mouse_y / 1000.0f;
			g_camerayrotation += g_mouse_x / 1000.0f;
			//g_camera->RotateX(g_cameraxrotation);
			g_mouse_x = 0;
			g_mouse_y = 0;
		}
		/*
		if (g_keys['d'])
			g_camera->RotateY(KEYBOARD_RATE_MS * 3.14159f / 2000.0f);
		if (g_keys['a'])
			g_camera->RotateY(KEYBOARD_RATE_MS * 3.14159f / -2000.0f);
		if (g_keys['w'])
			g_camera->RotateX(KEYBOARD_RATE_MS * 3.14159f / 2000.0f);
		if (g_keys['s'])
			g_camera->RotateX(KEYBOARD_RATE_MS * 3.14159f / -2000.0f);
		if (g_keys['e'])
			g_camera->RotateZ(KEYBOARD_RATE_MS * 3.14159f / 2000.0f);
		if (g_keys['q'])
			g_camera->RotateZ(KEYBOARD_RATE_MS * 3.14159f / -2000.0f);
		*/

		if (g_keys['w'] || g_keys['s'] || g_keys['d'] || g_keys['a'] || g_keys['r'] || g_keys['f'])
		{
			g_camera->RotateY(g_camerayrotation);

			if (g_keys['w'])
				g_camera->Translate(0.0f, 0.0f, KEYBOARD_RATE_MS * 0.4f);
			if (g_keys['s'])
				g_camera->Translate(0.0f, 0.0f, KEYBOARD_RATE_MS * -0.4f);
			if (g_keys['d'])
				g_camera->Translate(KEYBOARD_RATE_MS * 0.4f, 0.0f, 0.0f);
			if (g_keys['a'])
				g_camera->Translate(KEYBOARD_RATE_MS * -0.4f, 0.0f, 0.0f);
			if (g_keys['r'])
				g_camera->Translate(0.0f, KEYBOARD_RATE_MS * 0.4f, 0.0f);
			if (g_keys['f'])
				g_camera->Translate(0.0f, KEYBOARD_RATE_MS * -0.4f, 0.0f);

			g_camera->RotateY(-g_camerayrotation);

			printf("Camera coords = %f, %f\n", g_camera->m_viewmatrix.m_data[3], g_camera->m_viewmatrix.m_data[11]);
		}

		if (g_keys['t'])
			g_zoom *= 1.01f;
		if (g_keys['g'])
			g_zoom /= 1.01f;

		g_keystick += KEYBOARD_RATE_MS;
	}

	return 0;
}